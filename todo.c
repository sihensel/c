/*
   ToDo CLI program, similar to Taskwarrior
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STR_SIZE 100

void add_task(char *filename, char *name);
void show_tasks(void);
void complete_task(char *filename);

int main(int argc, char *argv[])
{

    // FIXME make this configurable
    char *homedir = getenv("HOME");
    char filename[STR_SIZE] = "/.todo";

    // concatenate the filename and the homedir into filename
    strcpy(filename, strcat(homedir, filename));

    if (argc == 1) {
        show_tasks();
    } else {
        if (strcmp(argv[1], "add") == 0) {
            char name[STR_SIZE];
            printf("Enter the task name\n");
            fgets(name, STR_SIZE, stdin);
            add_task(filename, name);
        } else if (strcmp(argv[1], "list") == 0) {
            complete_task(filename);
        } else if (strcmp(argv[1], "list") == 0) {
            show_tasks();
        } else {
            printf("Unknown argument '%s'.\n", argv[1]);
        }
    }
    return 0;
}

void add_task(char *filename, char *task_name)
{
    unsigned int len = 0;   // len of task_name

    struct {
        char name[STR_SIZE];
        int is_done;
        unsigned int due;   // due day, Unix timestring? is int big enough?
    } task;

    strcpy(task.name, task_name);
    task.due = 0;
    task.is_done = 0;

    len = strlen(task_name);

    if (task_name[len - 1] == '\n')
        task_name[len - 1] = '\0';

    FILE *file;
    file = fopen(filename, "a");
    fprintf(file, "%s\t%d\t%d\n", task_name, task.due, task.is_done);
    fclose(file);
}

void show_tasks(void)
{
    printf("INDEX\tTASK\tDUE\tDONE\n");
    return;
}

void complete_task(char *filename) {
    return;
}
