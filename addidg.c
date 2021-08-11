/*
 * Addidg v1.2
 * Add users to IDG project
 * (C) Warsaw 1998    Slawomir Krawczyk for Internet Services
 * To compile:
 *     gcc -o addidg addidg.c -lmysqlclient -L/usr/local/lib/mysql -lm
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <strings.h>
#include <bstring.h>
#include <ctype.h>
#include <unistd.h>
#include <syslog.h>
#include <pwd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <mysql/mysql.h>

#define DEFPASSWD "idg99"
#define sstrcpy(x,y) {char *_a=y; strcpy(x,_a?_a:"");}
#undef DEBUG
extern int errno;

FILE *f;
char user_name[10],domena[41],line[100];
char imie[41],nazwisko[41],full_name[81];
char *cp,*tmp;
int id=10000,exist=0,pid,users=0;
struct passwd *pw;
struct stat st;
MYSQL mysql,*sock;

/********************************/
/***    Mysql stuff           ***/
/********************************/

void kaboom(char *baza){

  if (!(sock = mysql_connect(&mysql,NULL,"","pierdola"))){
    fprintf(stderr,"Couldn't connect to mySQL engine!\n%s\n\n",
            mysql_error(&mysql));
    perror("");
    exit(1);
  }

  if(mysql_select_db(sock,baza) < 0){
    fprintf(stderr,"Could't select database idg\n",mysql_error(sock));
    exit(1);
  }
}

int chkdomain(char *name){

  char query[100];
  int i,rows;
  MYSQL_RES *res;
  MYSQL_ROW row;

  sprintf(query,"select ii from Instytucje where domena='%s'",name);

  if(mysql_query(sock,query) < 0 || !(res=mysql_store_result(sock))){
    fprintf(stderr,"Query failed (%s)\n",mysql_error(sock));
    exit(1);
  }

  rows=mysql_num_rows(res);
  if(rows==0) return(-1);
  row = mysql_fetch_row(res);
  mysql_free_result(res);
  return (atoi(row[0]));
}

int chklogin(char *name){

  char query[100];
  int i,rows;
  MYSQL_RES *res;
  MYSQL_ROW row;

  sprintf(query,"select imie,nazwisko from Osoby where id='%s'",name);

  if(mysql_query(sock,query) < 0 || !(res=mysql_store_result(sock))){
    fprintf(stderr,"Query failed (%s)\n",mysql_error(sock));
    exit(1);
  }

  rows=mysql_num_rows(res);
  if(rows==0) return(0);
  row = mysql_fetch_row(res);
  mysql_free_result(res);
  fprintf(stderr,"User %s (%s %s) exists in database\n",name,row[0],row[1]);
  return(-1);
}

int updatemysql(void){

  char query[500];
  int ii;
  MYSQL_RES *res;

  kaboom("idg");

  ii=chkdomain(domena);
  if(ii<0){
    fprintf(stderr,"\tNo such domain %s\n",domena);
    fprintf(stderr,"\tI drop adding user %s to database\n",user_name); 
    return;
  }
  sprintf(query,
      "insert into Osoby (id,imie,nazwisko,ii,ostatnio_czytal) values ('%s','%s','%s','%d',curdate())",
    user_name,imie,nazwisko,ii);

  if(mysql_query(sock,query) < 0){
    fprintf(stderr,"Query failed (%s)\n",mysql_error(sock));
    exit(1);
  }

  mysql_close(sock);
}

/********************************/
/*** Add user to IDG project  ***/
/***     Main function        ***/
/********************************/

