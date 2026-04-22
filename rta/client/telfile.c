#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>

#include "../include.rta/address.h"
#include "../include.rta/argue.h"

#define	__PORT	PORT(23)
#define	TX_DATA	84
#define	RX_DATA	84

#ifdef LINUX
static struct sockaddr_in this = { AF_INET } ;
static struct sockaddr_in that = { AF_INET, __PORT } ;
#endif

#ifdef OSX
static struct sockaddr_in this = { sizeof(struct sockaddr_in), AF_INET } ;
static struct sockaddr_in that = { sizeof(struct sockaddr_in), AF_INET, __PORT } ;
#endif

static char	tx_data[TX_DATA];
static char	rx_data[RX_DATA];

static int	sixteen = sizeof(struct sockaddr_in) ;
static int	port;

int main(int argc, char *argv[])
{
   char		*p,
		*q;

   int		 j,
		 x,
		 y;

   int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) ;

   if (s < 0) printf("no socket E %d\n", errno) ;
   else
   {
      x = bind(s, (struct aockaddr *) &this, sizeof(struct sockaddr_in));

      if (flag['v'-'a']) printf("bind status %d\n\n", x);

      if (x < 0) printf("not bound E%d\n", errno) ;
      else
      {
         if (argc < 2) printf("address argument required n.e.t.a[:port]\n");
         else
         {
            argue(argc, argv);

            q = argument[0];
            p = (int *) &that.sin_addr.s_addr;

            if (flag['v'-'a']) printf("%s\n", argument[0]);

            x = sscanf(q, "%hhd.%hhd.%hhd.%hhd:%d", p, p + 1, p + 2, p + 3, &port) ;

            if (x > 4) that.sin_port = PORT(port);

            if (flag['v'-'a'])
            printf("%hhu.%u.%u.%u:%u\n", p[0], p[1], p[2], p[3],PORT(that.sin_port));
 
            x = connect(s, (struct aockaddr *) &that, sizeof(struct sockaddr_in));
            if (flag['v'-'a']) printf("connect %d\n", x);

            if (x < 0) printf("connection fail E %d\n", errno);
            else
            {
               for (;;)
               {
                  q = fgets(tx_data, TX_DATA, stdin);

                  if (q == NULL)
                  {
                     printf("no data\n");
                     break;
                  }

                  x = send(s, tx_data, strlen(tx_data), 0) ;
                  if (x < 0) break;
                  if (flag['w'-'a']) usleep(1000);

                  if ((tx_data[0] == '.')
                  ||  (tx_data[0] & 0x80)) break;

                  j = recv(s, rx_data, RX_DATA, 0) ;
                  if (j < 0) break;

                  write(1, rx_data, j);
               }
            }
         }
      }
   }

   close(s);
   if (x < 0) printf("tx E %d\n", errno);
   if (j < 0) printf("rx E %d\n", errno);
   printf("eof\n");
   return 0;
}
