# Tutorial 0 - C Programming

Author: Christopher Wong<br>
Contact: pkcwongab@ust.hk<br>

---

## File System

C language code files involves two file types.
- ```.h``` contains structure and function declarations
- ```.c``` contains function implementations

## Basic Structure

A project written in C must implement the program entry function ```int main(void)```, which is usually found in ```main.c```.

```C

  /*
   * file: main.c
   */

  int main(void)  // program entry point
  {
    // code here
  }

```

## Variables

Variables are "boxes" that hold data.

Some primitive data types in C. These data types are native to C.
- ```char```
- ```int```
- ```float```
- ```double```

Also some common non-primitive data types we may encounter. Appropriate libraries must be included before usage.
- ```bool``` (available in ```<stdbool.h>```)
- ```size_t``` (available in ```<stdlib.h>```)

To declare a variable, the syntax is the data type identifier followed by its name.

```C

  int main()
  {
    char haha;
    int hoho;
    double hehe;
  }

```

To assign a value to a variable, use the ```=``` operator.

```C

  int main()
  {
    char haha;
    int hoho;
    double hehe;
    haha = 'a';
    hoho = 64;
    hehe = 3.1415926535897932;
  }

```

## Standard Input / Output

Standard input / output functions may be found in library ```<stdio.h>```.

### Console Output

Function ```void printf(const char*, ...)``` prints to console.

```C

  #include <stdio.h>

  int main()
  {
    printf("Hello World!"); // prints "Hello World!" on console
  }

```

To print the value of a variable, specify the data type token and provide the variable as additional argument.

```C

  int main()
  {
    int hoho = 123;
    double hehe = 0.618;
    printf("%d %f", hoho, hehe);  // prints "123 0.618"
  }

```

Data type tokens can be referred from the table below. Common usage in *italic*.

Token | Group | Applicable Data Types
--- | --- | ---
%d | Decimals | char, *int*
%f | Floating Points | *float*, *double*
%x | Hexadecimals | char, *int*
%c | Characters | *char*, int

### Console Input

Function ```void scanf(const char*, ...)``` stores an input from console to a variable. Specify the data type token and provide the address to the variable. Similar to ```printf```, but an ampersand(&) is required before the variable name.

> For further reading on address and memory location, see my other tutorial notes ```data-structure.md``` on C pointers.

```C

  int main()
  {
    int hoho;
    scanf("%d", &hoho);
  }

```

## Operators

An operator assigns or modifies a variable or value.
- ```=``` assignment
- ```+``` addition
- ```-``` subtraction
- ```*``` multiplication
- ```/``` division
- ```%``` modulo
- ```()``` parentheses

## Comparators

A comparator compares the value of two expressions, most of which are self explanatory.
- ```==``` equal to
- ```!=``` not equal to
- ```>``` greater than
- ```>=``` greater than or equal to
- ```<``` less than
- ```<=``` less than or equal to

## Statements

A statement is an expression which can be evaluated to either 1 or 0, or in other words true or false. The evaluated value of a statement can be stored by a ```bool```.

```C

  int main()
  {
    bool statement = (2 > 1); // true
  }

```

## Conditionals

Conditionals are flow controls which guide the execution of a program based on statement evaluations.

### If-Else

Self explanatory.

```C

  int main()
  {
    int a;

    scanf("%d", &a);
    if (a > 50)
    {
      printf("greater than 50.\n");
    }
    else
    {
      printf("not greater than 50.\n")
    }
  }

```

### Switch-Case

Switch-Case allows a variable or expression to be tested for equality against a list of values.

```C

  int main()
  {
    int a;
    scanf("%d", &a);
    switch (a % 2)
    {
      case 0:
        printf("even");
        break;
      default:
        printf("odd");
    }
  }

```

### Do-While

While loop blocks execute as long as the condition is met.

```C

  int main()
  {
    int i = 0;
    while (i != 10)
    {
      printf("Hello\n");  // prints 10 times
      i++;
    }
  }

```

Do-While loop blocks execute at least once, terminating when condition is no longer met.

```C

  int main()
  {
    int i = 0;
    do
    {
      printf("Hello\n");  // prints 10 times
      i++;
    }
    while(i != 10);
  }

```

### For-loops

Similar to while loops, but with its own counter.

```C

  int main()
  {
    for (int i = 0; i != 10; i++)
    {
      printf("Hello\n");  // prints 10 times
    }
  }

```

### Break-Continue

Break statements jumps out of a loop. The loop may be ```do-while``` or ```for```.

