#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

unsigned long pagecount(char *fn)
{
	FILE *f;
	unsigned long t=0,d,multi=1;
	char c,i[10];

	if ((f=fopen(fn,"rb"))==NULL) return -1;

	c=fgetc(f);
	while (!feof(f))
	{
		if (c==27)
		{
			/* ESCAPE */

			c=fgetc(f);
			if ((c=='(') || (c==')'))
			{
				/* ESCAPE, NAWIAS */
				c=fgetc(f);
				if (c=='s')
				{
					c=fgetc(f);
					d=0;
					while (isdigit(c)) 
					{
						if (d<9) i[d++]=c; else break;
						c=fgetc(f);
					}
					i[d]=0; d=0;
					if (c=='W')
					{
						for (d=0;d<=atol(i);d++)
						/* while (c!=27) */ c=fgetc(f);
						continue;
					}
				}
			} else
			if (c=='&')
			{
				c=fgetc(f);
				if (c=='l')
				{
					c=fgetc(f);
					d=0;
                                        while (isdigit(c))
                                        {
                                                if (d<9) i[d++]=c; else break;
                                                c=fgetc(f);
                                        }
                                        i[d]=0; d=0;
					if (c=='X')
					{
						if (atol(i)>1)
						{
							multi=atol(i);
#ifdef LIBPCL_DEBUG
                        				printf("multi: %lX\n",multi);
#endif
						}
						continue;
					}
				}
			}
			if (c=='*')
			{
				/* ESCAPE, GWIAZDKA */
				c=fgetc(f);
				if (c=='b')
				{
					c=fgetc(f);
					d=0;
					while (isdigit(c)) 
					{
						if (d<9) i[d++]=c; else break;
						c=fgetc(f);
					}
					i[d]=0; d=0;
					if (c=='m')
					{
						c=fgetc(f);
						while(isdigit(c)) 
						{
							if (d<9) i[d++]=c; else break;
							c=fgetc(f);
						}
						i[d]=0; d=0;
					}
					if (c=='W')
					{
						for (d=0;d<=atol(i);d++)
						/* while (c!=27) */ c=fgetc(f);
						continue;
					}
				}
			}
		}

		if (c==12)
		{
#ifdef LIBPCL_DEBUG
			printf("marker: %lX\n",ftell(f)-1);
#endif
			 t++;
		}
		c=fgetc(f);
	}
	fclose(f);
	
	return t * multi;
}
