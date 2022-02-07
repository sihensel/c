/*
   ToDo CLI program, similar to Taskwarrior
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STR_SIZE 100

void add_task(char *filename, char *name);
void show_tasks(char *filename);
void complete_task(char *filename);

int main(int argc, char *argv[])
{

    // FIXME make this configurable
    char *homedir = getenv("HOME");
    char filename[STR_SIZE] = "/.todo";

    // concatenate the filename and the homedir into filename
    strcpy(filename, strcat(homedir, filename));

    if (argc == 1) {
        show_tasks(filename);
    } else {
        if (strcmp(argv[1], "add") == 0) {
            char name[STR_SIZE];
            printf("Enter the task name\n");
            fgets(name, STR_SIZE, stdin);
            add_task(filename, name);
        } else if (strcmp(argv[1], "done") == 0) {
            complete_task(filename);
        } else if (strcmp(argv[1], "list") == 0) {
            show_tasks(filename);
        } else {
            printf("Unknown argument '%s'.\n", argv[1]);
        }
    }
    return 0;
}

void add_task(char *filename, char *task_name)
{
    FILE *fp;
    unsigned int len = 0;   // len of task_name

    // maybe remove this struct later
    struct {
        char name[STR_SIZE];
        unsigned int due;   // due day, Unix timestring? is int big enough?
    } task;

    strcpy(task.name, task_name);
    task.due = 0;

    len = strlen(task.name);

    if (task.name[len - 1] == '\n')
        task.name[len - 1] = '\0';

    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        return;
    }

    fprintf(fp, "%s\t%d\n", task.name, task.due);
    fclose(fp);
    return;
}

void show_tasks(char *filename)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    unsigned int max_tok_len = 0;
    const char *DELI = "\t";

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        return;
    }

    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, DELI);
        if (strlen(token) > max_tok_len)
            max_tok_len = strlen(token);
    }

    printf("INDEX\t%-*s\tDUE\n", max_tok_len, "TASK");

    fp = fopen(filename, "r");
    int counter = 0;
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, DELI);
        printf("%d\t", counter);

        // parse all tokens in a line, separated by DELI
        while (token != NULL) {
            if (strchr(token, 10)) {
                printf("%s", token);
            } else {
                printf("%-*s\t", max_tok_len, token);
            }
            token = strtok(NULL, DELI);
        }
        counter ++;
    }

    fclose(fp);
    free(line);

    return;
}

void complete_task(char *filename) {
    return;
}
