#ifndef KANBAN_H
#define KANBAN_H

#include <ncurses.h>

#define BUFF_SIZE 8192          // size of the buffer for the JSON data
#define FILENAME "todo.json"    // name of the JSON file
#define W_ARR_LENGTH 20         // size of the window arrays

/**
  * Set up the initial terminal screen
  * Gets called each time the screen needs to be redrawn
*/
void set_up(void);

/**
  * End the ncurses screen and restore default values for the terminal
*/
void tear_down(void);

/**
  * Add a new task to the backlog
*/
void add_task(void);

/**
  * Draw a new card in one of the three columns on the screen
  * @param *task JSON object for the task
  * @param index index indicating the position of the card
  * @param column number of the column into which the card is added
  * @returns a pointer to the newly added card
*/
WINDOW *add_card(struct json_object *task, int index, int column);

/**
  * read the JSON file defined with the FILENAME macro
*/
void read_json_file(void);

/**
  * write the JSON buffer to the file defined with the FILENAME macro
*/
void write_json_file(void);

/**
  * parse the JSON buffer and deserialize it
*/
void parse_json(void);

/**
  * move the cursor on position down and select that card
*/
void move_cursor_down(void);

/**
  * move the cursor on position up and select that card
*/
void move_cursor_up(void);

/**
  * move the cursor one column to the left
*/
void move_cursor_left(void);

/**
  * move the cursor one column to the right
*/
void move_cursor_right(void);

/**
  * highlight a new card after moving the cursor
  * @param *new_card the card to select next
*/
void highlight_new_card(WINDOW *new_card);

/**
  * select the card at the current cursor position
*/
void highlight_current_card(void);

/**
  * delete the currently selected task 
*/
void delete_task(void);

/**
  * move a task one column up or down
  * @param input keyboard that triggers the operation
*/
void move_task(char input);

/**
  * not implemented yet!
  * redraw the screen when the terminal window gets resized
*/
void resize_handler(int sig);

extern int errno;

#endif
