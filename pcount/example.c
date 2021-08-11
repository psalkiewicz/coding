#include<stdio.h>
#include"libpcl.h"

int main(void)
{
	unsigned long a=0;

	a=pagecount("data/"KTORY".prn");

	printf("\nLiczba stron: %ld\n\n",a);

	return 0;
} 
