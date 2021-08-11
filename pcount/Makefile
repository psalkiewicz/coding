CC=gcc
CFLAGS=-Wall -ansi -pedantic -I. -DKTORY="\"gerling\"" -DLIBPCL_DEBUG
LDOPTS=

all:	liby prog link

liby:	libpcl.c

prog:	example.c

link:	libpcl.o example.o
	$(CC) $(LDOPTS) libpcl.o example.o -o example 

clean:
	rm -f *.o example

