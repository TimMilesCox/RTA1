#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "../include.rta/argue.h"
#include "../include.rta/address.h"

#define	RX_PORT	200
#define	DATA	16392
#define	SYMBOLS	72	

static int		 f;
static long		 cycle,
			 remote_cycle;;

#ifdef	OSX
static struct sockaddr_in	 host = { 16, AF_INET, PORT(RX_PORT) } ;
#endif

#ifdef	LINUX
static struct sockaddr_in	 host = { AF_INET, PORT(RX_PORT) } ;
#endif

static int		 sixteen = sizeof(struct sockaddr_in) ;
static char		*p = (char *) &host.sin_addr ;

static char	 	 remote_filename[SYMBOLS] ;
static char		 remote_response[24];
static char		 data[DATA];

void represent(int x, char *q) ;

int main(int argc, char *argv[])
{
   int	 f,
	 s,
	 interval = 50 << 10,
	 filename_bytes,
	 symbol,
	 y,
	 x = 0; 

   char *q;
   unsigned char *uq = (unsigned char *) remote_response;

   argue(argc, argv);

   if (arguments) x = sscanf(argument[0], "%hhd.%hhd.%hhd.%hhd",
                                          p, p + 1, p + 2, p + 3) ;

   if ((arguments < 2) || (x < 4)) printf("tx host datafile\n");
   else
   {
      f = open(argument[1], O_RDONLY);

      if (f < 0) printf("datafile unavailable\n");
      else
      {
         x = read(f, data, DATA); 
         if (x < DATA)
         {
            if (x < 0) printf("no data read %d\n", errno);
            else       printf("insufficient data %d\n", x);
         }
         else
         {
            close(f);

            #if 0
            s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (s < 0) printf("no socket  %d\n", errno);
            else
            #endif
 
            for (;;)
            {
               printf("%hhd.%hhd.%hhd.%hhd:%d file>",
                       *p, *(p + 1), *(p + 2), *(p + 3), RX_PORT);

               fflush(stdout);

               q = fgets(remote_filename, SYMBOLS, stdin);

               if (q == NULL)
               {
                  printf("stdio broken\n");
                  break;
               }

               q = strchr(q, '\n');

               if (q)
               {
                  if ((q > remote_filename) && (*(q - 1) == '\r')) q--;
                  *q = 0;
                  filename_bytes = q - remote_filename;
               }
               else filename_bytes = strlen(remote_filename);
    
               if (filename_bytes == 0) continue;

               if (remote_filename[0] == '+')
               {
                  interval = (atoi(remote_filename + 1)) << 10;
                  continue;
               }

               s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

               if (s < 0) printf("no socket  %d\n", errno);
               else
               {
                  x = connect(s, (struct sockaddr *) &host, sizeof(struct sockaddr_in));

                  if (x < 0) printf("connect fail %d\n", errno);
                  else
                  {
                      x = send(s, remote_filename, filename_bytes, 0);
                      if (x < 0) break;

                      if (remote_filename[0] == '.') break;

                      x = recv(s, remote_response, 6, 0);

                      if ((x ^ 6) || (memcmp(remote_response, "send\0\0", 6)))
                      {
                         if       (x < 0) printf("prompt fail %d\n", errno);
                         else if (x == 0) printf("remote close\n");
                         else
                         {
                            printf("unexpected prompt %d: [", x);
                            represent(x, remote_response);
                            printf("]\n");
                         }

                         break;
                      }

                      if (remote_filename[0] == '=')
                      {
                         for(;;)
                         {
                            x = send(s, data, DATA, 0);
                            if (x < 0) break;
                            usleep(interval);
                            if (remote_filename[0] == '.') break;

                            if (uflag['R'-'A'])
                            {
                               x = recv(s, remote_response, 6, 0);

                               if (x < 6)
                               {
                                  if       (x < 0) printf("remote response fail\n", errno);
                                  else if (x == 0) printf("remote close\n");
                                  else
                                  {
                                     printf("unexpected response %d: [", x);
                                     represent(x, remote_response);
                                     printf("]\n");
                                  }

                                  break;
                               }

                               remote_cycle = (unsigned long) uq[5]
                                           |                 (uq[4] << 8)
                                           |                 (uq[3] << 16)
                                           |                 (uq[2] << 24)
                                           | ((unsigned long) uq[1] << 32)
                                           | ((unsigned long) uq[0] << 40) ;
                            }

                            cycle++;
                            printf("[%ld:%ld]\r", cycle, remote_cycle);
                            fflush(stdout);
                            usleep(interval);
                         }
                      }
                      else x = send(s, data, DATA, 0);

                      x = close(s);
                      printf("txclose %d\n", x);
                      if (x < 0) break;
                  }
               }
               if (x < 0) printf("connect / send error %d\n", errno);
            }
         }   
      }
   }

   return 0;
}

void represent(int count, char *q)
{
   int		 x = count,
  		 symbol;

   if (count > 32) x = 32;


   while (x--)
   {
      symbol = *q++;

      if ((symbol < ' ') || (symbol > 127)) printf(".%2.2x.", symbol);
      else putchar(symbol);

      if (count > 32) printf("...");
   }
}

