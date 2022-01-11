# The C Programming Language

Continue with p.45 Decision Making

## Setup

Any file with the `.c` extension is a C-file. C files have to be compiled to run. This can be done with `cc` or `gcc`.
```sh
cc -o output file.c
cc -o output file1.c file2.c    # to compile multiple files
```

## Data Types

All variable Names in C are case sensitive, which means that `hello`, `Hello`, `HELLO` and `hELlO` are all different variables.

### Integer Types

Type | Size | Value Range
--- | --- | ---
char | 1 byte | -128 to 127 or 0 to 255
unsigned char | 1 byte | 0 to 255
signed char | 1 byte | -128 to 127
int | 2 or 4 bytes | -32.768 to 32.767 or -2.147.483.648 to 2.147.483.647
unsigned int | 2 or 4 bytes | 0 to 65.535 or 0 to 4.294.967.295
short | 2 bytes | -32.768 to 32.767
unsigned short | 2 bytes | 0 to 65.535
long | 4 bytes | -2.147.483.648 to 2.147.483.647 
unsigned long | 4 bytes | 0 to 4.294.967.295)

Integers use the `%d` placeholder in a `printf` statement and expects a base 10 number.  
`%i` can also be used, but it autodetects the base of the value (e.g. is 012 is 12 in decimal, but 10 in octal).
```c
int a = 5;
printf("The value of a is %d\n", a);
```

### Floating Point Types

Type | Size | Value Range | Precision
--- | --- | --- | ---
float | 4 bytes | 1,2E-38 to 3,4E+38 | 6 decimal places
double | 8 bytes | 2,3E-308 to 1,7E+308 | 15 decimal places
long double | 10 bytes | 3,4E-4932 to 3,4E+4932 | 19 decimal places


Floats use the `%f` placeholder in a `printf` statement.
```c
float b = 2.5;
printf("value of b is %f\n", b);
```

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

Declaration: tells the compiler that tere is a variable but no memory gets allocated for it
Definition: memory gets allocated for the variable, can only be done once
Initialization: A variable gets a value assigned to it

A vairable also gets declared during its definition (if it has no been declared yet)
```c
int add(int, int);  // declares the function, no memory allocation

// this defines the function, memory gets allocated
int add(int a, int b) {
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
When using the `extern` keyword, a variable gets declared but not defined. The following code throws an error, since `var` is not known to the compiler.
```c
extern int var;    // this is only a declaration, no memory allocation

int main(void)
{
    var = 5;        // this throws an error
    int var = 5;    // this works
    printf("the value of var is: %d\n", var);
}
```

Variable can be declared with a `type variable_name` construct. A variable has to be declared once so a value can be assigned to it.


```c
int a, b, c;
char c, ch;
float f, temperature;
```

You also can also be initialized with a value in their declaration.

```c
int a = 5, b = 7;
float temperature = 20.0;
```

### Lvalues and Rvalues

- lvalues refer to a memory location and may appear on both sides of an assignment
- all variables are lvalues
- rvalues refer to a data value that is stored at some address in memory
- rvalues cannot have values assigned to them, so they can only appear on the right-hand side of an assignment

### Using Header files

```c
// header.h
int add_num(int);
extern int global_var;

// c1.c
#include "header.h"     // not strictly necessary here, as we define the function below

int global_var = 15;

int add_num(int num)
{
    return num + 5;
}

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

## Constants
Constants, also called literals cannot be altered during the execution.

### Integer Literals
Integer literals can be a `decimal`, `octal` or `hexdecimal` value and can be distinguished by their prefix (`0x` or `0X` for `hexdecimal`, `0` for `octal`, no prefix for `decimal`). In addition, a literal can also have the suffix `L` for `long` and `U` for `unsigned`. They can be uppercase or lowercase and in any order.  

Example | Meaning
--- | ---
`212l` | long decimal
`0273` | octal
`0xfeeL` | long hexdecimal
`30UL` | unsigned long decimal

### Floating Point Literals
A floating-point literal has an integer part, a fractional part and an exponent part (signed by `e` or `E`).

Example | Meaning
--- | ---
`3.14159` | floating point value
`314159E-5L` | using an exponent with of type long

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
/* general schema:
#define variable value;
const type variable = value;
*/
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

## Macros

Macros get defined with a `#` and get processed by the preprocessor at compile time.  
`#include <header.h>` imports headers from the standard folder  
`#include "header.h"` imports headers from the current directory  
`#undef` undefines a macro  
`#define` macros can take arguments, but have no type checking. In general it is better to use functions instead.
```c
#define MULTIPLY (a, b) a * b
MULTIPLY(2 + 3, 3 + 5)  // 2 + 3 * 3 + 5 -> 16
#define MULTIPLY (a, b) (a) * (b)
MULTIPLY(2 + 3, 3 + 5)  // (2 + 3) * (3 + 5) -> 40
```
The preprocessor also supports `#if` - `#endif` constructs

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

`register` is used to define local variables that should be stored in a register instead of RAM (a register stores the data the CPU is currently processing, and is even faster than RAM). Also `register` does not mean the variable will be stored in the register, that depends on the hardware.
```c
{
    register int counter;
}
```

`static` keeps a local variable in existence during the lifetime of the program instead of destroying and recreating it each time it comes into and goes out of scope. This way, they retain their value between function calls. Declaring `static` on global variables causes them to be resitricted to the file they are declared in.
```c
int increase_counter(void)
{
    static int counter = 5;  // counter keeps its modified value between function calls
    counter ++;
    return counter;
}
```

`extern` is used to give a reference that is visible to all program files. It tells the compiler that the variable of function exists, but is defined somewhere else. Also no memory gets allocated, as it is just a declaration.
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
`|` | OR
`^` | XOR
`~` | flips all bits
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
`|=` | `a |= 5` is the same as `a = a | 5`


### Misc Operators

Operator | Description
--- | ---
`sizeof()` | returns the size of a variable
`&` | returns the memory address of a variable
`*` | returns the pointer to a variable
`? : ` | ternary condition, `if true ? then x : otherwise y`

### Operator Precedence

Higher precede operators appear at the top of the table and get evaluated first within an expression.

Category | Operator
--- | ---
Postfix | () [] -> . ++ --
Unary | + - ! ~ ++ -- (type)* & sizeof
Multiplicative | * / %
Additive | + -
Shift | << >>
Relational | < <= > >=
Equality | == !=
Bitwise AND | &
Bitwise XOR | ^
Bitwise OR | \|
Logical AND | &&
Logical OR | \||
Conditional | ?:
Assignment | = += -= *= /= %= >>= <<= &= ^= \|=
Comma | ,
