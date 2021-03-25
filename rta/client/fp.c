#include <stdio.h>
#include <string.h>

#ifdef	DOS
#include <fcntl.h>
#include <errno.h>
#include <winsock.h>
#define	detail_code WSAGetLastError()
#define EAGAIN WSAEWOULDBLOCK
#define	EWOULDBLOCK WSAEWOULDBLOCK
#define	usleep(X) Sleep(X/500)
#else
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define	detail_code errno
#endif

#include "../include.rta/argue.h"
#include "../include.rta/address.h"
#include "fp.h"

#ifdef	LINUX
static struct sockaddr_in target = {     AF_INET, PORT(FP_SERVICE) } ;

static struct sockaddr_in local  = {     PF_INET, 0                } ;
#endif


#ifdef	OSX
static struct sockaddr_in target = { 16, AF_INET, PORT(FP_SERVICE) } ;

static struct sockaddr_in local  = { 16, PF_INET, 0                } ;
#endif

#ifdef	DOS
static struct sockaddr_in target = {     AF_INET, PORT(FP_SERVICE) } ;

static struct sockaddr_in local  = {     PF_INET, 0                } ;

static WSADATA wsa;
#endif


int main(int argc, char *argv[])
{
   unsigned char	 sdata[TEXTL];
   unsigned char	 rdata[TEXTL];

   #ifdef DOS
   int			 wnet = WSAStartup(MAKEWORD(1, 1), &wsa);
   int			 s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   int			 f = 1;
   int			 u = ioctlsocket(s, FIONBIO, &f);
   #else
   int			 s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   int			 f = fcntl(s, F_GETFL, 0);
   int			 u = fcntl(s, F_SETFL, f | O_NONBLOCK);
   #endif

   int			 ostates;

   int			 x,
			 y,
			 maxtry,
			 symbol;

   int			 resends,
			 bytes;

   unsigned char	*p;

   unsigned char	 newnet[ARGUMENTS][4];

   FILE			*config = fopen("config.fp", "r");

   argue(argc, argv);

   if (flag['v'-'a'])
   {
      #if DOS
      printf("[wsa %d socket %d cntl %d:%d]\n", wnet, s, f, u);
      if (wnet) printf("[wsadetail %d\n]", WSAGetLastError());
      #else
      printf("[socket %d cntl %d:%d]\n", s, f, u);
      #endif
   }

   if (config)
   {
      while (p = fgets(rdata, TEXTL-1, config))
      {
         if (flag['q'-'a']) printf(p);

         x = sscanf(rdata, "%s %hhd.%hhd.%hhd.%hhd", sdata, &newnet[0][0],
                                                            &newnet[0][1],
                                                            &newnet[0][2],
                                                            &newnet[0][3]);

         if (strcmp(sdata, "fp-client") == 0)
         {
             local.sin_addr.s_addr = *((int *) newnet[0]);
         }

         if (strcmp(sdata, "fp-server") == 0) 
         { 
             target.sin_addr.s_addr = *((int *) newnet[0]);
         }
      }
      fclose(config);
   }

   for (x = 0; x < arguments; x++)
   {
      y = sscanf(argument[x], "%hhd.%hhd.%hhd.%hhd", &newnet[x][0],
                                                     &newnet[x][1],
                                                     &newnet[x][2],
                                                     &newnet[x][3]);
   }

   if (arguments)    target.sin_addr.s_addr = *((int *) newnet[0]);
   if (arguments > 1) local.sin_addr.s_addr = *((int *) newnet[1]);

   if (flag['v'-'a']) printf("[%8.8X > %8.8X]\n", local.sin_addr.s_addr,
                                                  target.sin_addr.s_addr);

   if (uflag['U'-'A']) target.sin_port = PORT(FP_SERVICE_192);

   x = bind(s, (struct sockaddr *) &local, 16);
   if (x < 0) x = 0 - errno;
   y = connect(s, (struct sockaddr *) &target, 16);
   if (y < 0) y = 0 - errno;

   ostates = s | x | f | u | y ;

   if ((uflag['Q'-'A'] == 0) || (ostates < 0))
   {
      printf("remote application socket %d bind state %d "
             "F %x NB %d udconnect state %d\n",
              s, x, f, u, y);
   }

   for (;;)
   {
      if (ostates < 0) break;

      p = fgets(sdata, TEXTL, stdin);
      if (!p) break;

      if (flag['p'-'a']) printf(p);

      if (sdata[0] == '.') break;

      resends = 1;
      if (uflag['R'-'A']) resends = RESENDS;
      bytes = strlen(sdata);

      while (resends--)
      {
         /*****************************************************
		RESEND is about sending again optionally
                for scripted benchmarks where one lost
		UDP response ought not stop benchmark
         *****************************************************/

         x = send(s, sdata, bytes, 0);

         if (x < 0)
         {
            printf("socket TX E %d\n", errno);
            break;
         }

         if (uflag['Q'-'A'] == 0) printf("send state %d\n", x);

         if (flag['z'-'a']) continue;

         maxtry = MAXTRY;

         while (maxtry--)
         {
            /*************************************************
		MAXTRY is about restarting the nonblock
		receive / wait. There are MAXTRY polls
            *************************************************/

            x = recv(s, rdata, TEXTL, 0);
            if (x < 0)
            {
               if ((detail_code == EAGAIN) || (detail_code == EWOULDBLOCK))
               {
                  x = 0;
                  usleep(TWARP);
               }
               else
               {
                  printf("RX state %d\n", detail_code);
                  break;
               }
            }

            if (x) break;
         }

         if (x) break;
      }

      if (x < 0) break;

      rdata[x] = 0;

      if (flag['x'-'a'])
      {
         printf("recv state %d/%d\n", x, errno);
         for (y = 0; y < x; y++) printf("%2.2x", rdata[y]);
         if (y) putchar(10);
      }
      else
      {
         if ((uflag['Q'-'A'] == 0) || (x < 0))
         printf("recv state %d/%d %s", x, errno, rdata);
         else if (x >= 0) printf("%s", rdata);
      }
   }

   #ifdef DOS
   closesocket(s);
   #else
   close(s);
   #endif
   return 0;
}
