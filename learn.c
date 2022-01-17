// continue with chapter 8

// this is a comment
/*
   this is a block comment
   compile a program with (g)cc -o <NAME> file.c
*/

#include <stdio.h>

int main(void)
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
    return 0;
}
