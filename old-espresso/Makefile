CC=gcc
CCOPTS=-Iinclude/ -ansi -pedantic -Wall -Wstrict-prototypes -ggdb -c
LDOPTS=-lncurses

all:
	@echo -e "Prosze wybrac jezyk / Please choose language"
	@echo
	@echo "		make pl"
	@echo "		make eng"
	@echo

pl:	pll do

eng:	engl do

pll:
	cp language/pl.h include/language.h

engl:
	cp language/eng.h include/language.h

do:	misc main espresso

clean:
	rm *.o espresso include/language.h

misc:		niface.c sopts.c remote.c
	$(CC) $(CCOPTS) niface.c
	$(CC) $(CCOPTS) sopts.c
	$(CC) $(CCOPTS) remote.c

main:		main.c
	$(CC) $(CCOPTS) main.c

espresso:	main.o niface.o sopts.o remote.o
	$(CC) $(LDOPTS) main.o sopts.o niface.o remote.o -o espresso
