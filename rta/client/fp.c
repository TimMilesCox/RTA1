#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "argument.h"

#define MTU		16384

#ifdef	INTEL
#define	TARGET_PORT	0x0012
#define	LISTEN		0x07071DAC

#define	DEFAULT
#ifdef	DEFAULT
#define	CLIENT		0
#else
#define	CLIENT		0x06071DAC
#endif

#else

#define	TARGET_PORT	0x1200
#define	LISTEN		0xAC1D0707

#ifdef	DEFAULT
#define CLIENT		0
#else
#define	CLIENT		0xAC1D0706
#endif

#endif

#define	MAXTRY		1200
#define	TWARP		2400

static struct sockaddr_in	 target
			= { 16, AF_INET, TARGET_PORT, { LISTEN } } ;

static struct sockaddr_in	 local = { 16, PF_INET, 0, { CLIENT } } ;

ARGUMENTS

int main(int argc, char *argv[])
{
   unsigned char	 sdata[MTU];
   unsigned char	 rdata[MTU];
   unsigned char	*p;

   int			 s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   int			 f = fcntl(s, F_GETFL, 0);
   int			 u = fcntl(s, F_SETFL, f | O_NONBLOCK);

   int			 maxtry,
			 x,
			 y,
			 symbol;

   unsigned char	 newnet[FILENAMES][4];

   ARGUMENT

   for (x = 0; x < filenames; x++)
   {
      y = sscanf(filename[x], "%hhd.%hhd.%hhd.%hhd", &newnet[x][0],
                                                     &newnet[x][1],
                                                     &newnet[x][2],
                                                     &newnet[x][3]);
   }

   if (filenames)    target.sin_addr.s_addr = *((long *) newnet[0]);
   if (filenames > 1) local.sin_addr.s_addr = *((long *) newnet[1]);

   if (flag['v'-'a']) printf("[%8.8X > %8.8X]\n", local.sin_addr.s_addr,
                                                  target.sin_addr.s_addr);
   x = bind(s, (struct sockaddr *) &local, 16);
   y = connect(s, (struct sockaddr *) &target, 16);

   printf("remote application socket %d bind state %d "
          "F %x NB %d udconnect state %d\n",
	   s, x, f, u, y);


   for (;;)
   {
      p = fgets(sdata, MTU-20-8, stdin);
      if (!p) break;
      if (sdata[0] == '.') break;

      x = send(s, sdata, strlen(sdata), 0);
      printf("send state %d\n", x);

      if (flag['z'-'a']) continue;

      maxtry = MAXTRY;
      while (maxtry--)
      {
         x = recv(s, rdata, MTU-20-8, 0);
         if (x < 0)
         {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
               x = 0;
               usleep(TWARP);
            }
            else
            {
               printf("RX state %d\n", errno);
               break;
            }
         }

         if (x) break;
      }

      if (x < 0) rdata[0] = 0;
      else rdata[x] = 0;

      if (flag['x'-'a'])
      {
         printf("recv state %d/%d\n", x, errno);
         for (y = 0; y < x; y++) printf("%2.2x", rdata[y]);
         if (y) putchar(10);
      }
      else printf("recv state %d/%d %s", x, errno, rdata);
   }

   close(s);
   return 0;
}
