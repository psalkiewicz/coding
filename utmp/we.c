#include<utmp.h>
#include<fcntl.h>

#define UTMP "/var/run/utmp"

main()
{
	struct utmp *rd;
	int a,f,g=sizeof(struct utmp);

	f=open(UTMP,O_RDONLY);
	
	a=read(f,rd,g);
	while (a==g)
	{
		if (rd->ut_type==7)
		{
			printf("%d %5s %20s %20s %20s %5d\n",
			rd->ut_type,
			rd->ut_user,
			rd->ut_host,
			rd->ut_line,
			rd->ut_id,
			rd->ut_pid);
		}
		a=read(f,rd,g);
	}
	close(f);
}
