#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

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


int main(int argc, char *argv[])
{
   unsigned char	 sdata[TEXT];
   unsigned char	 rdata[TEXT];

   int			 s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   int			 f = fcntl(s, F_GETFL, 0);
   int			 u = fcntl(s, F_SETFL, f | O_NONBLOCK);

   int			 x,
			 y,
			 maxtry,
			 symbol;

   unsigned char	*p;

   unsigned char	 newnet[ARGUMENTS][4];

   FILE			*config = fopen("config.fp", "r");

   argue(argc, argv);

   if (config)
   {
      while (p = fgets(rdata, TEXT-1, config))
      {
         x = sscanf(rdata, "%s %hhd.%hhd.%hhd.%hhd", sdata, &newnet[0][0],
                                                            &newnet[0][1],
                                                            &newnet[0][2],
                                                            &newnet[0][3]);

         if (strcmp(sdata, "fp-client") == 0)
         {
             local.sin_addr.s_addr = *((long *) newnet[0]);
         }

         if (strcmp(sdata, "fp-server") == 0) 
         { 
             target.sin_addr.s_addr = *((long *) newnet[0]);
         }
      }
   }

   for (x = 0; x < arguments; x++)
   {
      y = sscanf(argument[x], "%hhd.%hhd.%hhd.%hhd", &newnet[x][0],
                                                     &newnet[x][1],
                                                     &newnet[x][2],
                                                     &newnet[x][3]);
   }

   if (arguments)    target.sin_addr.s_addr = *((long *) newnet[0]);
   if (arguments > 1) local.sin_addr.s_addr = *((long *) newnet[1]);

   if (flag['v'-'a']) printf("[%8.8X > %8.8X]\n", local.sin_addr.s_addr,
                                                  target.sin_addr.s_addr);
   x = bind(s, (struct sockaddr *) &local, 16);
   y = connect(s, (struct sockaddr *) &target, 16);

   printf("remote application socket %d bind state %d "
          "F %x NB %d udconnect state %d\n",
	   s, x, f, u, y);


   for (;;)
   {
      p = fgets(sdata, TEXT, stdin);
      if (!p) break;
      if (sdata[0] == '.') break;

      x = send(s, sdata, strlen(sdata), 0);
      printf("send state %d\n", x);

      if (flag['z'-'a']) continue;

      maxtry = MAXTRY;
      while (maxtry--)
      {
         x = recv(s, rdata, TEXT, 0);
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
