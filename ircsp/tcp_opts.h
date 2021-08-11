#ifndef __TCP_OPTS_H
#define __TCP_OPTS_H

#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#define RD_BUF		2048

int tcp_open(char *host,int port,int nb);
int tcp_send(int fd,char *p);
char *tcp_get(int fd);

#endif
