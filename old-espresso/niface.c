#include<curses.h>
#include<language.h>
#include<niface.h>

extern WINDOW *stl,*mnw,*dst;

int nc_open(int *x,int *y)
{
	initscr();
	cbreak();
	noecho();
	nonl();
	start_color();
	getmaxyx(stdscr,*y,*x);
	if (((*y)<25)||((*x)<80)) nc_destroy(LANG002);
	if (has_colors()==TRUE) return 1;
	return 0;
}

void nc_pairs()
{
	init_pair(1,COLOR_WHITE,COLOR_RED);
	init_pair(2,COLOR_WHITE,COLOR_GREEN);
	init_pair(3,COLOR_WHITE,COLOR_RED);
	init_pair(4,COLOR_YELLOW,COLOR_BLACK);
	init_pair(5,COLOR_CYAN,COLOR_BLUE);
}

void nc_destroy(char *p)
{
        wclear(stl); wclear(mnw); wclear(dst);
	wrefresh(stl); wrefresh(stl); wrefresh(stl);
	endwin();
	printf("\n\n%s (%s)\n\n",LANG001,p);
	exit(0);
}
