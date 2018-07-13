# learnc

I'm currently trying to do a C refresher after not having touched it since university. Mostly I'm just going through Zed Shaw's Learn C The Hard Way.

# Notes

## Debugging

Ensure that you compile with:

```
CFLAGS=-Wall -g
```

Start the debugger (`lldb` on mac, `gdb` on linux):

```
lldb <c program>
```

If you have a run-tine error, execution will just stop at the error, so you don't need to do anything here. If you want to just investigate some weirdness, you'll need to set a breakpoint:

```
breakpoint set --name foo_func // break at function
breakpoint set --file foo.c --line 12 // break at line of file
breakpoint set -f foo.c -l 12 // break at line of file shorthand
```

Need to do some more investigation on how to set other kinds of breakpoints.

Start execution of the program:

```
run
```

List more code at current program step:

```
l
```

Step to next line of code:

```
n
```

Step into function call:

```
s
```

Debug variables:

```
p <variable name>
```

## Pointers

### What's the point of pointers?

C is a call-by-value language. When you call a function in C, the value of any parameters are literally copied into the function's call stack. Pass an int, 4-bytes are copied into the function. Pass a char and 1-byte is copied into the function. What happens when you want to pass a 100k element int array into a function? Instead of copying 400,000 bytes, you simply pass a pointer to the array (8 bytes for 64 bit) instead. Same principle goes for large structs - good practice is to pass in a reference to the struct instead of the struct itself.

### Pointer syntax

`type *ptr` - A pointer of <type> named `ptr`

`*ptr` - The value of whatever `ptr` is pointed at

`*(ptr + i)` - The value of (whatever `ptr` is pointed at plus `i`)

`&thing` - The address of thing

`type *ptr = &thing` - A pointer of <type> named `ptr` set to the address of `thing`

`ptr++` - Increment where `ptr` points

### Examples

#### Dereferencing

`*var` is used to dereference and pointer and can be used to get and to set a value.

```
// declare int ival and int pointer iptr.  Assign address of ival to iptr.
int ival = 1;
int *iptr = &ival;

// dereference iptr to get value pointed to, ival, which is 1
int get = *iptr;
printf("*iptr = %d\n", get); // *iptr = 1

// dereference iptr to set value pointed to, changes ival to 2
*iptr = 2;
printf("*iptr = %d\n", set); // *iptr = 2
printf("ival = %d\n", ival); // ival = 2
```

#### Can't mix pointer types

A pointer stores both the address of another variable, and what the TYPE is of the variable at that address. It needs to know the type so that it knows how many bytes the variable takes up. This means the below will throw a `assignment from incompatible pointer type`.

```
// declare an int value and an int pointer
int ival = 1;
int *iptr = &ival;

// declare a float value and a float pointer
float fval = 1.0f;
float *fptr = &fval;

// declare a char value and a char pointer
char cval = 'a';
char *cptr = &cval;

iptr = &fval;
fptr = &ival;
iptr = &cval;
```

#### Pointers to arrays

Just like you have a pointer to an int or float, you can have a pointer to an array as long as the pointer is the same type as the elements of the array.

```
int myarray[4] = {1,2,3,0};
int *ptr = myarray;
```

When an array is created, int myarray[4] = {1,2,3,0};, what actually happens is the compiler allocates memory for the entire array and then assigns a pointer to the array variable, in this case myarray, holding the address of the first element in the array.

Pointers and arrays are NOT interchangable. You can assign an array variable to a pointer of the same type but not the opposite. When an array is created, the array variable cannot be reassigned.

```
int myarray[4] = {1,2,3,0};

// you can do this, myarray is a valid int pointer pointing to the first element of myarray
int *ptr = myarray;
printf("*ptr=%d\n", *ptr); // *ptr=1

// you cannot do this, array variables cannot be reassigned
// myarray = ptr
// myarray = myarray2
// myarray = &myarray2[0]
```

You can use pointers to loop through arrays.

```
int ages[] = { 23, 43, 12, 89, 2 };
char *names[] = {
  "Alan", "Frank", "Mary", "John", "Lisa"
};

// safely get the size of ages
int count = sizeof(ages) / sizeof(int);
int i;

// first way of indexing
for (i = 0; i < count; i++) {
  printf("%s has %d years alive.\n", names[i], ages[i]);
}

// set up the pointers to the start of the arrays
int *cur_age = ages;
char **cur_name = names;

// second way of indexing, using pointers
for (i = 0; i < count; i++) {
  printf("%s is %d years old.\n", *(cur_name + i), *(cur_age + i));
}

// third way, pointers are just arrays
for (i = 0; i < count; i++) {
  printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
}

// fourth way with incrementing pointers and pointer arithmetic for
// comparison
for (
    cur_name = names, cur_age = ages;
    (cur_age - ages) < count;
    cur_age++, cur_name++
    ) {
  printf("%s lived %d years so far.\n", *cur_name, *cur_age);
}
```

