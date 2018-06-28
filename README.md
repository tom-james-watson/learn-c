# learnc

I'm currently trying to do a C refresher after not having touched it since university. I'm going through Zed Shaw's Learn C The Hard Way.

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

`type *ptr` - A pointer of <type> named `ptr`

`*ptr` - The value of whater `ptr` is pointed at

`*(ptr + i)` - The value of (whatever `ptr` is pointed at plus `i`)

`&thing` - The address of thing

`type *ptr = &thing` - A pointer of <type> named `ptr` set to the address of `thing`

`ptr++` - Increment where `ptr` points
