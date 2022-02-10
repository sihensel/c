/*
   ToDo CLI program, similar to Taskwarrior
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STR_MAX 100
#define PATH_MAX 256

void add_task(char *, char *);
int show_tasks(char *);
void complete_task(char *);

int main(int argc, char *argv[])
{

    // FIXME make this configurable
    char filename[PATH_MAX];

    // concatenate $HOME and "/.todo" into filename
    strcat(strcpy(filename, getenv("HOME")), "/.todo");

    if (argc == 1) {
        show_tasks(filename);
    }
    else {
        if (strcmp(argv[1], "add") == 0) {
            char name[STR_MAX];
            printf("Enter the task name:\n");
            fgets(name, STR_MAX, stdin);
            add_task(filename, name);
        }
        else if (strcmp(argv[1], "list") == 0) {
            show_tasks(filename);
        }
        else if (strcmp(argv[1], "done") == 0) {
            complete_task(filename);
        }
        else {
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
        char name[STR_MAX];
        unsigned int due;   // due day, Unix timestring? is int big enough?
    } task;

    strcpy(task.name, task_name);
    task.due = 0;

    len = strlen(task.name);

    // null-terminate newline strings, as produced by fgets()
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

int show_tasks(char *filename)
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
        return 0;
    }

    // read the file the first time to determine the longest task name
    // FIXME is there a better solution to this?
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, DELI);
        if (strlen(token) > max_tok_len)
            max_tok_len = strlen(token);
    }
    // output headers
    printf("INDEX\t%-*s\tDUE\n", max_tok_len, "TASK");

    // read the file a second time to render it to the user
    fp = fopen(filename, "r");
    int counter = 0;
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, DELI);
        printf("%d\t", counter + 1);

        // parse all tokens in a line, separated by DELI
        while (token != NULL) {
            if (strchr(token, '\n')) {
                printf("%s", token);
            }
            else {
                printf("%-*s\t", max_tok_len, token);
            }
            token = strtok(NULL, DELI);
        }
        counter ++;
    }
    fclose(fp);
    free(line);

    return counter;
}

void complete_task(char *filename)
{
    unsigned int index = 0;
    unsigned int no_of_tasks = 0;

    no_of_tasks = show_tasks(filename);

    if (no_of_tasks == 0) {
        return;
    }

    printf("\nSpecify the index of the task to complete:\n");
    scanf("%d", &index);

    if (index < 1 || index > no_of_tasks) {
        printf("Invalid input (please specify a valid index).\n");
        return;
    }
    else {
        // FIXME writing to a tempfile for now
        // can be replaced by loading the whole file into the buffer
        char tmpfile[PATH_MAX];
        FILE *fp = NULL;
        FILE *fp_tmp = NULL;
        char str[STR_MAX];
        int counter= 0;

        strcat(strcpy(tmpfile, getenv("HOME")), "/.todo_TMP");

        fp = fopen(filename, "r");
        fp_tmp = fopen(tmpfile, "w");
        if (fp == NULL || fp_tmp == NULL) {
            printf("Could not open file %s or %s\n", filename, tmpfile);
            return;
        }

        // read the original file
        while (!feof(fp)) {
            strcpy(str, "\0");
            fgets(str, STR_MAX, fp);

            if (!feof(fp)) {
                counter ++;

                // skip the line that should be removed
                if (counter != index)
                    fprintf(fp_tmp, "%s", str);
            }
        }
        fclose(fp);
        fclose(fp_tmp);

        remove(filename);
        rename(tmpfile, filename);
    }
    return;
}
