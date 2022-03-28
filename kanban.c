/*
   Ncurses kanban board that saves its tasks in a json file
   requires the json-c library ('json-c' package on Arch Linux)
   gcc -Wall -Wextra -lncurses -ljson-c -std=c99 -o kanban kanban.c
*/

#include <ncurses.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <json-c/json.h>

#include "kanban.h"

static char buffer[BUFF_SIZE];  // memory buffer for the json data
static int y, x;                // size of the terminal

// arrays that hold the cards with tasks for each column
static WINDOW *w_arr_backlog[W_ARR_LENGTH];
static WINDOW *w_arr_progress[W_ARR_LENGTH];
static WINDOW *w_arr_done[W_ARR_LENGTH];

// current cursor position, represents the currently selected card
struct cursor_position {
    int col;
    int row;
} cursor_pos;

// length of each json array
static int n_backlog;
static int n_progress;
static int n_done;

int main(void)
{
    // FIXME implement screen redrawing upon window resize
    signal(SIGWINCH, resize_handler);
    set_up();

    char input;
    for (;;) {
        input = getch();    // wait for user input
        // convert uppercase chars to lowercase
        if (input >= 65 && input <= 90)
            input += 32;

        switch (input) {
            case 'a':
                add_task();
                break;
            case 'd':
                delete_task();
                break;
            case 'm':
                move_task(input);
                break;
            case 'n':
                move_task(input);
                break;
            case 'j':
                move_cursor_down();
                break;
            case 'k':
                move_cursor_up();
                break;
            case 'h':
                move_cursor_left();
                break;
            case 'l':
                move_cursor_right();
                break;
            case 'q':
                tear_down();
                goto exit;      // exit the program when pressing q
            default:
                continue;
        }
    }
    exit:
    return 0;
}

void set_up(void)
{
    initscr();
    curs_set(0);    // hide the cursor
    noecho();       // don't show typed characters
    raw();

    getmaxyx(stdscr, y, x);
    refresh();

    if (x < 100) {
        mvaddstr(y-1, 1, "Please use a terminal that is at least "
                "100 characters wide");
        refresh();
        return;
    }

    // initialize all windows
    WINDOW *w_backlog_top = newwin(5, x/3, 0, 0);
    WINDOW *w_backlog_main = newwin(y-6, x/3, 5, 0);

    WINDOW *w_progress_top = newwin(5, x/3, 0, x/3);
    WINDOW *w_progress_main = newwin(y-6, x/3, 5, x/3);

    WINDOW *w_done_top = newwin(5, x/3, 0, x/3*2);
    WINDOW *w_done_main = newwin(y-6, x/3, 5, x/3*2);

    // draw borders
    box(w_backlog_top, 0, 0);
    box(w_backlog_main, 0, 0);
    box(w_progress_top, 0, 0);
    box(w_progress_main, 0, 0);
    box(w_done_top, 0, 0);
    box(w_done_main, 0, 0);

    mvwprintw(w_backlog_top, getmaxy(w_backlog_top)/2,
            getmaxx(w_backlog_top)/2 - 3, "Backlog");

    mvwprintw(w_progress_top, getmaxy(w_progress_top)/2,
            getmaxx(w_backlog_top)/2 - 5, "In Progress");

    mvwprintw(w_done_top, getmaxy(w_progress_top)/2,
            getmaxx(w_backlog_top)/2 - 2, "Done");

    // refresh all windows
    wrefresh(w_backlog_top);
    wrefresh(w_backlog_main);
    wrefresh(w_progress_top);
    wrefresh(w_progress_main);
    wrefresh(w_done_top);
    wrefresh(w_done_main);
    refresh();

    // only read the file once, exclusively operate on the buffer afterwards
    if (buffer[0] == '\0')
        read_json_file();
    // if the buffer already contains data, we just need to parse it
    if (buffer[0] != '\0')
        parse_json();

    // highlight the card at (0,0) during the first run
    static int first_run = 1;
    if (first_run) {
        cursor_pos.row = 0;
        cursor_pos.col = 0;
        first_run = 0;
    }
    highlight_current_card();
    return;
}

void tear_down(void)
{
    echo();
    curs_set(1);
    endwin();
    return;
}

