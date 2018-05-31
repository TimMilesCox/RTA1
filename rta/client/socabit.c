#include <stdio.h>
#include <sys/fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "../include.rta/argue.h"


#ifdef	NETWORK_BYTE_ORDER

#define	PORT(X)	X
#define	THERE	0xAC1D0707
#define	HERE	0xAC1D0706

#else

#define	PORT(X)	((unsigned short) X >> 8) | ((X & 255) << 8)
#define	THERE	0x07071DAC
#define	HERE	0x06071DAC

#endif

#define	LISTENER 9000
#define	DATA	100
#define	RETRIES	50
#define	TIME	80000

#ifdef	OSX
struct sockaddr_in	 here = { 16, AF_INET, 0, { HERE } } ;
struct sockaddr_in	there = { 16, AF_INET, PORT(LISTENER), { THERE } } ;	
#endif

#ifdef	LINUX
struct sockaddr_in       here = {     AF_INET, 0, { HERE } } ;
struct sockaddr_in      there = {     AF_INET, PORT(LISTENER), { THERE } } ;
#endif

int main(int argc, char *argv[])
{
   unsigned char	 data[DATA];
   unsigned char	*p;

   int			 s,
			 f,
			 u,
			 retries,
			 x;

   argue(argc, argv);

   if (arguments)
   {
      p = (unsigned char *) &there.sin_addr;
      sscanf(argument[0], "%hhd.%hhd.%hhd.%hhd", p, p+1, p+2, p+3);

      here.sin_addr.s_addr = 0;

      if (arguments > 1)
      {
         p = (unsigned char *) &here.sin_addr;
         sscanf(argument[0], "%hhd.%hhd.%hhd.%hhd", p, p+1, p+2, p+3);
      }
   }

   s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   f = fcntl(s, F_GETFL, 0);
   u = fcntl(s, F_SETFL, f | O_NONBLOCK);

   if (s < 0)
   {
      printf("socket error %d\n", errno);
   }
   else if (f < 0)
   {
      printf("flagread error %d\n", errno);
   }
   else if (u < 0)
   {
      printf("flagwrite error %d\n", errno);
   }

   else
   {
      x = bind(s, (struct sockaddr *) &here, 16);

      if (x < 0)
      {
         printf("bind error %d\n", errno);
      }
      else
      {
         x = connect(s, (struct sockaddr *) &there, 16);

         if (x < 0)
         {
            printf("connect error %d\n", errno);
         }
         else
         {
            for(;;)
            {
               putchar('>');
               fflush(stdout);

               p = fgets(data, DATA, stdin);

               if (!p) break;
               if (*p == '.') break;

               x = send(s, p, DATA, 0);

               if (x < 0)
               {
                  printf("send error %d\n", errno);
                  break;
               }

	       retries = RETRIES;

               while (retries--)
               {
                  x = recv(s, data, DATA, 0);

                  if (x < 0)
                  {
                     if ((errno == EWOULDBLOCK)
                     ||  (errno == EAGAIN))
                     {
                        usleep(TIME);
                        continue;
                     }
                  }

                  break;
               }

               if (x < 0)
               {
                  if (errno == EWOULDBLOCK) continue;
                  if (errno == EAGAIN)      continue;
                  printf("recv error %d\n", errno);
                  break;
               }

               data[x] = 0;
               printf("[%s]\n", data);
            }
         }
      }
   }

   close(s);
   return 0;
}

