#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <remote.h>

#define MYPORT 3924    /* the port users will be sending to */
#define CODE1  0x66
#define CODE2  0x19

int rem_control(long ip,char action)
{
	char c[3]="\x66\x19\x01";
        int fd;
        struct sockaddr_in rem;

	c[2]=action;

        if ((fd = socket(AF_INET, SOCK_DGRAM, 0))==-1) return -1;

        rem.sin_family = AF_INET;      /* host byte order */
        rem.sin_port = htons(MYPORT);  /* short, network byte order */
        rem.sin_addr.s_addr=ip;
        memset(&(rem.sin_zero),0,8);     /* zero the rest of the struct */
        if (sendto(fd, c, 3, 0,(struct sockaddr *)&rem, sizeof(struct sockaddr))!=3) return -1;
        close(fd);
        return 0;
}
