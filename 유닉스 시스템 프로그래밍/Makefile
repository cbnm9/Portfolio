# Makefile

CC=gcc
CFLAGS=-g
OBJS= sgrep.o
LIBS=

all :	add

add:	$(OBJS)
	$(CC) $(CFLAGS) -o sgrep $(OBJS)

sgrep.o: sgrep.c
	$(CC) $(CFLAGS) -c sgrep.c

clean:
	rm -f $(OBJS) sgrep core
