#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <pthread.h>
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
static struct sockaddr_in	 remote = { 16, AF_INET } ;
#endif

#ifdef	LINUX
static struct sockaddr_in	 host = { AF_INET, PORT(RX_PORT) } ;
static struct sockaddr_in	 remote = { AF_INET } ;
#endif

static int		 sixteen = sizeof(struct sockaddr_in) ;
static char		*p = (char *) &host.sin_addr ;

static char	 	 remote_filename[SYMBOLS] ;
static char		 remote_response[24];
static char		 data[DATA];
static char		 lookaside[DATA];

static pthread_attr_t	asyncb;
static pthread_t	asyncid;

static int		async_intervention;

static void *async() ;
static void represent(int x, char *q) ;
static int howso(char *left, char *right, int bytes) ;

int main(int argc, char *argv[])
{
   int	 f = -1,
	 s = -1,
	 newso = -1,
	 interval = 50 << 10,
	 filename_bytes,
	 symbol,
	 y,
	 x = 0; 

   char *q;
   unsigned char *uq = (unsigned char *) remote_response;

   argue(argc, argv);

   x = pthread_attr_init(&asyncb);

   if (x < 0) printf("threadcbinit %d e %d\n", x, errno);
   else
   {
      x = pthread_create(&asyncid, &asyncb, &async, NULL);
      if (x < 0) printf("async thread start %d %d\n", x, errno);
      else       printf("async thread ID %p\n", asyncid);
   }

   s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   if (s < 0) printf("no socket  %d\n", errno);
   else
   {
      x = bind(s, (struct sockaddr *) &host, sizeof(struct sockaddr_in));

      if (x < 0) printf("bind fail %d\n", errno);
      else
      {
         x = listen(s, 7);

         if (x <  0) printf("listen fail %d\n", errno);
         else for (;;)
         {
            if (async_intervention < 0) break;

            newso = accept(s, (struct sockaddr *) &remote, (socklen_t *) &sixteen);

            if (newso < 0)
            {
               printf("accept fail %d\n", errno);
               break;
            }
            else
            {
               x = recv(newso, remote_filename, SYMBOLS, 0);

               if (x < 0)
               {  
                  printf("rx output filename fail %d\n", errno);
                  break;
               }
               else
               {
                  while (x--)
                  {
                     if (remote_filename[x] == '\n') remote_filename[x] = 0;
                     if (remote_filename[x] == '\r') remote_filename[x] = 0;
                  }

                  if (remote_filename[0] == '.') break;
                  else if (remote_filename[0] == '#')
                  {
                     x = send(newso, "send\0\0", 6, 0);

                     if (x < 0)
                     {
                        printf("tx prompt fail %d\n", errno);
                        break;
                     }

                     for (;;)
                     {
                        if (async_intervention < 0) break;

                        x = recv(newso, lookaside, DATA, MSG_WAITALL);
                     
                        if (x < 0)
                        {  
                           printf("rx data for compare fail %d\n", errno);
                           break;
                        }

                        if (x == 0)
                        {  
                           printf("remote close halt %d\n", errno);
                           break;
                        }
                        
                        if ((uflag['E'-'A']) && (lookaside[0] < 0))
                        {
                           printf("remote EOF halt %d\n", errno);
                           break;         
                        }
                                            
                        x = howso(data, lookaside, DATA);

                        if (x)
                        {
                           printf("rx data noncorresponding halt %d\n", x);
                           break;
                        }

			cycle++;
                        printf("\r[%ld]", cycle);
                        fflush(stdout);
                     }
                  }
                  else
                  {
                     x = send(newso, "send\0\0", 6, 0);

                     if (x < 0)
                     {
                        printf("tx prompt fail %d\n", errno);
                        break;
                     }

                     x = recv(newso, data, DATA, MSG_WAITALL);

                     if (x < 0)
                     {
                        printf("rx data fail %d\n", errno);
                        break;
                     }

                     f = open(remote_filename, O_RDWR | O_CREAT | O_TRUNC, 0777);

                     if (f < 0)
                     {
                        printf("data file write open fail %d\n", errno);
                        break;
                     }

                     x = write(f, data, DATA);

                     if (x < 0)
                     {
                        printf("data file write fail %d\n", errno);
                        break;
                     }

                     x = close(f);

                     if (x < 0)
                     {
                        printf("data file close fail %d\n", errno);
                        break;
                     }
                  }
               }
            }
         }
      }
   }

   close(f);
   close(s);
   close(newso);

   return 0;
}

static void represent(int count, char *q)
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

static int howso(char *left, char *right, int bytes)
{
   int		 range1 = 12,
                 range2 = 12,
		 offset;
                 
   char		*start = right;

   while (bytes--)
   {
      if (*left++ - *right++)
      {
         left--;
         right--;
         offset = right - start;

         if (bytes < 11) range1 = range2 = bytes + 1;

         printf("difference at offset %d\n", offset);
         while (range1--) printf(" %2.2x", *left++);
         putchar('\n');
         while (range2--) printf(" %2.2x", *right++);
         putchar('\n');
         return 1;
      }
   }

   return 0;
}

static void *async()
{
   char		 intervention[72] = "\0";
   char		*q;

   for (;;)
   {
      q = fgets(intervention, 72, stdin);

      if (q == NULL)
      {
         printf("stdin broken: application stopping\n");
         async_intervention = -1;
         break;
      }

      if (intervention[0] == 0) 
      {
         printf("operator intervention: application stopping\n");
         async_intervention = -1;
         break;
      }
   }

   return NULL;
}

