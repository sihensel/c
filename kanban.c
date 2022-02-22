/*
   WIP
   Also testing ncurses
   compile with gcc -lncurses -std=c99 dnd.c
*/

#include <ncurses.h>
#include <string.h>

#define COLOR_BACKLOG 1
#define COLOR_PROCESS 2
#define COLOR_DONE    3

void set_up(void);
void tear_down(void);
void add_card(void);

int main(void)
{
    set_up();

    add_card();

    // logic here
    getch();

    tear_down();
    return 0;
}

void set_up(void)
{
    WINDOW *w_backlog_top;
    WINDOW *w_backlog_main;

    WINDOW *w_process_top;
    WINDOW *w_process_main;

    WINDOW *w_done_top;
    WINDOW *w_done_main;
    int x, y;

    initscr();
    curs_set(0);    // hide the cursor
    noecho();

    getmaxyx(stdscr, y, x);
    refresh();

    if (x < 100) {
        addstr("Please use a terminal that is at least 100 characters wide");
        refresh();
        return;
    }

    // rows, cols, y-origin, x-origin
    w_backlog_top = newwin(5, x/3, 0, 0);
    w_backlog_main = newwin(y-5, x/3, 5, 0);

    w_process_top = newwin(5, x/3, 0, x/3);
    w_process_main = newwin(y-5, x/3, 5, x/3);

    w_done_top = newwin(5, x/3, 0, x/3*2);
    w_done_main = newwin(y-5, x/3, 5, x/3*2);

    box(w_backlog_top, 0, 0);
    box(w_backlog_main, 0, 0);
    box(w_process_top, 0, 0);
    box(w_process_main, 0, 0);
    box(w_done_top, 0, 0);
    box(w_done_main, 0, 0);

    //mvwprintw(w_done_top, 2, 1, "x: %d\ty: %d", x, y);

    if (has_colors()) {
        if (start_color() == OK) {;
            use_default_colors();
            // identifier, fg, bg
            init_pair(COLOR_BACKLOG, COLOR_BLUE, COLOR_BLACK);
            init_pair(COLOR_PROCESS, COLOR_YELLOW, COLOR_BLACK);
            init_pair(COLOR_DONE, COLOR_GREEN, COLOR_BLACK);

            wattron(w_backlog_top, COLOR_PAIR(COLOR_BACKLOG));
            mvwprintw(w_backlog_top, getmaxy(w_backlog_top)/2,
                    getmaxx(w_backlog_top)/2 - strlen("Backlog")/2, "Backlog");
            wattroff(w_backlog_top, COLOR_PAIR(COLOR_BACKLOG));

            wattron(w_process_top, COLOR_PAIR(COLOR_PROCESS));
            mvwprintw(w_process_top, getmaxy(w_process_top)/2,
                    getmaxx(w_backlog_top)/2 - strlen("In Process")/2, "In Process");
            wattroff(w_process_top, COLOR_PAIR(COLOR_PROCESS));

            wattron(w_done_top, COLOR_PAIR(COLOR_DONE));
            mvwprintw(w_done_top, getmaxy(w_process_top)/2,
                    getmaxx(w_backlog_top)/2 - strlen("Done")/2, "Done");
            wattroff(w_done_top, COLOR_PAIR(COLOR_DONE));
        }
        else {
            addstr("Failed to start colors");
            refresh();
        }
    }
    else {
        mvwprintw(w_backlog_top, getmaxy(w_backlog_top)/2,
                getmaxx(w_backlog_top)/2 - strlen("Backlog")/2, "Backlog");

        mvwprintw(w_process_top, getmaxy(w_process_top)/2,
                getmaxx(w_backlog_top)/2 - strlen("In Process")/2, "In Process");

        mvwprintw(w_done_top, getmaxy(w_process_top)/2,
                getmaxx(w_backlog_top)/2 - strlen("Done")/2, "Done");
    }

    wrefresh(w_backlog_top);
    wrefresh(w_backlog_main);
    wrefresh(w_process_top);
    wrefresh(w_process_main);
    wrefresh(w_done_top);
    wrefresh(w_done_main);

    return;
}

void tear_down(void)
{
    echo();
    curs_set(1);
    endwin();

    return;
}


void add_card(void)
{
    WINDOW *card_left;
    WINDOW *card_second;
    WINDOW *card_middle;
    WINDOW *card_right;
    int x = getmaxx(stdscr);
    refresh();

    // FIXME assign window coordinates dynamically
    // Backlog
    // newwin(5, x/3-2, 6 + 5 * no_of_cards, x_start)
    card_left = newwin(5, x/3-2, 6, 1);
    card_second = newwin(5, x/3-2, 11, 1);
    // Process
    card_middle = newwin(5, x/3-2, 6, x/3+1);
    // Done
    card_right = newwin(5, x/3-2, 6, x/3*2+1);

    box(card_left, 0, 0);
    box(card_second, 0, 0);
    box(card_middle, 0, 0);
    box(card_right, 0, 0);

    mvwprintw(card_left, 1, 1, "Task Name");
    mvwprintw(card_left, 2, 1, "Do this and then to this");
    mvwprintw(card_left, 3, 1, "01.01.2022");

    wrefresh(card_left);
    wrefresh(card_second);
    wrefresh(card_middle);
    wrefresh(card_right);
    return;
}