void add_task(void) {
    echo();                 // display user input
    curs_set(1);            // show the cursor
    char task_name[51];     // allow 50 chars + '\n'
    char task_due[11];      // a date string has 11 chars, including '\n'

    WINDOW *w_input = newwin(y/2, x/2, y/4, x/4);
    box(w_input, 0, 0);

    mvwaddstr(w_input, 1, 1, "Please enter a task name: ");
    mvwgetnstr(w_input, 2, 1, task_name, 50);
    // when the user presses ESC (char 27), cancel the task creation
    if (strchr(task_name, 27))
        goto cancel_task_add;
    mvwaddstr(w_input, 3, 1, "Please enter a due date: ");
    mvwgetnstr(w_input, 4, 1, task_due, 10);
    if (strchr(task_due, 27))
        goto cancel_task_add;

    // add new tasks to the backlog
    struct json_object *parsed_json;
    struct json_object *backlog;

    // if the buffer is empty here, the json file is also empty
    // hence we need to create it
    if (buffer[0] == '\0') {
        parsed_json = json_object_new_object();
        backlog = json_object_new_array();
    }
    else {
        parsed_json = json_tokener_parse(buffer);
        json_object_object_get_ex(parsed_json, "backlog", &backlog);
    }
    struct json_object *new_task = json_object_new_object();
    json_object_object_add(new_task, "name", json_object_new_string(task_name));
    json_object_object_add(new_task, "due", json_object_new_string(task_due));

    json_object_array_add(backlog, new_task);
    // initialize empty arrays, if the file does not exist
    if (buffer[0] == '\0') {
        json_object_object_add(parsed_json, "backlog", backlog);
        json_object_object_add(parsed_json, "progress", json_object_new_array());
        json_object_object_add(parsed_json, "done", json_object_new_array());
    }
    // update the buffer
    strcpy(buffer, json_object_to_json_string_ext(parsed_json, JSON_C_TO_STRING_PRETTY));
    write_json_file();

    cancel_task_add:
    set_up();
    return;
}

WINDOW *add_card(struct json_object *task, int index, int column)
{
    static WINDOW *card;
    struct json_object *name;
    struct json_object *due;
    json_object_object_get_ex(task, "name", &name);
    json_object_object_get_ex(task, "due", &due);

    // decide in which column the card will be added
    switch (column) {
        case 0:
            card = newwin(4, x/3-2, 6+4*index, 1);
            break;
        case 1:
            card = newwin(4, x/3-2, 6+4*index, x/3+1);
            break;
        case 2:
            card = newwin(4, x/3-2, 6+4*index, x/3*2+1);
            break;
    }
    box(card, 0, 0);
    mvwaddstr(card, 1, 1, json_object_get_string(name));
    mvwaddstr(card, 2, 1, json_object_get_string(due));
    wrefresh(card);
    refresh();
    return card;
}

void resize_handler(int sig)
{
    // FIXME implement screen redrawing upon window resize
    if (sig == SIGWINCH) {
        return;
    }
    return;
}

void read_json_file(void)
{
    FILE *fp;
    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        if (errno != 2) {
            // suppress file not found error, as the file will be created
            mvprintw(y-1, 1, "Error opening file: %d %s\n", errno, strerror(errno));
        }
        return;
    }
    fread(buffer, BUFF_SIZE, 1, fp);
    fclose(fp);
    return;
}

void parse_json(void)
{
    struct json_object *parsed_json;    // holds the whole json object
    struct json_object *backlog;
    struct json_object *progress;
    struct json_object *done;

    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "backlog", &backlog);
    json_object_object_get_ex(parsed_json, "progress", &progress);
    json_object_object_get_ex(parsed_json, "done", &done);

    // get the length of each array
    n_backlog = json_object_array_length(backlog);
    n_progress = json_object_array_length(progress);
    n_done = json_object_array_length(done);

    // add cards for each element in each array
    for (int i=0; i<n_backlog; i++) {
        w_arr_backlog[i] = add_card(json_object_array_get_idx(backlog, i), i, 0);
    }
    for (int i=0; i<n_progress; i++) {
        w_arr_progress[i] = add_card(json_object_array_get_idx(progress, i), i, 1);
    }
    for (int i=0; i<n_done; i++) {
        w_arr_done[i] = add_card(json_object_array_get_idx(done, i), i, 2);
    }
    return;
}

void write_json_file(void)
{
    FILE *fp;
    fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        mvprintw(y-1, 1, "Error opening file: %s\n", strerror(errno));
        return;
    }
    fputs(buffer, fp);
    fclose(fp);
    return;
}

