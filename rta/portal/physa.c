#include <stdio.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

unsigned char * physa(unsigned char *name)
{
   static struct ifaddrs	*list;

   struct ifaddrs		*p;
   int				 x;
   unsigned char		*q;

   if (list == NULL)
   {
      x = getifaddrs(&list);

      if (x < 0) return NULL;
      if (list == NULL) return NULL;         
   }

   p = list;

   while (p)
   {
      x = strcmp(name, p->ifa_name);
      if (x == 0)
      {
         q = (unsigned char *) p->ifa_addr;
         if ((*q == 20) && (*(q + 1) == 18)) return (unsigned char *) (q + 11);
      }

      p = p->ifa_next;
   }

   return NULL;
}

