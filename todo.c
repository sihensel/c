/*
   ToDo CLI program, similar to Taskwarrior
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define STR_MAX 100
#define PATH_MAX 256

void add_task(char *);
int show_tasks(char *);
void complete_task(char *);

extern int errno;

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
            add_task(filename);
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


void add_task(char *filename)
{
    char input[STR_MAX];
    char name[STR_MAX];
    long due = 0;
    FILE *fp = NULL;
    const char* DELI = " +";
    char *token = NULL;

    printf("Enter the task name:\n");
    fgets(input, STR_MAX, stdin);

    // null-terminate newline strings, as produced by fgets()
    if (input[strlen(input) - 1] == '\n')
        input[strlen(input) - 1] = '\0';


    if (strstr(input, " +")) {
        token = strtok(input, DELI);
        strcpy(name, token);

        while (token != NULL) {
            token = strtok(NULL, "\0");
            due = strtol(token, NULL, 10);
            due = time(NULL) + due * 86400;     // 86400 = 1 day in seconds
            break;
        }
    }
    else {
        strcpy(name, input);
    }

    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }

    fprintf(fp, "%s\t%ld\n", name, due);
    fclose(fp);
    return;
}

int show_tasks(char *filename)
{
    FILE *fp = NULL;
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    int max_tok_len = 0;
    const char *DELI = "\t";
    double due = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", strerror(errno));
        return 0;
    }

    // read the file the first time to determine the longest task name
    // FIXME is there a better solution to this?
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, DELI);

        if (strlen(token) > max_tok_len) {
            max_tok_len = strlen(token);
        }
    }
    fclose(fp);

    // output headers
    printf("INDEX\t%-*s\tDUE\n", max_tok_len, "TASK");

    // read the file a second time and print to stdout
    fp = fopen(filename, "r");
    int counter = 0;
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, DELI);
        printf("%d\t", counter + 1);

        // parse all tokens in a line, separated by DELI
        while (token != NULL) {
            if (strchr(token, '\n')) {
                due = strtol(token, NULL, 10);
                if (due > 0) {
                    due -= time(NULL);
                    if (due > 0) {
                        due /= 86400;
                        printf("%.0lfd\n", due);
                    }
                    else {
                        printf("-\n");
                    }
                }
                else {
                    printf("-\n");
                }
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
    int index = 0;
    int task_count = 0;

    task_count = show_tasks(filename);

    if (task_count == 0) {
        return;
    }

    // FIXME get input via argv[]
    printf("\nSpecify the index of the task to complete:\n");
    scanf("%d", &index);

    if (index < 1 || index > task_count) {
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
            printf("Error opening file: %s\n", strerror(errno));
            return;
        }

        // read the original file
        while (!feof(fp)) {
            strcpy(str, "\0");
            fgets(str, STR_MAX, fp);

            if (!feof(fp)) {
                counter ++;

                // skip the line that should be removed
                if (counter != index) {
                    fprintf(fp_tmp, "%s", str);
                }
            }
        }
        fclose(fp);
        fclose(fp_tmp);

        remove(filename);
        rename(tmpfile, filename);
    }
    return;
}
