CC=gcc
CFLAGS=-Wall

default: binclock

binclock.o:
	$(CC) $(CFLAGS) -c binclock.c

binclock: binclock.o
	$(CC) $(CFLAGS) -o binclock binclock.o
	$(RM) binclock.o

clean:
	$(RM) binclock *.o
