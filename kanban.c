/*
   Ncurses kanban board that saves its tasks in a json file
   Also testing ncurses
   requires the json-c library ('json-c' package on Arch Linux)
   compile with gcc -lncurses -ljson-c -std=c99 kanban.c
*/

#include <ncurses.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <json-c/json.h>

// define color pairs to make reusing them easier
#define COLOR_BACKLOG  1
#define COLOR_PROGRESS 2
#define COLOR_DONE     3
// FIXME set buffer size to a reasonable amount
#define BUFF_SIZE 8192
#define FILENAME "ncurses.json"

// TODO write dedicated header file
void set_up(void);
void tear_down(void);
void add_task(void);
void add_card(struct json_object *task, int index, int column);
void resize_handler(int sig);
void read_json_file(void);
void write_json_file(void);
void parse_json(void);

extern int errno;
static char buffer[BUFF_SIZE];  // buffer for the json file
static int y, x;                // size of the terminal

int main(void)
{
    // FIXME implement screen redrawing upon window resize
    signal(SIGWINCH, resize_handler);

    set_up();

    char input = 0;
    for (;;) {
        input = getch();
        if (input == 'a' || input == 'A') {
            add_task();
        }
        else if (input == 'h' || input == 'H') {
            // TODO add help menu
        }
        else if (input == 'q' || input == 'Q') {
            tear_down();
            return 0;
        }
    }
    return 0;
}

void set_up(void)
{
    initscr();
    curs_set(0);    // hide the cursor
    noecho();
    keypad(stdscr, TRUE);
    raw();

    getmaxyx(stdscr, y, x);
    refresh();

    if (x < 100) {
        addstr("Please use a terminal that is at least 100 characters wide");
        refresh();
        return;
    }

    // newwin(rows, cols, y-origin, x-origin)
    WINDOW *w_backlog_top = newwin(5, x/3, 0, 0);
    WINDOW *w_backlog_main = newwin(y-7, x/3, 5, 0);

    WINDOW *w_progress_top = newwin(5, x/3, 0, x/3);
    WINDOW *w_progress_main = newwin(y-7, x/3, 5, x/3);

    WINDOW *w_done_top = newwin(5, x/3, 0, x/3*2);
    WINDOW *w_done_main = newwin(y-7, x/3, 5, x/3*2);

    box(w_backlog_top, 0, 0);
    box(w_backlog_main, 0, 0);
    box(w_progress_top, 0, 0);
    box(w_progress_main, 0, 0);
    box(w_done_top, 0, 0);
    box(w_done_main, 0, 0);

    if (has_colors()) {
        if (start_color() == OK) {;
            use_default_colors();
            // init_pair(identifier, fg, bg)
            init_pair(COLOR_BACKLOG, COLOR_BLUE, COLOR_BLACK);
            init_pair(COLOR_PROGRESS, COLOR_YELLOW, COLOR_BLACK);
            init_pair(COLOR_DONE, COLOR_GREEN, COLOR_BLACK);

            wattron(w_backlog_top, COLOR_PAIR(COLOR_BACKLOG));
            mvwprintw(w_backlog_top, getmaxy(w_backlog_top)/2,
                    getmaxx(w_backlog_top)/2 - strlen("Backlog")/2, "Backlog");
            wattroff(w_backlog_top, COLOR_PAIR(COLOR_BACKLOG));

            wattron(w_progress_top, COLOR_PAIR(COLOR_PROGRESS));
            mvwprintw(w_progress_top, getmaxy(w_progress_top)/2,
                    getmaxx(w_backlog_top)/2 - strlen("In Progress")/2, "In Progress");
            wattroff(w_progress_top, COLOR_PAIR(COLOR_PROGRESS));

            wattron(w_done_top, COLOR_PAIR(COLOR_DONE));
            mvwprintw(w_done_top, getmaxy(w_progress_top)/2,
                    getmaxx(w_backlog_top)/2 - strlen("Done")/2, "Done");
            wattroff(w_done_top, COLOR_PAIR(COLOR_DONE));
        }
        else {
            addstr("Failed to start colors");
            refresh();
        }
    }
    else {
        // print text without colors
        mvwprintw(w_backlog_top, getmaxy(w_backlog_top)/2,
                getmaxx(w_backlog_top)/2 - strlen("Backlog")/2, "Backlog");

        mvwprintw(w_progress_top, getmaxy(w_progress_top)/2,
                getmaxx(w_backlog_top)/2 - strlen("In Progress")/2, "In Progress");

        mvwprintw(w_done_top, getmaxy(w_progress_top)/2,
                getmaxx(w_backlog_top)/2 - strlen("Done")/2, "Done");
    }

    mvaddstr(y-2, 1, "(a)dd task");
    mvaddstr(y-2, x/2, "(q)uit");

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
    if (buffer[0] != '\0')
        parse_json();
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
    echo();
    char task_name[51];
    char task_due[11];      // a date string has 11 chars, including '\n'

    WINDOW *w_input = newwin(y/2, x/2, y/4, x/4);
    box(w_input, 0, 0);

    mvwaddstr(w_input, 1, 1, "Please enter a task name: ");
    mvwgetnstr(w_input, 2, 1, task_name, 50);
    mvwaddstr(w_input, 3, 1, "Please enter a due date (DD.MM.YYYY or 0 for none): ");
    mvwgetnstr(w_input, 4, 1, task_due, 10);

    // we just assume that new tasks get added to the backlog first
    // and then moved manually to the progress column
    struct json_object *parsed_json;
    struct json_object *backlog;

    // if the buffer is empty here, the json file is also empty
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
    if (buffer[0] == '\0') {
        json_object_object_add(parsed_json, "backlog", backlog);
        json_object_object_add(parsed_json, "progress", json_object_new_array());
        json_object_object_add(parsed_json, "done", json_object_new_array());
    }
    // update the buffer
    strcpy(buffer, json_object_to_json_string_ext(parsed_json, JSON_C_TO_STRING_PRETTY));
    write_json_file();

    set_up();
    noecho();
    return;
}

