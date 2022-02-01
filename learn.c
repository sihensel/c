// this is a comment
/*
   this is a block comment
   compile a program with (g)cc -o <NAME> file.c
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

extern int errno;

int factorial(unsigned int i)
{
    if (i <= 1) {
        return 1;
    }
    else {
        return i * factorial(i - 1);
    }
}

double avg(int num, ...)      // the first parameter is always an int and representats the number of parameters
{
    va_list valist;         // variable list type
    double sum = 0.0;
    
    va_start(valist, num);  // initialize the variable list
    for (int i = 0; i < num; i++) {
        sum += va_arg(valist, int);
    }
    va_end(valist);         // clean up memory
    
    return sum/num;
}

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 7;

    if (a == 5){
        printf("a is 5\n");
    }
    else if (a == 10) {
        printf("a is 10\n");
    }
    else {
        printf("a is neither 5 nor 10\n");
    }

    // this will execute all case statements because no break was used
    switch (a) {
        case 1:
            printf("a is 1\n");
            a++;
        case 2:
            printf("a is 2\n");
            a++;
        case 3:
            printf("a is 3\n");
            a++;
        default:
            printf("! (1 <= a <= 3)\n");
    }
    printf("%d\n", a);

    while (a <= 10) {
        printf("value of a: %d\n", a);
        a++;
    }

    int i, j;
    for (i = 2; i <= 100; i++) {
        for (j = 2; j <= (i/j); j++) {
            if (i % j == 0) { break; }
        }
        if (j > (i/j)) { printf("%d is prime\n", i); }
    }

    int table[2][3] = {     // This creates a "table" or matrix with 2 rows and 3 columns
        {0, 1, 2},
        {3, 4, 5},
    };
    int matrix[2][3] = {0, 1, 2, 3, 4, 5};
    double prices[5] = {0, 1, 2, 3, 4};
    int arr_size = sizeof prices / sizeof *prices;

    printf("size:\t%d\n", arr_size);

    int var = 10;
    int *pointer = NULL;

    pointer = &var;
    printf("Address of pointer: %p\n", pointer);
    printf("Value of pointer and var: %d, %d\n", *pointer, var);

    typedef struct Books {
        char title[50];
        char author[50];
        int book_id;
    } book;

    book moby_dick;
    strcpy(moby_dick.title, "Moby Dick");
    strcpy(moby_dick.author, "Herman Melville");
    moby_dick.book_id = 123456;

    printf("Book title:\t%s\n", moby_dick.title);
    printf("Book author:\t%s\n", moby_dick.author);
    printf("Book ID:\t%d\n", moby_dick.book_id);
    struct Books *book_pointer;
    book_pointer = &moby_dick;

    printf("Book title:\t%s\n", book_pointer->title);
    printf("Book author:\t%s\n", book_pointer->author);
    printf("Book ID:\t%d\n", book_pointer->book_id);

    int age;
    char name[20] = "Walter";

    printf("Please enter %s's age:\t", name);
    scanf("%d", &age);
    printf("%s is %d years old.\n", name, age);

    FILE *fp;
    fp = fopen("test.txt", "w");
    fprintf(fp, "This is written using fprintf %d\n", age);
    fputs("This is written using fputs \n", fp);

    fclose(fp);

    const int SIZE = 200;
    char buff[SIZE];
    fp = fopen("test.txt", "r");
    fgets(buff, SIZE, fp);
    printf("%s", buff);
    fgets(buff, SIZE, (FILE*)fp);
    printf("%s", buff);
    fclose(fp);

    fp = fopen("does_not_exist.txt", "r");

    if (fp == NULL) {
        fprintf(stderr, "Errno:\t%d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    }
    else {
        fclose(fp);
    }

    printf("factorial of 5:\t%d\n", factorial(5));
    printf("%lf\n", avg(3, 10, 15, 23));

    char str[100];
    char *descr;

    strcpy(str, "Peter Griffin");
    descr = malloc(200 * sizeof(char));

    if (descr == NULL) {
        fprintf(stderr, "Error allocating memory\n");
    }
    else {
        strcpy(descr, "Today is a very nice day");
    }
    printf("%s\n", str);
    printf("%s\n", descr);

    free(descr);

    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    return EXIT_SUCCESS;
}
