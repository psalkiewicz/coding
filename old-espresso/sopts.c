#include<curses.h>
#define SOPTS
#include<language.h>
#include<misc.h>
#include<niface.h>
#include<sopts.h>

extern WINDOW *dst,*stl,*mnw;
extern color;

int c_menu(int ogr)
{
	int a,p=0;
	int ac=1;
	
	while (p!=27)
	{
		for (a=0;cm_str[a][0];a++)
		{
			if ((a+1)==ac)
				wattron(mnw,COLOR_PAIR(5));
			mvwprintw(mnw,3+a,ogr+3,cm_str[a]);
			wattroff(mnw,COLOR_PAIR(5));
		}
		switch(p=wgetch(mnw))
		{
			case 'q':      quit_prompt(ogr*2); break;
			case W_DOL:    if (ac<a) ac++; else ac=1; break;
			case W_GORA:   if (ac>1) ac--; else ac=a; break;
			case W_RETURN: clean_c_menu(ogr); return ac; break;
		}
	}
	clean_c_menu(ogr);
	return 0;
}	

void clean_c_menu(int ogr)
{
     int a,b;
     for (a=0;cm_str[a][0];a++)
	for (b=0;b<strlen(cm_str[a]);b++)
        	mvwprintw(mnw,3+a,ogr+3+b," ");
}
void quit_prompt(int ogr)
{
      int a;

      wattron(stl,COLOR_PAIR(1));
      for (a=0;a<ogr;a++) mvwprintw(stl,0,a," ");
      mvwprintw(stl,0,1,LANG005);
      wrefresh(stl);
      curs_set(1);
      if (wgetch(stl)==KEY_YES) nc_destroy(LANG006);
      curs_set(0);
      for (a=0;a<ogr;a++) mvwprintw(stl,0,a," ");
      mvwprintw(stl,0,1,STATLN1);
      wattroff(stl,COLOR_PAIR(1));
      wrefresh(stl);
}

struct pos position(int d,int ax)
{
        int r=1,c=1,b;
        struct pos t;

        for (b=1;b<d;b++)
        {
                c++;
                if (((10*c)-6)>ax) { r++; c=1; }
        }
        t.x=(10*c)-7;
        t.y=4*r-1;
        return t;
}