void add_card(struct json_object *task, int index, int column)
{
    WINDOW *card;
    struct json_object *name;
    struct json_object *due;
    json_object_object_get_ex(task, "name", &name);
    json_object_object_get_ex(task, "due", &due);

    switch (column) {
        case 1:
            card = newwin(5, x/3-2, 6+5*index, 1);
            break;
        case 2:
            card = newwin(5, x/3-2, 6+5*index, x/3+1);
            break;
        case 3:
            card = newwin(5, x/3-2, 6+5*index, x/3*2+1);
            break;
        default:
            // this case should not occur
            break;
    }

    // Backlog
    // newwin(5, x/3-2, 6 + 5 * no_of_cards, x_start)
    // card_second = newwin(5, x/3-2, 11, 1);
    // Progress
    // card_middle = newwin(5, x/3-2, 6, x/3+1);
    // Done
    // card_right = newwin(5, x/3-2, 6, x/3*2+1);
    box(card, 0, 0);

    mvwaddstr(card, 1, 1, json_object_get_string(name));
    mvwaddstr(card, 2, 1, json_object_get_string(due));

    wrefresh(card);
    refresh();
    return;
}

void resize_handler(int sig)
{
    // FIXME implement screen redrawing upon window resize
    return;
}

void read_json_file(void)
{
    FILE *fp;
    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        // FIXME suppress the error message when the file does not exist
        // use access() from unistd.h or stat() from sys/stat.h
        mvprintw(y-1, 1, "Error opening file: %s\n", strerror(errno));
        return;
    }
    fread(buffer, BUFF_SIZE, 1, fp);
    fclose(fp);
    return;
}

void parse_json(void)
{
    struct json_object *parsed_json;    // holds the parsed json
    struct json_object *backlog;
    struct json_object *progress;
    struct json_object *done;
    int n_backlog, n_progress, n_done;

    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "backlog", &backlog);
    json_object_object_get_ex(parsed_json, "progress", &progress);
    json_object_object_get_ex(parsed_json, "done", &done);

    n_backlog = json_object_array_length(backlog);
    n_progress = json_object_array_length(progress);
    n_done = json_object_array_length(done);

    for (int i=0; i<n_backlog; i++) {
        add_card(json_object_array_get_idx(backlog, i), i, 1);
    }

    for (int i=0; i<n_progress; i++) {
        add_card(json_object_array_get_idx(progress, i), i, 2);
    }

    for (int i=0; i<n_done; i++) {
        add_card(json_object_array_get_idx(done, i), i, 3);
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

