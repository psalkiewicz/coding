/*
	pass.js		password ripper
*/

#include<stdio.h>
#include<math.h>

#define VAL 4030

char base[62] =    {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
		    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
		    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
		    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
		    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
		    'y', 'z'};

int f[62] = {   23,   535,  1047,   1559,   2071,   2583,  3095,  3607,  4119,   4631,
                12,    21,    26,     38,     53,     72,   101,   139,   294,    375,
               584,   841,  1164,   1678,   2425,   4989,  6478, 10076, 14494,  21785,
	     30621, 69677, 87452, 139356, 201113, 278810,    80,    83,    93,     99,
               113,   131,   159,    194,    346,    416,   619,   861,  1165,   1649,
              2256,  4766,  6077,   9554,  13713,  20576, 28894, 65661, 82386, 131248,
            164801, 262524};


int main(int argc, char **argv)
{
	unsigned long int  z=23;
	unsigned long int  y=28;
	unsigned long int v=0;
	unsigned long int x=0;

	char pass[1024];
	char n[32];

	int code = 0;

/*	int f[62];

	for (x=0; x<10; x++){
		f[x]=x<<9;
		f[x]+=23;
	} // dobrze 

	for (x=10; x<36; x++){
		y=y<<1;
		v=sqrt(y);
		sprintf(n,"%d",v);
		v=strtol(n, (char **)NULL, 16);
		v+=5;
		f[x]=v;
		y++;
	}

	for (x=36; x<62; x++){
		z=z<<1;
		v=sqrt(z);
		sprintf(n,"%d",v);
                v=strtol(n, (char **)NULL, 16);
		v+=74;
		f[x]=v;
		z++;
	}*/

		
	fgets(pass,1024,stdin);
	while(!feof(stdin))
	{
		code=0;
		pass[strlen(pass)-1]=0;		

		for (y=0;y<strlen(pass); y++)
		{
			for (x=0;x<62;x++)
			{
				if (pass[y]==base[x])
					code+=(y+1)*f[x];
			}
		}
	        if (code==VAL) printf("%s: %d\n",pass,code);

		fgets(pass,1024,stdin);
	}
}

