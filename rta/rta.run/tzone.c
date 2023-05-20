
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "../include.rta/argue.h"
#include "../engine.rta/emulate.h"

#ifdef  LINUX
#ifndef	GCC
#include "../tgen.x64/_mnames.h"
#endif
#endif

extern int		 base[];

void tzone(struct timeval *xronos)
{
   struct tm		*timep;

   timep = localtime(&xronos->tv_sec);

   if (timep) time_zone = timep->tm_gmtoff & 0x00FFFFFF;
   else printf("no localtime pointer returned\n");
}

