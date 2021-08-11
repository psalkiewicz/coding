#include<tcp_opts.h>
#include<irc_opts.h>
#include<hardcode.h>

#include<stdio.h>

int awaria(char *p)
{
	perror(p);
	return -1;
}

int main(int argc, char **argv)
{
	int fd,a,c;
	char *p,*d;
	
	fd=tcp_open(IRC_SERVER,IRC_PORT,0);
	
	tcp_send(fd,"USER twitter +iw twitter : Twittah\r\n");
	tcp_send(fd,"NICK "IRC_NICK"\r\n");

	if ((d=(char *) malloc(c=1))==NULL)
		return awaria("malloc");

	while(1)
	{		
		if ((p=tcp_get(fd))!=NULL)
		{
			a=0;
			while (a<strlen(p))
			{
				d[c-1]=p[a++];
				if (d[c-1]=='\n')
				{
					if (d[c-2]=='\r') d[c-2]=0; else d[c-1]=0;
					reply_process(fd,d);
					d=realloc(d,c=1);
				} else d=realloc(d,++c);
			}
	
		}
	}	
}
