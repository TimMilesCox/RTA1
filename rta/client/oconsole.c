#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#ifdef	DOS
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

#include <errno.h>

#include "../include.rta/address.h"

#undef	TRACE
#define	DATA	16480
#define	LF	10

#ifdef	LINUX
static struct sockaddr_in here = { AF_INET, USTDOUT	} ;
static struct sockaddr_in from = { AF_INET		} ;
#endif

#ifdef	OSX
static struct sockaddr_in here = { 16, AF_INET, USTDOUT	} ;
static struct sockaddr_in from = { 16, AF_INET		} ;
#endif

#ifdef  DOS
static struct sockaddr_in here = {     AF_INET, USTDOUT	} ;
static struct sockaddr_in from = {     PF_INET, 0	} ;

static WSADATA wsa;
#endif

static socklen_t sixteen = 16;

int main()
{
   unsigned char	 data[DATA];
   unsigned char	*q;
   unsigned short	*udgram;

   char			 time_string[60];

   #ifdef DOS
   time_t		 ltime;
   struct _timeb	 tstruct;
   struct tm		 tfields = { 0, 0 } ;
   #else
   struct tm		*time_fields;
   struct timeval	 timep;
   #endif

   #ifdef DOS
   int                   wnet = WSAStartup(MAKEWORD(1, 1), &wsa);
   int                   s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   int                   f = 1;
   int                   u = ioctlsocket(s, FIONBIO, &f);
   int			 x = (s < 0) ? 0 : bind(s, (struct sockaddr *) &here, 16);
   _tzset();
   #else
   int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   int x = (s < 0) ? 0 : bind(s, (struct sockaddr *) &here, 16);
   #endif

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

         #ifdef	DOS
         _strdate_s(time_string, 60);
	 printf(time_string);
	 _strtime_s(time_string, 60);
         printf(time_string();
         // time(&ltime);
         // ftime_s(&tstruct); https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/time-time32-time64?view=msvc-170
         #else
	 gettimeofday(&timep, NULL);
         time_fields = localtime(&timep.tv_sec);
         strftime(time_string, 60, "%d.%m.%Y %T", time_fields);
         #endif

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

