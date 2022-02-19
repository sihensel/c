/*
   ToDo CLI program, similar to Taskwarrior

   TODO add support for time quantities (d, s, w, y)
   TODO resolve all FIXME
   TODO refactor code and add proper comments
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define STR_MAX 100
#define PATH_MAX 256

void add_task(char *filename, char *name, char *days);
int show_tasks(char *filename, int do_print);
void complete_task(char *filename, char *index_str);

extern int errno;

int main(int argc, char *argv[])
{
    // FIXME make this configurable
    char filename[PATH_MAX];
    // concatenate $HOME and "/.todo" into filename
    strcat(strcpy(filename, getenv("HOME")), "/.todo");

    if (argc == 1) {
        show_tasks(filename, 1);
    }
    else if (argc >= 3 && strcmp(argv[1], "add") == 0) {
        if (argc == 3)
            add_task(filename, argv[2], "0");
        else if (argc >= 4)
            add_task(filename, argv[2], argv[3]);
    }
    else if (argc >= 3 && strcmp(argv[1], "done") == 0) {
        complete_task(filename, argv[2]);
    }
    return 0;
}


void add_task(char *filename, char *name, char *days)
{
    FILE *fp = NULL;
    const char* DELI = " +";
    char *token = NULL;
    long due = 0;

    due = strtol(days, NULL, 10);
    if (due > 0)
        due = time(NULL) + due * 86400;

    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }
    fprintf(fp, "%s\t%ld\n", name, due);
    fclose(fp);
    return;
}

int show_tasks(char *filename, int do_print)
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
    if (do_print)
        printf("INDEX\t%-*s\tDUE\n", max_tok_len, "TASK");

    // read the file a second time and print to stdout
    fp = fopen(filename, "r");
    int counter = 0;
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, DELI);
        if (do_print)
            printf("%d\t", counter + 1);

        // parse all tokens in a line, separated by DELI
        while (token != NULL) {
            if (strchr(token, '\n')) {
                due = strtol(token, NULL, 10);
                if (due > 0) {
                    due -= time(NULL);
                    if (due > 0) {
                        due /= 86400;
                        // FIXME this is ugly
                        if (do_print)
                            printf("%.0lfd\n", due);
                    }
                    else {
                        if (do_print)
                            printf("-\n");
                    }
                }
                else {
                    if (do_print)
                        printf("-\n");
                }
            }
            else {
                if (do_print)
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


void complete_task(char *filename, char *index_str)
{
    int index = 0;
    int task_count = 0;
    printf("COMPLETE TASK\n");

    task_count = show_tasks(filename, 0);

    if (task_count == 0) {
        return;
    }

    // FIXME get input via argv[]

    index = strtol(index_str, NULL, 10);
    if (index < 1 || index > task_count) {
        printf("Invalid task index\n");
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

