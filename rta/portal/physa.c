#include <stdio.h>
#include <string.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#ifdef	LINUX
#include <linux/if_packet.h>
#else
#include <sys/types.h>
#include <net/if_dl.h>
#endif

#include "../include.rta/argue.h"

unsigned char * physa(unsigned char *name)
{
   static struct ifaddrs	*list;

   struct ifaddrs		*p;
   int				 x;
   unsigned char		*q;

   #ifdef LINUX
   struct sockaddr_ll		*asoc;
   #else
   struct sockaddr_dl		*asoc;
   #endif

   if (flag['v'-'a']) putchar('.');

   if (list == NULL)
   {
      if (flag['v'-'a'])  putchar(':');
      x = getifaddrs(&list);

      if (x < 0) return NULL;
      if (list == NULL) return NULL;         
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

         asoc = (struct sockaddr_ll *) p->ifa_addr;

         #ifdef LINUX
	 if (asoc->sll_family == AF_PACKET) return asoc->sll_addr; 
         #else
         if (asoc->sdl_family == AF_LINK)
         {
             return (unsigned char *) asoc->sdl_data + asoc->sdl_nlen;
         }
         #endif
      }

      p = p->ifa_next;
   }

   return NULL;
}

