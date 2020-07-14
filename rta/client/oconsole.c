#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "../include.rta/address.h"

#undef	TRACE
#define	DATA	480
#define	LF	10

#ifdef	LINUX
static struct sockaddr_in here = { AF_INET, PORT(18)	} ;
static struct sockaddr_in from = { AF_INET		} ;
#endif

#ifdef	OSX
static struct sockaddr_in here = { 16, AF_INET, PORT(18)} ;
static struct sockaddr_in from = { 16, AF_INET		} ;
#endif

static socklen_t sixteen = 16;

int main()
{
   unsigned char	 data[DATA];
   unsigned char	*q;

   int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   int x = (s < 0) ? 0 : bind(s, (struct sockaddr *) &here, 16);
   int y;

   #ifdef TRACE
   q = (unsigned char *) &here;
   y =*q;
   printf("[%2.2lx][", sizeof(struct sockaddr_in));
   while (y--) printf("%2.2x", *q++);
   printf("]\n");
   #endif

   if      (s < 0) printf("no socket %d\n", errno);
   else if (x < 0) printf("bind failed %d\n", errno);
   else
   {
      printf("RTA1 output broadcast console\n");

      for (;;)
      {
         x = recvfrom(s, data, DATA, 0, (struct sockaddr *) &from, &sixteen);
         if (x < 0) break;

         q = (unsigned char *) &from.sin_addr;
         y = 3;
         while (y--) printf("%u.", *q++);
         printf("%u:%d ", *q, (in_port_t) PORT(from.sin_port));
         data[x] = 0;
         printf("%s", data);
         if (data[x - 1] ^ LF) printf("\n");
      }

      printf("oconsole stopped listening contingency %d\n", errno);
   }

   return 0;
}