#### Pointers to structs

Like an array, a pointer to a struct holds the memory address of the first element in the struct. Here is some example code for declaring and using a struct pointer.

```
struct person {
  int age;
  char *name;
};
struct person first;
struct person *ptr;

first.age = 21;
first.name = "full name";
ptr = &first;

printf("age=%d, name=%s\n", first.age, ptr->name); // age=21, name=full name
```

You access values of a struct via `strct.field`. The -> operator is used to access a value from a struct pointer. This is equivalent to doing `(*ptr).field`, but as accessing a value from a struct pointer is so common, it has its own syntax.

#### Pointers to pointers

```
int val = 1;
int *ptr = 0;

// declare a variable ptr2ptr which holds the value-at-address of
// an *int type which in holds the value-at-address of an int type
int **ptr2ptr = 0;
ptr = &val;
ptr2ptr = &ptr;

printf("&ptr=%p, &val=%p\n", (void *)&ptr, (void *)&val);
// &ptr=0x7fff5a33a790, &val=0x7fff5a33a79c

printf(
  "ptr2ptr=%p, *ptr2ptr=%p, **ptr2ptr=%d\n",
  ptr2ptr,
  *ptr2ptr,
  **ptr2ptr
);
// ptr2ptr=0x7fff5a33a790, *ptr2ptr=0x7fff5a33a79c, **ptr2ptr=1
```

#### Pointers to functions

Pointers can be created to a function and used like so:

```
#include <stdio.h>

// Define a function that we can make a pointer to
int add(int n, int m) {
  return n+m;
}

int main(int argc, char *argv[]) {
  // Create a pointer to a function with the same prototype as add
  int (*funcPtr)(int,int) = add;
  printf("%d\n", (*funcPtr)(6, 3)); // 9
}
```

You can also pass function pointers as arguments to other functions:

```
#include <stdio.h>

int add(int x, int y) {
    return x + y;
}

int subtract(int x, int y) {
    return x - y;
}

// Another function that accepts a pointer to a function with the same
// prototype as add/subtract
int calculate(int (*op)(int,int), int x, int y) {
  return op(x, y);
}

int main(int argc, char *argv[]) {
  printf("%d\n", calculate(add, 6, 3)); // 9
  printf("%d\n", calculate(subtract, 6, 3)); // 3
}
```

`typedef`s can be used to make dealing with function pointers much nicer. Here's the same code as above but using a `typedef`:

```
#include <stdio.h>

typedef int (*operation)(int, int);

int add(int x, int y) {
    return x + y;
}

int subtract(int x, int y) {
    return x - y;
}

// Another function that accepts a pointer to a function with the same
// prototype as add/subtract
int calculate(operation op, int x, int y) {
  return op(x, y);
}

int main(int argc, char *argv[]) {
  printf("%d\n", calculate(add, 6, 3)); // 9
  printf("%d\n", calculate(subtract, 6, 3)); // 3
}
```


## Heap vs Stack

Both are chunks of RAM. Heap is easier to explain. It's just all the remaining memory in your computer, and you access it with the function `malloc` to get more. Each time you call `malloc`, the OS uses internal functions to register that piece of memory to you, and then returns a pointer to it. When you are done with it, you use `free` to return the block of memory to the OS. Failing to do so will cause a memory leak.

The stack is a special region of memory that stores temporary variables, which each function creates as locals to that function. Each argument to a function is _pushed_ onto the stack and then used inside the function. It's truly a stack data structure - LIFO. The same happens for variables locally defined within a function. When the function exits, the C compiler simply pops these variables off of the stack to clean up. Using the stack prevents memory leaks.

*If you didn't get it from `malloc`, or a function that calls `malloc`, then it's on the stack.*

Problems to watch out for:
* If you get a block of memory from `malloc` and have that pointer on the stack, then when the function exits the pointer will get popped off the stack and lost.
* If you put too much data on the stack (like large structs and arrays), then you can cause a _stack overflow_. Use `malloc` and the heap in this case.
* If you take a pointer to something on the stack, and then pass or return it from your function, then the function receiving it will _segmentation fault_ (segfault), because the actual data will get popped off and disappear.

Typically each thread gets a stack, whereas a whole application will share one heap.

Stack is faster. Allocation or deallocation is very simple, just moving a pointer. Heap (de)allocation is much more complex. Bytes in the stack tend to be reused very often and as such are usually mapped to processor cache, making it even faster.