int adduser(void){

  exist=0;
  strcpy(full_name,imie);
  strcat(full_name," ");
  strcat(full_name,nazwisko);

/* Check if user exists */

  if (getpwnam(user_name)) {
     fprintf(stderr, "\tUser %s exists\n", user_name);
     return;
  }else fprintf(stderr, "\tAdding user %s\n", user_name);

/* Find free id */

  setpwent();
  while((pw=(struct passwd *)getpwent())!=NULL)
    if((pw->pw_uid > id) && (pw->pw_uid < 20000)) id=pw->pw_uid;
  endpwent();
  id++;

/* Add entry to /etc/passwd */

  if (lckpwdf() == -1){
    fprintf(stderr,"Can lock passwd !!!\n");
    exit(-1);
  }
  if((f=fopen("/etc/passwd","a"))==NULL){
    fprintf(stderr,"Can not open file /etc/passwd\n");
    exit(-1);
  }
  fprintf(f,"%s:%s:%d:102:%s:/home/idg/%s/./%s:/bin/passwd\n",
           user_name,crypt(DEFPASSWD,"CC"),id,full_name,domena,user_name);
  fclose(f);
  ulckpwdf();

/* Create directory */

  tmp=(char *)calloc(100,1);
  sprintf(tmp,"/home/idg/%s",domena);
  if(stat(tmp,&st))
    if(errno==ENOENT){
      mkdir(tmp,00775);
      chown(tmp,0,102);
      chdir(tmp);
      mkdir("etc",00711);
      mkdir("bin",00711);
      chdir("bin");
      link("/home/ftp/bin/ls","ls");
      chdir(tmp);
      chdir("etc");
      link("/etc/group.idg","group");
      if((f=fopen("passwd","a+"))==NULL){
        fprintf(stderr,"Can not open file passwd\n");
        exit(-1);
      }
      fprintf(f,"root:*:0:102:Administrator:/:/bin/passwd\n");
      fclose(f);

    }else{
      perror("stat error");
      exit(-1);
    }
  chdir(tmp);
  mkdir(user_name,00755);
  chown(user_name,id,102);

/* Update passwd in chrooted ftpd */

  chdir("etc");
  if((f=fopen("passwd","a+"))==NULL){
    fprintf(stderr,"Can not open file passwd\n");
    exit(-1);
  }
  fprintf(f,"%s:*:%d:102:%s:/%s:/bin/passwd\n",
          user_name,id,full_name,user_name);
  fclose(f);

/* Update /etc/named/named.szkoly */

  if((f=fopen("/etc/named/named.szkoly","r+"))==NULL){
    fprintf(stderr,"Can not open file /etc/named/named.szkoly\n");
    exit(-1);
  }
  for(;;){
    fgets(line,99,f);
    if(feof(f)) break;
    if(strncmp((char *)line,";begin",6)==0) break;
  }
  for(;;){
    fgets(line,99,f);
    if(feof(f)) break;
    tmp=(char *)strtok(line," \n\t");
    if(strcmp((char *)tmp,(char *)domena)==0) exist=1;
  }
  if(!exist){
    fseek(f,0,SEEK_END);
    fprintf(f,"%-30.30sIN\tA\t195.116.165.3\n",domena);
  }
  fclose(f);

/* Add entry in /etc/virt.szkoly */

  if((f=fopen("/etc/virt.szkoly","a"))==NULL){
    fprintf(stderr,"Can not open file /etc/virt.szkoly\n");
    exit(-1);
  }
  sprintf(tmp,"%s.%s@%s.szkoly.edu.pl",imie,nazwisko,domena);
  fprintf(f,"%-60.60s %s@mail.szkoly.edu.pl\n",tmp,user_name);
  sprintf(tmp,"%s@%s.szkoly.edu.pl",user_name,domena);
  fprintf(f,"%-60.60s %s@mail.szkoly.edu.pl\n",tmp,user_name);
  fclose(f);

/* Add entry in /etc/genericstable */

  if((f=fopen("/etc/genericstable","a"))==NULL){
    fprintf(stderr,"Can not open file /etc/genericstable\n");
    exit(-1);
  }
  sprintf(tmp,"%s.%s@%s.szkoly.edu.pl",imie,nazwisko,domena);
  fprintf(f,"%s\t%s\n",user_name,tmp);
  fclose(f);

/* Add entry is /etc/sendmail.cw */

  if((f=fopen("/etc/sendmail.cw","a"))==NULL){
    fprintf(stderr,"Can not open file /etc/sendmail.cw\n");
    exit(-1);
  }
  if(!exist) fprintf(f,"%s.szkoly.edu.pl\n",domena);
  fclose(f);

/* Update sendmail databases */

  chdir("/etc");
  rename("virtusertable","virtusertable.old");
  system("/bin/cat /etc/virt.* > virtusertable");
  system("/usr/sbin/makemap hash virtusertable < virtusertable");
  system("/usr/sbin/makemap hash genericstable < genericstable");

/* Subscribe to forum */

  if((f=fopen("/usr/local/petidomo/lists/forum/list","a"))==NULL){
    fprintf(stderr,
            "Can not open file /usr/local/petidomo/lists/forum/list\n");
    exit(-1);
  }
  fprintf(f,"%s@szkoly.edu.pl\n",user_name);
  fclose(f);

/* Add httpd alias */

  if(!exist){
   if((f=fopen("/usr/local/apache/conf/szkoly.alias","a"))==NULL){
    fprintf(stderr,"Can not open file /usr/local/apache/conf/szkoly.alias\n");
    exit(-1);
   }
   fprintf(f,"Alias /%s /home/idg/%s/\n",domena,domena);
   fclose(f);
  }
  
  if(!exist){
   if((f=fopen("/usr/local/apache/conf/szkoly.conf","a"))==NULL){
    fprintf(stderr,"Can not open file /usr/local/apache/conf/szkoly.szkoly\n");
    exit(-1);
   }
   fprintf(f,"<VirtualHost 195.116.165.3>\n");
   fprintf(f,"ServerName %s.szkoly.edu.pl\n",domena);
   fprintf(f,"ServerAdmin Webak@is.com.pl\n");
   fprintf(f,"DocumentRoot /home/idg/%s\n",domena);
   fprintf(f,"</VirtualHost>\n\n");
   fclose(f);
  }

/* Update mysql database */

  updatemysql();

  users++;

}