```C

  int main()
  {
    while (1)
    {
      // this part runs
      break;
      // this part is skipped
    }
    // break jumps here
  }

```

Continue statements skips through all succeeding statements within the loop.

```C

  int main()
  {
    while (1)
    {
      // continue jumps back here
      continue;
      // this part is skipped
    }
    // code here only executes when loop condition is false
  }

```

## Functions

Functions in C follow the standard system call API. Functions usually accept parameters, and return a result.

```
<type> <function name>(<parameters>);
```

An example function.

```C

  int addition(int a, int b)
  {
    return a + b;
  }

```

Functions may be called by other functions.

```C

  int addition(int a, int b)
  {
    return a + b;
  }

  int main()
  {
    printf("%d", addition(2, 3)); // outputs 5
  }

```

> A function may also be passed as a parameter. It shall be discussed in advanced tutorial

```C

  int addition(int a, int b)
  {
    return a + b;
  }

  int calculator(int a, int b, int (*operation)(int, int))
  {
    return operation(a, b);
  }

  int main()
  {
    printf("%d", calculator(2, 3, &addition));  // more advaced function calls
  }

```

## Scopes

All functions in C have a global scope. Any function can be used after it has been declared.

```C

  void func1()
  {
    printf("Hello!");
  }

  int main()
  {
    func1();  // calls func1
    func2();  // compilation error
  }

  void func2()
  {
    printf("This doesn't work!");
  }

```

All variables in C have their scope defined in a {} parentheses. A variable is recognized as it is referenced within its scope and after its declaration.

```C

  int x;

  void func()
  {
    int a;
  }

  int main()
  {
    int b;
    {
      int c;
      {
        int d;
      }
      // a, d and e are undefined
      // b, c and x are defined
    }
    int e;
  }

```

---

## Extra - Data Types

### Enumerations
Enum is a data type whose collection of values corresponds to explicitly named constants. Unless specified, the value of the first element is 0, while any other element has a value one greater than its previous.
```C
  enum color
  {
    RED,  // RED = 0
    GREEN,  //GREEN = 1
    BLUE  //BLUE = 2
  };
```
Enums are basically constants, but they make code more readable.
```C
  int main()
  {
    enum color myColor = RED;
    switch (myColor)
    {
      case RED:
      case GREEN:
      case BLUE:
    }
  }
```

### Pointers
Every variable is a memory location, and every memory location has its address defined.
```C
  int main()
  {
    int haha; // haha is a memory location
    haha = 123; // haha stores an integer
  }
```
In C, we may get the address of a memory location with the ampersand(&) operator.
```C
  int main()
  {
    int haha;
    printf("address of haha: %x\n", &x);  // a possible output may be bff5a3f6
  }
```
A pointer is a variable whose value is the address of another variable. Note that a pointer itself does not store any user data. It only stores an address to another variable. It does not hold any information to the actual data stored. The universal data type of a pointer is therefore ```void*```.
```C
  int main()
  {
    void *haha;  // declaration of a pointer
    haha = NULL;  // a good habit to initialize a pointer to NULL
  }
```
To retrieve the stored data to an address, the reverse operator is the asterisk(*). The operation is called dereferencing. Before dereferencing, it is neccessary to cast the pointer to the appropriate data type.
```C
  int main()
  {
    void* pointer = NULL;
    int haha = 123;
    pointer = &haha;  // pointer now stores address to haha
    printf("%d\n", *((int*)(pointer))); // cast the pointer to int* before dereferencing.
  }
```
The above code may be simplified.
```c
  int main()
  {
    int* pointer = NULL;
    int haha = 123;
    pointer = &haha;
    printf("%d\n", *pointer);
  }
```

### Arrays
An array stores a collection of data of the same data type.
```C
  int main()
  {
    int myArray[10];  // an int array of size 10
  }
```
For an array ```A``` of size ```n```, one may access the memory location by ```A[0]```, ```A[1]``` up to ```A[n-1]```. Note that ```A``` is the pointer to the starting address of the array, which is also the address to the first element of the array, ie ```A[0]```.
```C
  int main()
  {
    int A[10];
    printf("%x %x\n", A, &A[0]);  // equivalent expressions
    printf("%x %x\n", &A[3], A + sizeof(int) * 3);  // equivalent expressions
  }
```
> It is also possibe to declare arrays of dynamic sizes with the help of pointers and a memory allocator.
```C
  int main()
  {
    int n = 10;
    int* myArray = malloc(sizeof(int) * n);
    // some code here
    free(myArray);  // it is necessary to free up memory after program execution
  }
```
The array ```myArray``` may be accessed same as a static array.
