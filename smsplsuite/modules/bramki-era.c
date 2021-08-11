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
    char c[1024],*c2=PLUS_TPL,c3[2048],*c4,c5[4096];

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
    
    c4=c3;
    while (c2[++i]!='%');
    strncpy(c4,c2,i);
    c4+=i; c2+=(i+1);
    sprintf(c4,"%d%s%s\n\x00",a,c2,c);
    if (send(fd,c3,strlen(c3),0)<0) return -1;
    if ((a=recv(fd,c5,4096,0))<0) return -1;
    
    f1=creat("dupa",0600);
    write(f1,c3,strlen(c3));
    close(f1);
//    if (strstr(c5,"ana na numer")!=NULL) return 16; else return 8;
    printf(c5);
    close(fd);
}

main()
{
    printf("%d\n\n",plusgsm_cgisendsms("48601365756","dupadupa","t@t.pl"));
}
