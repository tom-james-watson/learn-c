# learnc

I'm currently trying to do a C refresher after not having touched it since university. I'm going through Zed Shaw's Learn C The Hard Way.

## Notes

### Debugging

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
breakpoint set --name main
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
