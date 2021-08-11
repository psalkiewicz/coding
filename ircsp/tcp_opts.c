#include<tcp_opts.h>

#include<stdio.h>

int tcp_open(char *host,int port,int nb)
{
	struct hostent *he;
	struct sockaddr_in rem;
	int fd;

	if ((he=gethostbyname(host))==NULL) return -1;
	if ((fd=socket(AF_INET,SOCK_STREAM,0))<0) return -1;
	
	rem.sin_family=AF_INET;
	rem.sin_port=htons(port);
	rem.sin_addr=*((struct in_addr *)he->h_addr);
	memset(&(rem.sin_zero),0,8);

	if (connect(fd, (struct sockaddr *)&rem,sizeof(struct sockaddr))<0)
		return -1;
	
	if (nb) fcntl(fd,F_SETFL,O_NONBLOCK);
	
	return fd;
}		

int tcp_send(int fd,char *p)
{
	if (send(fd,p,strlen(p),0)!=strlen(p))
		return -1;
	else
		return 0;
}

char *tcp_get(int fd)
{
	int a;
	char c[RD_BUF],*p;

	if ((a=recv(fd,c,RD_BUF-1,0))<0) 
		return NULL;
	if ((p=(char *) malloc(a+1))==NULL) 
		return NULL;
	
	c[strlen(c)]=0;
	strncpy(p,c,a);

	return p;
}

