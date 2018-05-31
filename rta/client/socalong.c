#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "../include.rta/argue.h"

#undef	NETWORK_BYTE_ORDER

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
#define	DATA	160

struct sockaddr_in	 here = { 16, AF_INET, 0, { HERE } } ;
struct sockaddr_in	there = { 16, AF_INET, PORT(LISTENER), { THERE } } ;	

int main(int argc, char *argv[])
{
   unsigned char	 data[DATA];
   unsigned char	*p;

   int			 s,
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

   s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   if (s < 0)
   {
      printf("socket error %d\n", errno);
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

               p = fgets(data, DATA-1, stdin);

               if (!p) break;
               if (*p == '.') break;

               x = send(s, p, strlen(p), 0);

               if (x < 0)
               {
                  printf("send error %d\n", errno);
                  break;
               }

               x = recv(s, data, 6, 0);

               if (x < 0)
               {
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

