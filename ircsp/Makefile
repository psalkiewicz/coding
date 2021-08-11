CC=gcc
CFLAGS=-Wall -ansi -pedantic -I.
LDOPTS=

all:	adds prog link

adds:	tcp_opts.c irc_opts.c

prog:	main.c

link:	tcp_opts.o irc_opts.o main.o
	$(CC) $(LDOPTS) tcp_opts.o irc_opts.o main.o -o ircspam 

clean:
	rm -f *.o ircspam

