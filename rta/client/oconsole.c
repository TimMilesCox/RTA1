#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#include "../include.rta/address.h"

#undef	TRACE
#define	DATA	480
#define	LF	10

#ifdef	LINUX
static struct sockaddr_in here = { AF_INET, USTDOUT	} ;
static struct sockaddr_in from = { AF_INET		} ;
#endif

#ifdef	OSX
static struct sockaddr_in here = { 16, AF_INET, USTDOUT	} ;
static struct sockaddr_in from = { 16, AF_INET		} ;
#endif

static socklen_t sixteen = 16;

int main()
{
   unsigned char	 data[DATA];
   unsigned char	*q;
   unsigned short	*udgram;

   char			 time_string[60];
   struct tm		*time_fields;
   struct timeval	 timep;

   int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   int x = (s < 0) ? 0 : bind(s, (struct sockaddr *) &here, 16);

   int			 y;
   int			 iphl;

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

	 gettimeofday(&timep, NULL);
         time_fields = localtime(&timep.tv_sec);
         strftime(time_string, 60, "%d.%m.%Y %T", time_fields);

         if (from.sin_addr.s_addr == LOCAL)
         {
            /********************************************************
		option -o relay
		because WiFi broadcast is not getting received
                entire datagram has been forwarded
		to allow display of [original:source]
            ********************************************************/

            iphl = (data[0] & 15) << 2;
            q = data + iphl;
            udgram = (unsigned short *) q;

            printf("[%u.%u.%u.%u:%d] ", data[12], data[13],
                                        data[14], data[15], PORT(*udgram));
            q = data + iphl + 8;
            x -= iphl;
            x -= 8;
         }
         else
         {
            q = (unsigned char *) &from.sin_addr;
            y = 3;
            while (y--) printf("%u.", *q++);
            printf("%u:%d ", *q, (in_port_t) (PORT(from.sin_port)) & 0xFFFF);
            q = data;
         }

         printf("%s.%3.3u ", time_string, timep.tv_usec / 1000);
         q[x] = 0;
         printf("%s", q);
         if (q[x - 1] ^ LF) printf("\n");
      }

      printf("oconsole stopped listening contingency %d\n", errno);
   }

   return 0;
}

