#include<tcp_opts.h>
#include<irc_opts.h>
#include<hardcode.h>

void reply_process(int d,char *c)
{
	char p[2048];

	if (c[0]==':')
	{
		if (!strncmp(c+1,IRC_NICK,strlen(IRC_NICK)))
		{
			strncpy(p,c+strlen(IRC_NICK)+2,2048);
			if (!strncasecmp(p,"MODE",4))
			{
				tcp_send(d,"JOIN #dupa123\r\n");
			}	
		}
	}
	printf("%s\n",c);
}
