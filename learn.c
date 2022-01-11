// continue with chapter 8

// this is a comment
/*
   this is a block comment
   compile a program with (g)cc -o <NAME> file.c
*/

#include <stdio.h>
#include "h1.h"     // include the header as we use the function below

int count;
int write(void);

int main(void)
{
    count = 10;
    char *pointer;
    pointer = "lala";
    printf("%s\n", pointer);
    return 0;
}
