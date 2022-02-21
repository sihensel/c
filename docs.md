# The C Programming Language

This document servers as a reference for me.

## Setup

Any file with the `.c` extension is a C-file and can be compiled with `cc` or `gcc`.
```sh
cc -o output file.c
cc -o output file1.c file2.c    # to compile multiple files
```

## Data Types

All variable Names in C are case sensitive, which means that `hello`, `Hello`, `HELLO` and `hELlO` are all different variables.

### Integer Types

Type | Size
--- | ---
char | 1 byte
unsigned char | 1 byte
signed char | 1 byte
int | 2 or 4 bytes
unsigned int | 2 or 4 bytes
short | 2 bytes
unsigned short | 2 bytes
long | 4 bytes
unsigned long | 4 bytes

Integers use the following placeholders in format strings:


### Floating Point Types

Type | Size | Precision
--- | --- | ---
float | 4 bytes | 6 decimal places
double | 8 bytes | 15 decimal places
long double | 10 bytes | 19 decimal places

Floats use the `%f` or `%lf` (for doubles) placeholder in a `printf` statement.

### void Types

Void indicates that no value is available. This can be used in three cases.
- A function returns as void if it does not return any value
- Function arguments can be void when it does not take any input
- A pointer can point to void when it points to the address but not the its type

In conclusion, there are five basic data types:

Type | Description
--- | ---
char | Typically a single octet (one byte)
int | The most natural size of integer for the machine
float | Single-precision floating point value
double | Double-precision floating point value
void | Represents the absence of a type

## Variable declaration and definition

There are three stages in variable and function declaration.

Name | Description
--- | ---
Declaration | Tells the compiler that tere is a variable but no memory gets allocated for it
Definition | Memory gets allocated for the variable, can only be done once
Initialization | A variable gets a value assigned to it

A vairable also gets declared during its definition (if it has no been declared yet)

```c
int add(int, int);  // declares the function, no memory allocation

// this defines the function, memory gets allocated
int add(int a, int b)
{
    return (a + b);
}
```

The following example works, since `var` is implicitely defined (and thus declared) globally.

```c
int var;    // this is a definition and allocates memory

int main(void)
{
    var = 5;
    printf("the value of var is: %d\n", var);
}
```

When using the `extern` keyword, a variable gets declared but not defined and no memory gets allocated.

```c
extern int var;    // this is only a declaration, no memory allocation
```

Variables can also be assigned values during declaration.

```c
int a = 3, b, c;        // only a is 3
char c, ch;
float f, temperature = 23.2;
```

### Lvalues and Rvalues

- Lvalues refer to a memory location and may appear on both sides of an assignment (all variables are Lvalues)
- Rvalues refer to a data value that is stored at some address in memory
- Rvalues cannot have values assigned to them, so they can only appear on the right-hand side of an assignment

## Specifiers

Specifier | Description
--- | ---
`%c` | character (char)
`%s` | string
`%i` | Integer, auto-detects the base of the number
`%d` | Decimal integer
`%u` | Unsigned decimal integer
`%lu` | Unsigned int or unsigned long
`%o` | Octal integer
`%x` or `%X` | Hexadecimal integer
`%f` | float
`%lf` | double
`%e` or `%E` | floating point number in scientific notation
`%p` | address (or pointer)
`%06d` | specifies the width (456 becomes 000456)
`%.2f` | float with two decimals (rounded)
`%n` | prints nothing

## Constants

Constants, also called literals cannot be altered during execution.

### Integer Literals
Integer literals can be a `decimal`, `octal` or `hexdecimal` value and can be distinguished by their prefix (`0x` or `0X` for `hexdecimal`, `0` for `octal`, no prefix for `decimal`). In addition, a literal can also have the suffix `L` for `long` and `U` for `unsigned`. They can be uppercase or lowercase and in any order.  

Example | Description
--- | ---
`212l` | long decimal
`0273` | octal
`0xfeeL` | long hexdecimal
`30UL` | unsigned long decimal

### Floating Point Literals
A floating-point literal has an integer part, a fractional part and an exponent part (signed by `e` or `E`).

Example | Description
--- | ---
`3.14159` | floating point value
`314159E-5L` | using an exponent of type long

