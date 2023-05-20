#include <stdio.h>
#include <string.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#ifdef	LINUX
#define	IFIDXA sockaddr_ll
#include <linux/if_packet.h>
#else
#define	IFIDXA sockaddr_dl
#include <sys/types.h>
#include <net/if_dl.h>
#endif

#include "../include.rta/argue.h"

void ifidxa(unsigned char *name, struct IFIDXA *readout)
{
   static struct ifaddrs	*list;

   struct ifaddrs		*p;
   int				 x;
   unsigned char		*q;

   struct IFIDXA		*asoc;


   if (flag['v'-'a']) putchar('.');

   if (list == NULL)
   {
      if (flag['v'-'a'])  putchar(':');
      x = getifaddrs(&list);

      if (x < 0) return;
      if (list == NULL) return;         
   }

   p = list;

   while (p)
   {
      if (flag['v'-'a']) printf("[%s]", p->ifa_name);
      x = strcmp(name, p->ifa_name);

      if (x == 0)
      {
	 if (flag['v'-'a'])
         {
            q = (unsigned char *) p->ifa_addr;
	    putchar('[');
	    x = sizeof(struct ifaddrs);
	    while (x--) printf("%2.2x", *q++);
	    printf("]\n");
         }

         asoc = (struct IFIDXA *) p->ifa_addr;

         #ifdef LINUX
	 if (asoc->sll_family == AF_PACKET)
         {
            *readout = *asoc;
            return;
         } 
         #else
         if (asoc->sdl_family == AF_LINK)
         {
            *readout = *asoc;
            return;
         }
         #endif
      }

      p = p->ifa_next;
   }
}