/********************************/
/***   Interactive interface  ***/
/********************************/

void addinteractive(void){

  printf ("\tLogin name :");
  if (fgets(user_name, 10, stdin) != user_name) return;
  if (!(cp = strchr(user_name, '\n'))) return;
  *cp = '\0';
  printf ("\tImie       :");
  if (fgets(imie, 40, stdin) != imie) return;
  if (!(cp = strchr(imie, '\n'))) return;
  *cp = '\0';
  printf ("\tNazwisko   :");
  if (fgets(nazwisko, 40, stdin) != nazwisko) return;
  if (!(cp = strchr(nazwisko, '\n'))) return;
  *cp = '\0';
  printf ("\tDomena     :");
  if (fgets(domena, 40, stdin) != domena) return;
  if (!(cp = strchr(domena, '\n'))) return;
  *cp = '\0';

  adduser();
  system("/usr/sbin/pwconv");
}

/********************************/
/***      MySQL interface     ***/
/********************************/

void addmysql(void){

  int i;
  char query[100];
  MYSQL_RES *res;
  MYSQL_ROW row;

  kaboom("idg");

  sprintf(query,"select id,imie,nazwisko,domena from Instytucje,Osoby where Osoby.ii=Instytucje.ii");
  if(mysql_query(sock,query) < 0 || !(res=mysql_store_result(sock))){
    fprintf(stderr,"Query failed (%s)\n",mysql_error(sock));
    exit(1);
  }

  for(i=0;i<mysql_num_rows(res);i++){
    row = mysql_fetch_row(res);
    strcpy((char *)user_name,(char *)row[0]);
    strcpy((char *)imie,(char *)row[1]);
    strcpy((char *)nazwisko,(char *)row[2]);
    strcpy((char *)domena,(char *)row[3]);
    adduser();
  }
  mysql_free_result(res);
  mysql_close(sock);
  system("/usr/sbin/pwconv");

}

/********************************/
/*** File database interface  ***/
/********************************/

void addfiledb(char *path){

  FILE *db;
  
  if((db=fopen(path,"r"))==NULL){
    fprintf(stderr,"Can not open file %s\n",path);
    exit(-1);
  }  
  for(;;){
    fgets(line,199,db);
    if(feof(db)) break;
    sstrcpy((char *)user_name,(char *)strtok(line,";\n\r"));
    sstrcpy((char *)imie,(char *)strtok(NULL,";\n\r"));
    sstrcpy((char *)nazwisko,(char *)strtok(NULL,";\n\r"));
    sstrcpy((char *)domena,(char *)strtok(NULL,";\n\r"));
    adduser();
  }
  fclose(db);
  system("/usr/sbin/pwconv");
}

void usage(char *prog){

  fprintf(stderr,"\nUsage : %s [-m] [-f filename]\n",prog);
  fprintf(stderr,"\twithout parameters interactively adding single user\n");
  fprintf(stderr,"\t-h this help\n");
  fprintf(stderr,"\t-m adding new users checking MySQl database\n");
  fprintf(stderr,"\t-f adding new users using database file in format:\n");
  fprintf(stderr,"\t\t login;imie;nazwisko;domena\n");
  fprintf(stderr,"\t\t ATTEN: can't be null fields\n");
  exit(-1);
}

void final(void){

/* Reload named */

  if((f=fopen("/var/run/named.pid","r"))==NULL){
    fprintf(stderr,"Can not open file /var/run/named.pid\n");
    exit(-1);
  }
  fgets(line,99,f);fclose(f);
  pid=atoi(line);
  kill(pid,1);

/* Reload sendmail */

  if((f=fopen("/var/run/sendmail.pid","r"))==NULL){
    fprintf(stderr,"Can not open file /var/run/sendmail.pid\n");
    exit(-1);
  }
  fgets(line,99,f);fclose(f);
  pid=atoi(line);
  kill(pid,1);

/* Reload httpd */

  if((f=fopen("/usr/local/apache/logs/httpd.pid","r"))==NULL){
    fprintf(stderr,"Can not open file /usr/local/apache/logs/httpd.pid\n");
    exit(-1);
  }
  fgets(line,99,f);fclose(f);
  pid=atoi(line);
  kill(pid,1);

}


int main(int argc,char **argv){

  int i;
  char *dbname;

  printf ("\n Addidg v1.2 by Night Session\n ");
  printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

  openlog("addidg", LOG_PID, LOG_AUTH);

  if(argc==1) addinteractive();
  while ((i = getopt(argc, argv, "hf:m")) != EOF)
    switch (i) {
      case 'm': addmysql();
                break;
      case 'f': dbname=optarg;
                addfiledb(dbname);
                break;
      case 'h':
      default:  usage(argv[0]);
    }
  if (argc - optind > 0) usage(argv[0]);

  final();

  fprintf(stderr,"Done.\n");
  fprintf(stderr,"Added %d new users\n",users);
  exit(0);

}
