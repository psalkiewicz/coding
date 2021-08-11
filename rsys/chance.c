#include<stdio.h>

#define PLACEON		20
#define TS		37
#define INITIAL		80
#define SPINS		20


int pl[PLACEON];

int isintable(int b, int m)
{
	int i;
	
	for (i=0;i<m;i++)
		if (pl[i]==b) return 1;
	
	return 0;
}

main()
{
	int i,a;
	int s = 0;
	signed int cash = INITIAL;
	signed long int re;

	/* Init */
	srand(time(NULL));
	/* End */

	/* Place */	
	while ((cash >= PLACEON) && (s<SPINS))
	{
		s++;

		cash -= PLACEON;		

		printf("Placed ones: ");
		for (i=0;i<PLACEON;i++)
		{
			a=rand() % TS;
			while (isintable(a,i)) a=rand() % TS;
			pl[i] = a;
			printf("%d ",a);
		}
		printf("\n");
	
		a = rand() % TS;

		printf("Winning one: %d\n",a);
	
		if (isintable(a,PLACEON))
			re = TS - 1;
		else
			re = 0;

		printf("Reward: %d\n",re-PLACEON);

		cash+=re;

		printf("Cash: %d\n",cash);
	}
}
