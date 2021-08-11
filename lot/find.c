#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define RESFILE "result.dat"

struct res {
	int l[20];
	char d[10];
	struct res *n;
};

int main(void)
{
	FILE *f;
	int *b,i;
	unsigned long int ci[81];
	char c[1024],*c2,*c3;
	struct res *r,*a;

	if ((f=fopen(RESFILE,"rt"))==NULL) { perror("fopen("RESFILE")"); return -1; }

	fgets(c,1024,f);
	r=(struct res *) malloc(sizeof(struct res));
	a=r;	
	while (!feof(f))
	{
		c[strlen(c)-1]=0;
		strncpy(a->d,c,10);
		c2=c+11;
		
		c3=strtok(c2,",");
		b=a->l;
		while(c3!=NULL)
		{
			*(b++)=atoi(c3);
			c3=strtok(NULL,",");
		}

		a->n=(struct res *) malloc(sizeof(struct res));
		a=a->n;	
		fgets(c,1024,f);
	}
	fclose(f);

	a=r;
	memset(ci,0,81*sizeof(unsigned long int));
	do
	{
		for (i=0;i<20;i++) ci[a->l[i]]++;
		a=a->n;
	} while (a->n!=NULL);

	return 0;
}


