#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<string.h>
#include<netinet/in.h>

#include"bramki-pl.h"

int plusgsm_cgisendsms(char *no,char *txt,char *mai)
{
    int f1,fd,a,b,i=-1;
    struct sockaddr_in rm;
    struct hostent *he;
    char c[1024],*c2=PLUS_TPL,c3[4096],*c4,c5[4096];

    if ((strlen(no)!=11)||(strlen(txt)>160)||(strlen(mai)>30)) return 1;
    if ((b=strlen(c2))>1000) return 1;
    
    if ((fd=socket(AF_INET,SOCK_STREAM,0))<0) return -1;
    if ((he=gethostbyname(PLUS_WWW))==NULL) return -1;
    
    rm.sin_family=AF_INET;
    rm.sin_port=htons(WWWPORT);
    rm.sin_addr= *((struct in_addr *)he->h_addr);
    memset(&(rm.sin_zero),0,8);
    
    if (connect(fd,(struct sockaddr *)&rm,sizeof(struct sockaddr))<0) return -1;
    
    a=sprintf(c,"numer=%%2B%s&mail=%s&tekst=%s&dzien=dzisiaj&godz=00&min=00",no,mai,txt);
    sprintf(c5,"Content-length: %d\r\n\r\n",a);
    if (send(fd,c2,strlen(c2),0)<0) return -1;
    if (send(fd,c5,strlen(c5),0)<0) return -1;
    if (send(fd,c,strlen(c),0)<0) return -1;
    if ((a=recv(fd,c3,4096,0))<0) return -1;
    
//    f1=creat("dupa",0600);
//    write(f1,c2,strlen(c2));
//    write(f1,c5,strlen(c5));
//    write(f1,c,strlen(c));
//    close(f1);
    if (strstr(c5,"ana na numer")!=NULL) return 16; else return 8;
//    printf(c3);
    close(fd);
}

main()
{
    printf("%d\n\n",plusgsm_cgisendsms("48601365756","dupadupa","t@t.pl"));
}
