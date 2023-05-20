#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "../include.rta/argue.h"
#include "../include.rta/address.h"

#ifdef	LINUX
#define	SOCKETS	1000
#else
#define	SOCKETS 250
#endif

#define	PROTO_SERVICE	10240
#define	TEXT	72

#ifdef  LINUX
static struct sockaddr_in target = {     AF_INET, PORT(PROTO_SERVICE) } ;

static struct sockaddr_in local  = {     PF_INET, 0               } ;
#endif


#ifdef  OSX
static struct sockaddr_in target = { 16, AF_INET, PORT(PROTO_SERVICE) } ;

static struct sockaddr_in local  = { 16, PF_INET, 0               } ;
#endif

#ifdef  DOS
static struct sockaddr_in target = {     AF_INET, PORT(PROTO_SERVICE) } ;

static struct sockaddr_in local  = {     PF_INET, 0                } ;

static WSADATA wsa;
#endif

static int		 target_port = 10240;
static int		 sixteen = 16;

static void icolumn(int what, char *where)
{
   int          used = what % 10000;

   *where++ = used / 1000 | '0'; used %= 1000;
   *where++ = used / 100  | '0'; used %= 100;
   *where++ = used / 10   | '0'; used %= 10;
   *where = used | '0';
}

int main(int argc, char *argv[])
{
   #ifdef DOS
   int                   wnet = WSAStartup(MAKEWORD(1, 1), &wsa);
   int			 proposed = 250;			 
   int			 sockets = 0;
   int			 s[SOCKETS]; /* = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); */
   int                   f = 1;
   int                   u = ioctlsocket(s, FIONBIO, &f);
   #else
   int			 proposed = 250;
   int			 sockets = 0;
   int                   s[SOCKETS] /* = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) */ ;
   int                   f	/*	= fcntl(s, F_GETFL, 0)	*/	;
   int                   u	/*	= fcntl(s, F_SETFL, f | O_NONBLOCK)	*/ ;
   #endif

   char			 text[TEXT] = "proto";
   char			*p = (char *) &target.sin_addr.s_addr;

   int			 x, y, cycles, mlen, cycle, recycle;

   int			 pass_total;
   int			 total;
   int			 pass_errors;
   int			 errors;
   int			 all_totals = 0;
   int			 all_errors = 0;
   int			 connections = 0;

   argue(argc, argv);

   if (arguments == 0) argument[0] = "172.29.7.7:10240";

   x = sscanf(argument[0], "%hhd.%hhd.%hhd.%hhd:%d", p, p + 1, p + 2, p + 3, &target_port);

   if (arguments > 1)
   {
      sscanf(argument[1], "%d", &proposed);
      if (proposed < 0) proposed = 1;
      if (proposed > SOCKETS) proposed = SOCKETS;
   }

   if (x < 4)
   {
      printf("target selection indecisive %d.%d.%d.%d:%d\n", *p, *(p + 1), *(p + 2), *(p + 3), target_port);
      return 0;
   }

   target.sin_port = PORT(target_port);

   for (sockets = 0; sockets < proposed; sockets++)
   {
      s[sockets] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
      printf ("s %d:%d\n", sockets, s[sockets]);

      if (s[sockets] < 0)
      {
         printf("no socket %d, E %d\n", sockets, errno);
         if (sockets == 0) return 0;
         break;
      }

      local.sin_port = 0;
      x = bind(s[sockets], (struct sockaddr *) &local, 16);

      if (x < 0)
      {
         printf("bind error socket %d, E %d\n", sockets, errno);
         if (sockets == 0) return 0;
         break;
      }

      x = connect(s[sockets], (struct sockaddr *) &target, 16); 

      if (x < 0)
      {
         printf("connect error socket %d, E %d\n", sockets, errno);
         if (sockets == 0) return 0;
         break;
      }

      connections++;
   }

   for (;;)
   {
      printf("\nproto>");
      fflush(stdout);
      
      p = fgets(text + 5, TEXT - 5, stdin);
      if (p == NULL) break;
      if (*p == '.') break;

      if (*p == '-')
      {
         reflag(p);
         printf("flags are ");
         for (x = 0; x < 26; x++) if (flag[x]) putchar('a' + x);
         for (x = 0; x < 26; x++) if (uflag[x]) putchar('A' + x);
         putchar('\n');
         continue;
      }

      memcpy(text, "0005 ", 5);
      cycles = 5;
      sscanf(text + 5, "%d", &cycles);
      if (cycles < 0) cycles = 0;
//      if (x > 0) icolumn(cycles, text);
      mlen = strlen(text);

      total = 0;
      errors = 0;

      for (cycle = 0; cycle < cycles; cycle++)
      {
         pass_total = 0;
         pass_errors = 0;
         icolumn(cycle, text);

         for (y = 0; y < sockets; y++)
         {
            icolumn(cycle, text);
            x = send(s[y], text, mlen, 0);

            if (x < 0)
            {
               printf("xmit error socket %d, E %d\n", y, errno);
            }
         }

         for (y = 0; y < sockets; y++)
         {
            x = recv(s[y], text, TEXT, 0);

            if (x < 0)
            {
               printf("rx error, E %d\n", errno);
            }
            else
            {
               recycle = atoi(text);

               if (recycle ^ cycle)
               {
                  pass_errors++;
                  if (flag['q'-'a'] == 0) printf("[%d:%d %d:%d]\n", y, s[y], recycle, cycle);
               }

               pass_total++;

               if (flag['q'-'a'] == 0)
               {
                  text[x] = 0;
                  printf("%d:%d %s\n", y, s[y], text);
               }
            }
         }

         total += pass_total;
         errors += pass_errors;
         printf("%d responses on pass %d\n", pass_total, cycle);
         if (pass_errors) printf("%d errors\n", pass_errors);
      }

      all_errors += errors;
      all_totals += total;
      printf("total responses %d\n", total);
      if (errors) printf("total errors %d\n", errors);
   }

   for (y = 0; y < sockets; y++) close(s[y]);

   printf("%d connections\nall totals %d\n", connections, all_totals);
   if (all_errors) printf("total sequence errors %d\n", all_errors);
   return 0;
}

