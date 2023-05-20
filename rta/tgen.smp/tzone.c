
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "../engine.smp/emulate.h"
#include "../engine.smp/smp.h"

#ifdef  LINUX
#ifndef	GCC
#include "../tgen.smp/_mnames.h"
#endif
#endif

extern smp		 core[];

void tzone(struct timeval xronos)
{
   struct tm		*timep;
   int			 cores = RTA_SMP;
   smp			*xcore = core;	/*	= &core[0]	*/

   timep = localtime(&xronos.tv_sec);

   if (timep) time_zone = timep->tm_gmtoff & 0x00FFFFFF;
   else       printf("no localtime pointer returned\n");
}

