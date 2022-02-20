#ifndef TODO_H

#define STR_MAX 100
#define PATH_MAX 256

void add_task(char *filename, char *name, char *days);
int show_tasks(char *filename, int do_print);
void complete_task(char *filename, char *index_str);

extern int errno;

#endif
