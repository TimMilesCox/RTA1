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
#include <unistd.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define	detail_code errno
#endif

#include "../include.rta/argue.h"
#include "../include.rta/address.h"
#include "fp.h"

#define	RESENDS	5

#ifdef	LINUX
static struct sockaddr_in target = {     AF_INET, PORT(FPONLINE_SERVICE) } ;

static struct sockaddr_in local  = {     PF_INET, 0                      } ;
#endif


#ifdef	OSX
static struct sockaddr_in target = { 16, AF_INET, PORT(FPONLINE_SERVICE) } ;

static struct sockaddr_in local  = { 16, PF_INET, 0                      } ;
#endif

#ifdef	DOS
static PROCESS_INFORMATION pi;
static struct sockaddr_in target = {     AF_INET, PORT(FPONLINE_SERVICE) } ;

static struct sockaddr_in local  = {     PF_INET, 0                      } ;

static WSADATA wsa;
#endif


int main(int argc, char *argv[])
{
   unsigned char	 sdata[FPONLINE_TEXTL];
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

   int			 x,
			 y,
			 j,
			 maxtry,
			 symbol;

   int			 resends,
			 bytes;

   unsigned char	*p;

   unsigned char	 newnet[ARGUMENTS][4];

   FILE			*config = fopen("config.fp", "r");

   int			 runagate,
			 pduf;

   unsigned char	 path[72];
   unsigned char	 pdupath[72];


   argue(argc, argv);

   if (arguments > 2)
   {
      sprintf(path, "%s/dynamic.rta/runagate.msm", argument[2]);
      sprintf(pdupath, "%s/dynamic.rta/pduimage", argument[2]);
   }
   else
   {
      sprintf(path, "%s/dynamic.rta/runagate.msm", getenv("HOME"));
      sprintf(pdupath, "%s/dynamic.rta/pduimage", getenv("HOME"));
   }

   if (flag['v'-'a']) printf("[%s -> %s]\n", path, pdupath);

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
             local.sin_addr.s_addr = *((long *) newnet[0]);
         }

         if (strcmp(sdata, "fp-server") == 0) 
         { 
             target.sin_addr.s_addr = *((long *) newnet[0]);
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

   if (arguments)    target.sin_addr.s_addr = *((long *) newnet[0]);
   if (arguments > 1) local.sin_addr.s_addr = *((long *) newnet[1]);

   if (flag['v'-'a']) printf("[%8.8X > %8.8X]\n", local.sin_addr.s_addr,
                                                  target.sin_addr.s_addr);

   if (uflag['U'-'A']) target.sin_port = PORT(FP_SERVICE_192);

   x = bind(s, (struct sockaddr *) &local, 16);
   y = connect(s, (struct sockaddr *) &target, 16);

   if ((uflag['Q'-'A'] == 0)
   ||  (s < 0) || (x < 0) || (f < 0) || (u < 0) || (y < 0))
   printf("remote application socket %d bind state %d "
          "F %x NB %d udconnect state %d\n",
	   s, x, f, u, y);


   for (;;)
   {
      p = fgets(sdata, FPONLINE_TEXTL, stdin);
      if (!p) break;

      if (flag['p'-'a']) printf(p);

      if (sdata[0] == '.') break;
      j = strlen(p);

      runagate = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);

      if (runagate < 0)
      {  
         printf("please run setup script fponline.cfg in client directory\n"
                "then execute fponline again: error %d \n", errno);
      
         break;
      }

      x = write(runagate, "\t$xqt_fp,$192\t", 14);
      if (x == 14) x = write(runagate, sdata, j);

      if (x < 0)
      {
         printf("error submitting algorithm\n", errno);
         break;
      }

      close(runagate);


      #ifdef X86_MSW

      x = CreateProcess("cmd.exe", "mpdu.bat", NULL, NULL, 0, 0, NULL, NULL, NULL, &pi);
      if (x) printf("process launch %s failed E %x", "mpdu", GetLastError());
      else WaitForSingleObject(pi.hProcess, INFINITE);

      #else

      x = fork();

      if (x)
      {
         /****************************************************
		this is the caller
         ****************************************************/

         printf(" %d launched mpdu\n", x);
         y = waitpid(x, &j, 0);
         printf(" %d returned mpdu, %d\n", y, j);
         if (y < 0) printf("%d\n", errno);
      }
      else
      {
         /***************************************************
		this is the clone
         ***************************************************/

         execlp("./mpdu", "blanco", (char *) 0);
         return 0;             /* I think you have to stop */
      }

      #endif

      pduf = open(pdupath, O_RDONLY, 0777);


      if (pduf < 1)
      {
         printf("error %d obtaining pdu image\n", errno);
         break;
      }

      x = read(pduf, sdata, FPONLINE_TEXTL);

      if (x < 0)
      {
         printf("error %d reading pdu image\n", errno);
         continue;
      }

      close(pduf);

      resends = 1;
      if (uflag['R'-'A']) resends = RESENDS;
      bytes = x;

      while (resends--)
      {
         /*****************************************************
		RESEND is about sending again optionally
                for scripted benchmarks where one lost
		UDP response ought not stop benchmark
         *****************************************************/

         x = send(s, sdata, bytes, 0);

         if ((uflag['Q'-'A'] == 0) || (x < 0))
         printf("send state %d\n", x);

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

      if (x < 0) rdata[0] = 0;
      else rdata[x] = 0;

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
