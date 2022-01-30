/*
   ToDo CLI program, similar to Taskwarrior
*/

#include <stdio.h>
#include <string.h>

#define STR_SIZE 100

void add_task(char *);
void show_tasks(void);

int main(int argc, char *argv[])
{
    if (argc == 1) {
        show_tasks();
    }
    else {
        if (strcmp(argv[1], "add") == 0) {
            char title[STR_SIZE];
            printf("Enter the task title\n");
            fgets(title, STR_SIZE, stdin);
            add_task(title);
        }
        else {
            printf("Argument '%s' not known\n", argv[1]);
        }
    }


    return 0;

}

void add_task(char *title)
{
    struct {
        char title[STR_SIZE];
        int is_done;
    } task;

    strcpy(task.title, title);
    task.is_done = 0;

    printf("%s%d\n", task.title, task.is_done);
}
