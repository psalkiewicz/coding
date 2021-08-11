#define PRJ "DialerKill"
#define VER "1.0"

#include<stdio.h>
#include<windows.h>
#include<winreg.h>

int main()
{
		TCHAR c[MAX_PATH];
		TCHAR cv[MAX_PATH];
		DWORD i=MAX_PATH,iv=MAX_PATH,j=0;
		HKEY b;
		long a;
		
		a=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_ENUMERATE_SUB_KEYS,&b);

		if (a!= ERROR_SUCCESS)
		{
				/* Error opening registry key */
				return -1;
		}

		printf("%d \r\n",MAX_PATH);

		while (RegEnumValue(b,j++,c,&i,NULL,NULL,cv,&iv)==ERROR_SUCCESS)
		{
			/* Sprawdz czy wpis nie jest dialerem. Jesli tak:
				 - usun klucz z rejestru
			     - sprawdz czy program nie dziala
			     - jesli tak to usun z pamieci
			     - usun program z dysku
			*/

			printf("%s=%s\r\n",c,cv);
			i=MAX_PATH; iv=MAX_PATH;
		}

		RegCloseKey(b);	

		return 0;
}