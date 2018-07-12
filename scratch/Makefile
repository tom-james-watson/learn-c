CC=cc
CFLAGS=-Wall -g

scratch: scratch.o
	$(CC) $(CFLAGS) scratch.o -o scratch

scratch.o: scratch.c
	$(CC) $(CFLAGS) -c scratch.c

clean:
	rm -f *.o scratch
