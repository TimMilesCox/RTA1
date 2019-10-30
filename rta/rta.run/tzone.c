
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "../engine.rta/emulate.h"

#ifdef  LINUX
#include "../tgen.x64/_mnames.h"
#endif

extern int		 base[];
extern struct timeval	 xronos;

void tzone()
{
   struct tm	*timep;

   timep = localtime(&xronos.tv_sec);

   if (timep) time_zone = timep->tm_gmtoff & 0x00FFFFFF;
   else       printf("no localtime pointer returned\n");
}

