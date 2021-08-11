CC=gcc
CFLAGS=-pedantic -ansi -Wall
LDOPTS=

all:	mn lnk

mn:	main.c

lnk:	main.o
	$(CC) $(LDOPTS) main.o -o ircspam

clean:
	rm -f *.o ircspam