void move_cursor_down(void)
{
    switch (cursor_pos.col) {
        case 0:
            // prevent the cursor from moving out of bounds
            if (cursor_pos.row < n_backlog - 1) {
                highlight_new_card(w_arr_backlog[cursor_pos.row + 1]);
                cursor_pos.row += 1;
            }
            break;
        case 1:
            if (cursor_pos.row < n_progress - 1) {
                highlight_new_card(w_arr_progress[cursor_pos.row + 1]);
                cursor_pos.row += 1;
            }
            break;
        case 2:
            if (cursor_pos.row < n_done - 1) {
                highlight_new_card(w_arr_done[cursor_pos.row + 1]);
                cursor_pos.row += 1;
            }
            break;
    }
    return;
}

void move_cursor_up(void)
{
    switch (cursor_pos.col) {
        case 0:
            // prevent the cursor from moving out of bounds
            if (cursor_pos.row > 0) {
                highlight_new_card(w_arr_backlog[cursor_pos.row - 1]);
                cursor_pos.row -= 1;
            }
            break;
        case 1:
            if (cursor_pos.row > 0) {
                highlight_new_card(w_arr_progress[cursor_pos.row - 1]);
                cursor_pos.row -= 1;
            }
            break;
        case 2:
            if (cursor_pos.row > 0) {
                highlight_new_card(w_arr_done[cursor_pos.row - 1]);
                cursor_pos.row -= 1;
            }
            break;
    }
    return;
}

void move_cursor_left(void)
{
    switch (cursor_pos.col) {
        case 0:
            // prevent the cursor from moving out of bounds
            break;
        case 1:
            // move the cursor right if a card is in the same row position
            if (cursor_pos.row < n_backlog) {
                highlight_new_card(w_arr_backlog[cursor_pos.row]);
                cursor_pos.col -= 1;
            }
            // else select the last card of the next column
            else {
                highlight_new_card(w_arr_backlog[n_backlog - 1]);
                cursor_pos.col -= 1;
                cursor_pos.row = n_backlog - 1;
            }
            break;
        case 2:
            if (cursor_pos.row < n_progress) {
                highlight_new_card(w_arr_progress[cursor_pos.row]);
                cursor_pos.col -= 1;
            }
            else {
                highlight_new_card(w_arr_progress[n_progress - 1]);
                cursor_pos.col -= 1;
                cursor_pos.row = n_progress - 1;
            }
            break;
    }
    return;
}

void move_cursor_right(void)
{
    switch (cursor_pos.col) {
        case 0:
            // move the cursor right if a card is in the same row position
            if (cursor_pos.row < n_progress) {
                highlight_new_card(w_arr_progress[cursor_pos.row]);
                cursor_pos.col += 1;
            }
            // else select the last card of the next column
            else {
                highlight_new_card(w_arr_progress[n_progress - 1]);
                cursor_pos.col += 1;
                cursor_pos.row = n_progress - 1;
            }
            break;
        case 1:
            if (cursor_pos.row < n_done) {
                highlight_new_card(w_arr_done[cursor_pos.row]);
                cursor_pos.col += 1;
            }
            else {
                highlight_new_card(w_arr_done[n_done - 1]);
                cursor_pos.col += 1;
                cursor_pos.row = n_done - 1;
            }
            break;
        case 2:
            // prevent the cursor from moving out of bounds
            break;
    }
    return;
}

