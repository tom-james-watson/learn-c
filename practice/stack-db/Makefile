CC=cc
CFLAGS=-Wall -g

stack: stack.o
	$(CC) $(CFLAGS) stack.o -o stack

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

clean:
	rm -f *.o stack
