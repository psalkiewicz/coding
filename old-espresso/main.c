#include<curses.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>

#include<niface.h>
#include<language.h>
#include<misc.h>
#include<config.h>
#include<sopts.h>
#include<remote.h>

WINDOW *stl,*mnw,*dst;
int color;

int main(void)
{
	char c[1024],*c2;
	int x,y;
	int a,l;
	int ob,obo,tot=0;
	struct k *st=NULL;
	struct k *ac=NULL;
	struct pos co;
	
	FILE *f1;
	
	signal(SIGINT,SIG_IGN);

	color=nc_open(&x,&y);
	nc_pairs();

	stl=newwin(1,x,0,0);
	mnw=newwin(y-2,x,1,0);
	dst=newwin(1,x,y-1,0);	
	intrflush(mnw,FALSE);
	keypad(mnw,TRUE);

	curs_set(0);
	wattron(stl,COLOR_PAIR(1));
	wattron(dst,COLOR_PAIR(1));
	for (a=0;a<x;a++) 
	{
		mvwprintw(stl,0,a," ");
		mvwprintw(dst,0,a," ");
	}
	a=0;
	mvwprintw(stl,0,1,STATLN1);
	mvwprintw(dst,0,1,STATLN2);
	wrefresh(stl);
	wrefresh(dst);
	
	if ((f1=fopen(CONFIG_DIR"computer.cfg","rt"))==NULL)
		nc_destroy(LANG003);

	fgets(c,1024,f1);
	l=sizeof(struct k);
	if ((ac=malloc(l))==NULL) nc_destroy(LANG004);
	st=ac;
	while (!feof(f1))
	{
	    c2=strtok(c,":");
	    if (c2!=NULL) {
		strncpy(ac->name,c2,20);
		c2=strtok(NULL,":");
		if (c2!=NULL) {
		    ac->ip=inet_addr(c2);
		    c2=strtok(NULL,":");
		    if (c2!=NULL) {
			ac->tariff=atoi(c2);
			if ((ac->n=malloc(l))==NULL) nc_destroy(LANG004);
			ac=ac->n;
			ac->name[0]=0;
			if (++tot>=(LICENSEX2/2)) break;
		    }
		}    
	    }
	    fgets(c,1024,f1);
	}	
	fclose(f1);
	if (!tot) nc_destroy(LANG007);
	ac=st;
	ob=1;
	while (ac->name[0])
	{
		 co=position(ob++,x/2);
		 mvwprintw(mnw,co.y,co.x,ac->name);
		 wattron(mnw,COLOR_PAIR(2));
		 mvwprintw(mnw,co.y+1,co.x,"     ");
		 wattroff(mnw,COLOR_PAIR(2));
		 ac=ac->n;
	}
	ob=1; obo=1;
	while(1)
	{

	        co=position(obo,x/2);
        	mvwprintw(mnw,co.y,co.x-2,"  ");

	        co=position(ob,x/2);
        	wattron(mnw,COLOR_PAIR(4)|A_BOLD);
        	mvwprintw(mnw,co.y,co.x-2,"->");
        	wattroff(mnw,COLOR_PAIR(4)|A_BOLD);
		switch (wgetch(mnw))
		{
			case 'q': quit_prompt(x); break;
			case W_RETURN:
			{
				ac=st;
				for (a=1;a<ob;a++) ac=ac->n;
				switch(c_menu(x/2))
				{
					case CM_POFF: rem_control(ac->ip,REM_POWEROFF); break;
					case CM_REBT: rem_control(ac->ip,REM_REBOOT); break;
				}
				break;
			}
			case W_PRAWO: if (ob<tot) obo=ob++; else {obo=ob; ob=1;} break;
			case W_LEWO:  if (ob>1) obo=ob--; else {obo=ob; ob=tot;} break;
			default:	break;
		}
	}
	nc_destroy("!! Code end");
	return 0;
}