void highlight_current_card(void)
{
    WINDOW *current_card;
    // get the currently selected card from the current cursor position
    switch (cursor_pos.col) {
        case 0:
            current_card = w_arr_backlog[cursor_pos.row];
            break;
        case 1:
            current_card = w_arr_progress[cursor_pos.row];
            break;
        case 2:
            current_card = w_arr_done[cursor_pos.row];
            break;
    }
    // change the border to make the selection distinct
    wborder(current_card, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(current_card);
    return;
}

void highlight_new_card(WINDOW *new_card)
{
    WINDOW *old_card;
    // get the currently selected card from the current cursor position
    switch (cursor_pos.col) {
        case 0:
            old_card = w_arr_backlog[cursor_pos.row];
            break;
        case 1:
            old_card = w_arr_progress[cursor_pos.row];
            break;
        case 2:
            old_card = w_arr_done[cursor_pos.row];
            break;
    }
    // change the border to make the selection distinct
    box(old_card, 0, 0);
    wborder(new_card, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(old_card);
    wrefresh(new_card);
    return;
}

void delete_task(void)
{
    struct json_object *parsed_json;
    struct json_object *backlog;
    struct json_object *progress;
    struct json_object *done;

    parsed_json = json_tokener_parse(buffer);
    // get the currently selected card
    switch (cursor_pos.col) {
        case 0:
            json_object_object_get_ex(parsed_json, "backlog", &backlog);
            json_object_array_del_idx(backlog, cursor_pos.row, 1);
            // force-reset the corresponding array containing all cards
            memset(w_arr_backlog, 0, sizeof w_arr_backlog);
            // move the cursor to prevent it from being out of bounds
            if (cursor_pos.row == n_backlog - 1)
                cursor_pos.row -= 1;
            if (n_backlog == 1) {
                if (n_progress > 0) {
                    cursor_pos.col = 1;
                }
                else if (n_done > 0) {
                    cursor_pos.col = 2;
                }
                else {
                    cursor_pos.col = 0;
                }
                cursor_pos.row = 0;
            }
            break;
        case 1:
            json_object_object_get_ex(parsed_json, "progress", &progress);
            json_object_array_del_idx(progress, cursor_pos.row, 1);
            memset(w_arr_progress, 0, sizeof w_arr_progress);
            if (cursor_pos.row == n_progress - 1)
                cursor_pos.row -= 1;
            if (n_progress == 1) {
                if (n_backlog > 0) {
                    cursor_pos.col = 0;
                }
                else if (n_done > 0) {
                    cursor_pos.col = 2;
                }
                else {
                    cursor_pos.col = 0;
                }
                cursor_pos.row = 0;
            }
            break;
        case 2:
            json_object_object_get_ex(parsed_json, "done", &done);
            json_object_array_del_idx(done, cursor_pos.row, 1);
            memset(w_arr_done, 0, sizeof w_arr_done);
            if (cursor_pos.row == n_done - 1)
                cursor_pos.row -= 1;
            if (n_done == 1) {
                if (n_backlog > 0) {
                    cursor_pos.col = 0;
                }
                else if (n_progress > 0) {
                    cursor_pos.col = 1;
                }
                else {
                    cursor_pos.col = 0;
                }
                cursor_pos.row = 0;
            }
            break;
    }
    // update the buffer, save the file and redraw the screen
    strcpy(buffer, json_object_to_json_string_ext(parsed_json, JSON_C_TO_STRING_PRETTY));
    write_json_file();
    set_up();
    return;
}

void move_task(char input)
{
    struct json_object *parsed_json;
    struct json_object *backlog;
    struct json_object *progress;
    struct json_object *done;
    struct json_object *task;           // task to move

    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "backlog", &backlog);
    json_object_object_get_ex(parsed_json, "progress", &progress);
    json_object_object_get_ex(parsed_json, "done", &done);

    switch (input) {
        // move the task right
        case 'm':
            switch (cursor_pos.col) {
                case 0:
                    task = json_object_array_get_idx(backlog, cursor_pos.row);
                    // use json_object_get() to retain the reference count
                    json_object_array_add(progress, json_object_get(task));
                    json_object_array_del_idx(backlog, cursor_pos.row, 1);
                    break;
                case 1:
                    task = json_object_array_get_idx(progress, cursor_pos.row);
                    json_object_array_add(done, json_object_get(task));
                    json_object_array_del_idx(progress, cursor_pos.row, 1);
                    break;
                case 2:
                    break;
            }
            break;
        // move the task left
        case 'n':
            switch (cursor_pos.col) {
                case 0:
                    break;
                case 1:
                    task = json_object_array_get_idx(progress, cursor_pos.row);
                    json_object_array_add(backlog, json_object_get(task));
                    json_object_array_del_idx(progress, cursor_pos.row, 1);
                    break;
                case 2:
                    task = json_object_array_get_idx(done, cursor_pos.row);
                    json_object_array_add(progress, json_object_get(task));
                    json_object_array_del_idx(done, cursor_pos.row, 1);
                    break;
            }
            break;
    }
    // update the buffer and save the file
    strcpy(buffer, json_object_to_json_string_ext(parsed_json, JSON_C_TO_STRING_PRETTY));
    write_json_file();

    // move the cursor one position up
    cursor_pos.row -= 1;
    if (cursor_pos.row < 0)
        cursor_pos.row = 0;

    // reset all card window arrays
    memset(w_arr_backlog, 0, sizeof w_arr_backlog);
    memset(w_arr_progress, 0, sizeof w_arr_progress);
    memset(w_arr_done, 0, sizeof w_arr_done);
    set_up();
    return;
}