### Character Literals
Character literals are enclosed in single quotes. These can be a plain character (`'x'`), an escape sequence (`'\n'`) or a universal character (`'\u02C0'`).

### String Literals
String literals are enclosed in double quotes and can contain plain characters, escape sequence or universal characters. Strings can be broken up by string literal and whitespaces.
These are all the same string:

```c
"Hello, World"
"Hello, \
World"
"Hello" " World"
```

Constants can be defined using the `#define` preprocessor or the `const` keyword. Literals are written in uppercase out of convention, not necessity.
`const` allows type checking, while `#define` can be used to save space and compile time, as it gets processed by the preprocessor.

```c
#include <stdio.h>

#define LENGTH 8
#define WIDTH 10

int main(void)
{
    int area = LENGTH * WIDTH;

    const int HEIGHT = 5;
    int volume = area * HEIGHT;

    printf("The area has a size of %d\n", area);
    printf("The volume has a size of %d\n", volume);
    return 0;
}
```

## Storage Classes

A storage class defines the scope and lifetime of variables or functions and precede the type that they modify. There are four storage classes.

- `auto`
- `register`
- `static`
- `extern`

`auto` is the default for all local variables.

```c
{
    auto int month;
}
```

`register` is used to define local variables that should be stored in a register instead of RAM (a register stores the data the CPU is currently processing, and is even faster than RAM). It is up to the hardware to decide if it respects registers.

```c
{
    register int counter;
}
```

`static` keeps a local variable in existence during the lifetime of the program instead of destroying and recreating it each time it comes into and goes out of scope. This way, they retain their value between function calls. Declaring `static` on global variables or functions causes them to be resitricted to the file they are declared in.

```c
int increase_counter(void)
{
    static int counter = 5;  // counter keeps its modified value between function calls
    counter ++;
    return counter;
}
```

`extern` is used to give a reference that is visible to all program files. It tells the compiler that the variable or function exists, but is defined somewhere else. Also no memory gets allocated, as it is just a declaration.

```c
extern int count;
extern int func(int);
```

## Operators

### Binary Operators
Binary operators operate on the binary bits of a number, e.g.

```ini
A = 110011
B = 101101

A & B  = 100001
A | B  = 111111
A ^ B  = 011110
~A     = 001100
A << 2 = 11001100
A >> 3 = 000110
```

Operator | Description
--- | ---
`&` | AND
`\|` | OR
`^` | XOR
`~` | flip all bits
`<<` | shift all bits to the left
`>>` | shift all bits to the right

### Assignment Operators

Operator | Description
--- | ---
`=` | simple assignment
`+=` | `a += 5` is the same as `a = a + 5`
`-=` | `a -= 5` is the same as `a = a - 5`
`*=` | `a *= 5` is the same as `a = a * 5`
`/=` | `a /= 5` is the same as `a = a / 5`
`%=` | `a %= 5` is the same as `a = a % 5`
`<<=` | `a <<= 2` is the same as `a = a << 2`
`>>=` | `a >>= 5` is the same as `a = a >> 5`
`&=` | `a &= 5` is the same as `a = a & 5`
`^=` | `a ^= 5` is the same as `a = a ^ 5`
`\|=` | `a \|= 5` is the same as `a = a \| 5`

### Misc Operators

Operator | Description
--- | ---
`sizeof` | returns the size of a variable
`&` | returns the memory address of a variable
`*` | returns the pointer to a variable
`? : ` | ternary condition, `if true ? then x : otherwise y`
`++` | increase the value by one
`--` | decrease the value by one

```c
int a = 10, x;
x = a++;    // x = 10, as a gets incremented AFTER the expression
x = ++a     // x = 11, as a gets incremented BEFORE the expression
```
## Decision Making

In C, any `if` statement that evaluates to a _non-null_ or _non-zero_ value is considered true.

```c
int a = 5;
if (a == 5) {
    printf("a is 5\n");
}
else if (a == 10) {
    printf("a is 10\n");
}
else {
    printf("a is neither 5 nor 10\n");
}
```

Of course, if statements can be nested.
An alternative to `if`-statements is a `ternary` statement.


```c
int a = 5, b = 7, c;

(a == 5) ? printf("a is 5\n") : printf("a is not 5\n");
c = (a == 5) ? a : b;
```

