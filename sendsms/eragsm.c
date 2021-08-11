#include<stdio.h>

main()
{
	char msg1[1024]="bookopen=&numer=xxxxxxxxx&ksiazka=ksi%B1%BFka+telefoniczna&message=";
	char msg2[1024]="&podpis=&kontakt=&code=&Nadaj=Nadaj";

	char *text="hello!";
	char tel[9]="602506524";
	
	char *a;

	long len;

	/* Content-Length: 125 */

	fprintf(stdout,"POST /sms/sendsms.asp HTTP/1.1\n"
		       "Accept: */*\n"
		       "Referer: http://boa.eragsm.com.pl/sms/sendsms.asp\n"
		       "Accept-Language: pl\n"
		       "Content-Type: application/x-www-form-urlencoded\n"
		       "Accept-Encoding: gzip, deflate\n"
		       "User-Agent: Mozilla/4.0 (compatible; MSIE 4.01; Windows 98)\n"
		       "Host: boa.eragsm.com.pl\n"
		       "Connection: Keep-Alive\n");

	len=strlen(msg1)+strlen(msg2)+strlen(text);
	
	a=msg1+16;
	strncpy(a,tel,9);

	fprintf(stdout,"Content-Length: %d\n\n",len);
	fprintf(stdout,"%s",msg1);
	fprintf(stdout,"%s",text);
	fprintf(stdout,"%s\n",msg2);
}
