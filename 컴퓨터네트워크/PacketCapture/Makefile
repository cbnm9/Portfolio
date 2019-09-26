# Makefile

CC=gcc
CFLAGS=
OBJS= main.o common_func.o dns.o ftp.o http.o telnet.o
LIBS=
all :	add

add:	$(OBJS)
	$(CC) $(CFLAGS) -o pk $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
common_func.o: common_func.c
	$(CC) $(CFLAGS) -c common_func.c
dns.o: dns.c
	$(CC) $(CFLAGS) -c dns.c
ftp.o: ftp.c
	$(CC) $(CFLAGS) -c ftp.c
http.o: http.c
	$(CC) $(CFLAGS) -c http.c
telnet.o: telnet.c
	$(CC) $(CFLAGS) -c telnet.c

clean:
	rm -f $(OBJS) pk core