Another way to test variables against a set of values is using the `switch` statement. Each value is called a `case`.
A `break` terminates the `switch` statement. If not `break` statement appears, the next case statement will be evaluated.A `default` case can be added which will be executed when all other cases fail (or no break is used).

```c
int a = 3;

switch (a) {
    case 1:
        printf("a is 1\n");
        break;
    case 2:
        printf("a is 2\n");
        break;
    case 3:
        printf("a is 2\n");
        break;
    default:
        printf("a is neither 1, 2 or 3\n");
}
```

## Loops

```c
int a = 5;

while (a <= 10) {
    printf("value of a: %d\n", a);
    a++;
}

for (int i = 0; i < 5; i++) {
    printf("current index: %d\n", i);
}

// the init step is not necessary, just make sure to write the semicolon
int i = 0;
for (; i < 5; i += 2) {
    printf("current index: %d\n", i);
}

int b = 10
do {
    printf("value of b: %d\n", b);
    b += 3;
} while (b < 20);

// of course, loops can be nested
int i, j;
for (i = 2; i <= 100; i++) {
    for (j = 2; j <= (i/j); j++) {
        if (i % j == 0) { break; }
    }
    if (j > (i/j)) { printf("%d is prime\n", i); }
}

// this creates an infinite loop
for (;;) {
    // code
}
```

The flow of a loop can be influenced by control statements

