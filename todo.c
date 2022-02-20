/*
   ToDo CLI program, similar to Taskwarrior

   TODO add support for time quantities with argv (d, s, w, y)
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stddef.h>

#include "todo.h"

int main(int argc, char *argv[])
{
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
    long due = 0;

    // strtol returns zero when the input is not convertable
    due = strtol(days, NULL, 10);
    if (due > 0)
        due = time(NULL) + due * 86400;     // 1d = 86400 seconds

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

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", strerror(errno));
        return 0;
    }

    // read the file once to determine the longest task name
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, DELI);

        if (strlen(token) > (size_t) max_tok_len) {
            max_tok_len = strlen(token);
        }
    }
    fclose(fp);

    char out_buff[8192];
    char *target = out_buff;
    double due = 0;
    int counter = 0;

    // header
    target += sprintf(target, "INDEX\t%-*s\tDUE\n", max_tok_len, "TASK");

    // read the file a second time
    // collect all outputs in out_buff using sprintf
    fp = fopen(filename, "r");
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, DELI);
        target += sprintf(target, "%d\t", counter + 1);

        // parse all tokens in a line, separated by DELI
        while (token != NULL) {

            // since due is the last item in a row, we can recognize it by '\n'
            if (strchr(token, '\n')) {
                due = strtol(token, NULL, 10);
                if (due > 0) {
                    due -= time(NULL);
                    if (due > 0) {
                        due /= 86400;
                        target += sprintf(target, "%.0lfd\n", due);
                    }
                    else {
                        target += sprintf(target, "-\n");
                    }
                }
                else {
                    target += sprintf(target, "-\n");
                }
            }
            // if we don't have '\n' it must be the task name
            else {
                target += sprintf(target, "%-*s\t", max_tok_len, token);
            }
            token = strtok(NULL, DELI);
        }
        counter ++;
    }
    fclose(fp);
    free(line);

    if (do_print)
        printf("%s", out_buff);

    return counter;
}


void complete_task(char *filename, char *index_str)
{
    int index = 0;
    int task_count = 0;

    task_count = show_tasks(filename, 0);
    if (task_count == 0)
        return;

    index = strtol(index_str, NULL, 10);
    if (index < 1 || index > task_count) {
        printf("Invalid task index\n");
        return;
    }
    else {
        FILE *fp = NULL;
        FILE *fp_tmp = NULL;
        char tempfile[PATH_MAX];
        char str[STR_MAX];
        int counter = 0;

        // generate the temp filename (~/.todo_TMP)
        strcat(strcpy(tempfile, getenv("HOME")), "/.todo_TMP");

        fp = fopen(filename, "r");
        fp_tmp = fopen(tempfile, "w");
        if (fp == NULL || fp_tmp == NULL) {
            printf("Error opening file: %s\n", strerror(errno));
            return;
        }

        // read the original file and write to the tempfile
        while (feof(fp) == 0) {
            strcpy(str, "\0");
            fgets(str, STR_MAX, fp);

            if (feof(fp) == 0) {
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
        rename(tempfile, filename);
    }
    show_tasks(filename, 1);
    return;
}