Control statement | Description
--- | ---
`break` | terminates the current loop and continue with the subsequent code
`continue` | skip the remainder of the loop body and start the next iteration
`goto` | continue at the labeled statement (bad practice, don't use)

## Arrays

Arrays are a collection of variables of the same type. All arrays use contiguous memory addresses, with the lowest address being the first element and the highest being the last.  
A single-dimensional array gets defined with an integer constant that indicates the size of the array.

```c
double temperatures[10];
int grades[5] = {3, 1, 5, 2, 3};
```

Multidimensional Arrays get defined by specifying multiple sizes.

```c
int table[2][3];    // This creates a "table" or matrix with 2 rows and 3 columns

int table[2][3] = {
    {0, 1, 2},
    {3, 4, 5},
};
// Is the same as
int table[2][3] = {0, 1, 2, 3, 4, 5};
```

C does not provide and direct builtin way to determine the size (or length) of an array. Instead, there are two simple ways to get the length.

```c
// Method 1: save the size upfront
#define ARR_SIZE 5;
double prices[ARR_SIZE];

// Method 2: using sizeof
// Since all elements of an array are of the same type,
// we can use the size of the whole array and divide it by th size of the first element
double prices[5] = {0, 1, 2, 3, 4};
int arr_size = sizeof prices / sizeof prices[0];
int arr_size = sizeof prices / sizeof *prices;    // or use the pointer
```

Arrays can be passed to functions in three different ways:
```c
void func(int arr[10]) { }
void func(int arr[]) { }
void func(int *arr) { }
```

By default, a function cannot return an array. But it can return a pointer instead.

```c
int *func(void)
{
    static int arr[5] = {0, 1, 2, 3, 4};      // static, since we return a local variable
    return arr;
}

int main(void)
{
    int *prices = func();
}
```

A pointer of an array always points to the _first_ element.

```c
double prices[10];
double *p;
p = prices;     // Here, p holds the value of the first element of prices

// also, these are the same
prices[4]
*(prices + 4)

// this means, once we store the first element of prices in p, we can access the other elements using
(*p + 3)
```

## Pointers

In C you can get the memory address of a variable by prefixing it with the ampersand `&`.

```c
int var = 10;
printf("Memory address of var: %p\n", &var);
```

A pointer can be specified with an asterisk `*`. It holds the memory address of another variable. Use the pointer itself to gain the memory address and use `*pointer` to get the value at the location of that address.  
It is good practice to assign a `NULL` value to a pointer at the time of variable declaration.

```c
int var = 10;
int *pointer = NULL;

pointer = &var;
printf("Address of pointer: %p\n", pointer);
printf("Value of pointer and var: %d, %d\n", *pointer, var);
```

You can check if a pointer is not NULL with `if (pointer)`.  
There are four arithmetic operators that can be used on pointers: `+`, `-` , `++`, `--`.  
`++` and `--` move the pointer to the next or previous memory address respectively.
A pointer can also point to another pointer.
Pointers can be passed to functions which allows changing values outside of the function.
A function can also return a pointer, which can point to local variables.

## Strings

Strings are one-dimensional arrays of characters terminated by a null character `\0`. A string should be treated as read-only.
String support the following (and more) functions, which can be included with `#include <string.h>`.

Function | Action
--- | ---
`strlen(s1)` | returns the length of string s1
`strcpy(s1, s2)` | copies s2 into s1
`strcat(s1, s2)` | concatenates s2 onto the end of s1
`strcmp(s1, s2)` | returns 0 is s1 == s2, less than 0 if s1<s2 and greater than 0 if s1>s2
`strchr(s1, ch)` | returns a pointer to the first occurence of ch in s1
`strstr(s1, s2)` | returns a pointer to the first occurence of s2 in s1

## Structures

Structures allow to store several data items of different kinds, whereas arrays allow only one data type.

```c
struct Books {          // 'Books' is the struct tag
    char title[50];
    char author[50];
    int book_id;
} book;                 // struct variable

int main(void) {
    struct Books b1;    // use struct keyword to define additional variables
    strcpy(book.title, "Moby Dick");
    strcpy(book.author, "Herman Melville");
    book.book_id = 123456;

    printf("Book title:\t%s\n", book.title);
    printf("Book author:\t%s\n", book.author);
    printf("Book ID:\t%d\n", book.book_id);

    return 0;
}
```

Of course, there can also be pointers of structures. To access struct elements via a pointer, use `->`.

```c
struct Books {
    char title[50];
    char author[50];
    int book_id;
} book;

struct Books *book_pointer;
book_pointer = &book;

printf("Book title:\t%s\n", book_pointer->title);
printf("Book author:\t%s\n", book_pointer->author);
printf("Book ID:\t%d\n", book_pointer->book_id);
```

Structs allow the packing of data via bit fields. This only works if the integer word length is less than or equal to the minimum machine word length. This is especially useful when struct elements are used as booleans.

```c
struct package {
    unsigned int f1:1;     // bit field of length 1
    unsigned int f2:4;     // bit field of length 4
} pack;
```

## Enums

Enumerated types, also called `enums`, consist of integral constants. Notice how a comma is used to seperate entries, whereas a `struct` uses semicolons. The first value of an `enum` is 0, then 1 and so on. Default values can be declared, which causes the following constant to be that value + 1 and so on.  
Enum variables can only take one value, which makes it useful for flags.  
They can also be used in `switch` or `for` statements.

```c
enum flags {
    CONST1,         // default value 0
    CONST2 = 9,     // value 9
    CONST3          // default value 10
};

int flag = CONST3;  // flag = 10
```

It can be practical to give enums values of the power of 2, e.g. to replicate Linux file permissions. This allows using multiple constants with the bitwise OR `|` operator for one value, as their binary values don't conflict.

```c
enum filemodes {
    READ = 4,       // 0100
    WRITE = 2,      // 0010
    EXECUTE = 1     // 0001
} mode;

int usr_perm = READ | EXECUTE;  // usr_perm = 5
```

## Union

Unions allow to store different data types in the same memory location. A union can have many members, but only one member can contain a value at any given time. This allows the efficient reuse of memory addresses. Unions have the size of its largest member.

```c
union Data {
    int i;
    char str[20];   // this union will have a size of 20 bytes
} data;
```

Union members can be accessed the same way as structs with `.` and `->` for pointers.

```c
union Data {
    int i;
    char str[20];
} data;

data.i = 10;
strcpy(data.str, "Hello World");

printf("data.i\t%d\n", data.i);     // this will contain a trash value
printf("data.str\t%s\n", data.str);
```

## Typedef

Typedef can be used to give a type a new name. Uppercase letters are used for typdefs by convention. This is especially useful with structs and unions.

```c
typedef unsigned int INT;

typedef struct Person {     // struct tag
    char fname[20];
    char lname[20];
} person;                   // new type

INT var = 5;
person p1;
strcpy(p1.fname, "Peter");
strcpy(p1.lname, "Griffin");

person p2 = {"John", "Doe"};    // declaration
```

## Input and Output

The `int getchar(void)` function reads the next available character from the screen and returns it as an integer.
The `int putchar(int)` function puts the passed character on the screen and also returns it.

```c
int c;
printf("Enter a value: ");
c = getchar();
printf("\nYou entered: ");
putchar(c);
```

The `char *fgets(char *string, int length, FILE *stream)` function reads a line from STDIN into the buffer pointed to by `string` until LF or EOF occur. This is the safer alternative to `gets()` as it stops reading once the buffer size is reached. Use subsequent calls to `fgets()` to read the remaining input.  
The `int puts(const char *s)` function writes the string `s` and a newline to STDOUT, provided the string is null-terminated.

```c
char str[10];
printf("Enter a value: ");
fgets(str, 10, stdin);
printf("\nYou entered: ");
puts(str);
```

`scanf(const char *format)` reads input from STDIN and scans the input according to the provided format.
`printf(const char *format)` printf writes to STDOUT according to the format provided.

```c
int age;
char name[20];
scanf("%s %d", name, &age);
printf("%s is %d years old.\n", name, age);
```

## File I/O

In C, files can be opened or created with the `FILE *fopen(const char *filename, const char *mode)` function.
`filename` is a string literal and the access mode can have one of the following values.

Mode | Description
--- | ---
`r` | reading
`w` | writing from the beginning of the file, creates the file if it does not exists
`a` | append mode, creates the file if it does not exist
`r+` | both reading and writing
`w+` | both reading and writing, truncates the file to zero length first
`a+` | read from the beginning and write in append mode

To handle binary files, attach the `b` suffix to any of the above modes, e.g. `rb` or `w+b` or `wb+`.

To close a file, use the `int fclose(FILE *fp)` function. It returns zero on success or EOF if there is an error.

To write to a file, there is the `int fputc(int c, FILE *fp)` function, which writes one character to the stream and returns the written character or EOF on error. Alternatively, there is the `int fputs(const char *s, FILE *fp)` function that writes a string to a file and returns any non-negative value on success or EOF. At last, there is the `int fprintf(FILE *fp, const char *format, ...)` function.

```c
FILE *file;
file = fopen("test.txt", "w");
fprintf(file, "This is written using fprintf %d\n", age);
fputs("This is written using fputs \n", file);
fclose(file);
```

You can read from a file with the `fgetc(FILE *fp)` function, which reads and returns a single character.
`fgets(char *buf, int n, FILE *fp)` reads upt to n-1 characters from the input stream and appends a null character to terminate the string. It stops once it encounters a newline character. This time, instead of STDIN, we use the filestream as the input.
`fscanf(FILE *fp, const char *format)` works the same, but stops at the first space character.

```c
char buff[200];
fp = fopen("test.txt", "r");
fgets(buff, 200, fp);
printf("%s", buff);
fclose(fp);
```

There are also binary I/O functions, that are not covered here.

## Preprocessors

Preprocessors are not part of the compiler, but a seperate step in the compilation process. It performs text substitution and instructs the compiler to do preprocessing. All preprocessor commands begin with the hash symbol `#`.

Directive | Description
--- | ---
`#define` | Substitutes a preprocessor macro
`#include` | Inserts a header from another file
`#undef` | Undefines a macro
`#ifdef` | Returns true if a macro is defined
`#ifndef` | Returns true if a macro is not defined
`#if` | Tests if a compile time condition is true
`#else` | The alternative for `#if`
`#elif` | Elseif
`#endif` | Ends the preprocessor conditional
`#error` | Prints an error message to STDERR
`#pragma` | Issues special commands to the compiler

```c
#define MAX_ARRAY_LENGTH 20     // replace instances of MAX_ARRAY_LENGTH with 20, used as a constant
#include <stdio.h>              // include a header file

#undef FILE_SIZE                // undefines FILE_SIZE
#define FILE_SIZE 500           // and sets it to 500

#ifndef MESSAGE
    #define MESSAGE "Hello World"   // define MESSAGE if it is not defined
#endif

#ifdef DEBUG
    // some debugging statements, useful when passing the -DDEBUG flag to gcc
#endif
```

There are some reserved macros in C.

Macro | Description
--- | ---
`__DATE__` | Current Date as a charachter literal in "MMM DD YYYY"
`__TIME__` | Current time in "HH:MM:SS"
`__FILE__` | Current file name as a string literal
`__LINE__` | Current line as a decimal constant
`__STDC__` | Defined as 1 when using the ANSI standard

```c
#define MULTIPLY (a, b) a * b
MULTIPLY(2 + 3, 3 + 5)  // 2 + 3 * 3 + 5 -> 16

// versus
#define MULTIPLY (a, b) (a) * (b)
MULTIPLY(2 + 3, 3 + 5)  // (2 + 3) * (3 + 5) -> 40
```

There are special preprocessor operators that can help create macros.

Use `\` when a macro would be too long for one line.
`#` within a macro converts a parameter to a string.

```c
#define MESSAGE(a, b) \
    printf("My name is " #a " " #b".\n")

int main(void)
{
    MESSAGE(Peter, Griffin);
    return 0;
}
```

The `defined` operator is used to determine if an identifier is defined with `#define`.

```c
#if !defined (MESSAGE)
    #define MESSAGE "Hello World"
#endif
```

## Header files

Header files have the `.h` extension and contain all constants, macros, global variables and function prototypes for a program. This header file is then included where needed, which is equal to copying the code into your `.c` file.
If a header file is included twice, the compiler will also process its content twice, leading to errors. To prevent this, it is good practice to wrap the entire content of the header file in a conditional.

```c
#ifndef HEADER_FILE
#define HEADER_FILE

// header file code

#endif
```

### Using Header files

```c
// header.h
int add_num(int);
extern int global_var;
```

```
// c1.c
#include "header.h"     // not strictly necessary here, as we define the function below

int global_var = 15;

int add_num(int num)
{
    return num + 5;
}
```

```
// main.c
#include <stdio.h>
#include "header.h"     // include the header as we use the function below

int main(void)
{
    int x = 5;
    int y = add_num(x);     // function usage without defining it
    printf("%d is now %d\n", x, y);
    printf("The value of 'global_var' is %d\n", global_var);
    return 0;
}
```

## Type casting

Variables types can be converted to different types, either implicitely of explicitely.

```c
int sum = 20, count = 4;
double avg;

avg = (double) sum / count;     // here, sum gets converted BEFORE the expression
```

## Error handling

C in itself does not provide erro handling, but makes use of return values.
The `perror()` function displays the string that is passed to it, and the textual representation of the `errno` value.
`strerror()` returns a pointer to the textual representation of the `errno`.

```c
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

extern int errno;

int main(void)
{
    FILE *fp;
    fp = fopen("does_not_exist.txt", "r");

    if (fp == NULL) {
        fprintf(stderr, "Errno:\t%d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    }
    else {
        fclose(fp);
    }
    return EXIT_SUCCESS;    // from stdlib.h
}
```

## Recursion

```c
int factorial(unsigned int i)
{
    if (i <= 1) {
        return 1;
    }
    else {
        return i * factorial(i - 1);
    }
}
```

## Variable arguments

Sometimes a function needs to accept a non-deterministic amount of parameters instead of a set amount. This is done with ellipses and the `stdarg.h` header.

```c
#include <stdarg.h>

double avg(int num, ...)    // the first parameter is always an int and representats the number of parameters
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
```

## Memory Management

C allows for dynamic memory management with function defined in `stdlib.h`.

Function | Description
--- | ---
`void *calloc(int num, int size)` | Allocates an array of `num` elements with `size` bytes
`void free(void *address)` | Release a memory block specified by `address`
`void *malloc(int num)` | Allocates an array of `num` bytes and leaves it initialized
`void *realloc(void *address, int newsize)` | Reallocates memory, extending it to `newsize`

```c
#include <stdio.h>
#include <stdlib.h>

char name[100];
char *descr;

strcpy(name, "Peter Griffin");
descr = malloc(200 * sizeof(char));

if (descr == NULL) {
    fprintf(stderr, "Error allocating memory\n");
}
else {
    strcpy(descr, "Today is a very nice day");
}
descr = realloc(descr, 400 * sizeof(char));
printf("%s\n", descr);
free(descr);
```

## Command Line Arguments

Command line arguments can be passed to a C program and are handled with the main function. `argv[0]` always holds the program name. So if no argument will be passed, `argc == 1`. Arguments are seperated by a space, arguments that contain a space have to be wrapped in quotes.

```c
int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
}
```
