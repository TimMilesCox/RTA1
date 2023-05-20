
#ifdef	OSX
#ifndef	INTEL
#define	FORK
#endif
#endif

/**********************************************

	that stuff is in case
	posix_spawnp can't be configured
	in gcc for powerpc OSX

**********************************************/

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
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/mman.h>

#ifndef	FORK
#include <spawn.h>
#endif
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#define	detail_code errno

extern char **environ;
#endif

#include "../include.rta/argue.h"
#include "../include.rta/address.h"
#include "fp.h"

#ifdef	LINUX
#define	RAMFS	"/dynamic/rta/"
#endif
#ifdef	OSX
#define	RAMFS	"/Volumes/rta/"
#endif

#define	ONLINE	RAMFS "online/"

//	"\033[38;5;94m"
#ifdef  MSW
#define BLUE
#define	GREEN
#define RED
#define PRIMARY
#define	BRIGHT
#else
#define BLUE            "\033[94m"
#define	GREEN		"\033[92m"
#define RED             "\033[91m"
#define PRIMARY         if (flag['b'-'a'] == 0) printf("\033[0m");
#define	BRIGHT		if (flag['b'-'a'] == 0) printf(bright);
#endif

static char		 bright[12] = BLUE;

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

static unsigned char	 path[240];
static unsigned char	 pdupath[240];
static unsigned char	 dynamic_masmdef[240];
static unsigned char	 save_masmdef[240];
static unsigned char	 path_txo[240];
static unsigned char	 msympath[240];
static unsigned char	 ramspace_d[240];
static unsigned char	 client_d[240];
static unsigned char	 fixed_names_path[240];

#ifdef FORK
#else
static char		*scriptp[] = { "bash", "./mpdu", NULL } ;
static char		*scriptq[] = { "bash", "./fponline.cfg", NULL } ;
static char		*scriprt[] = { ONLINE "masmx", msympath, path_txo, "-wkuX", NULL } ;
static char		*scriptl[] = { ONLINE "masmx", save_masmdef, "my_names.txo", "-wk", NULL } ;
#endif


static int		 name_store;
static int		 dynamic;

static FILE		*fixed_names_stream;

/****************************************************
	internalise and combine the tools

		lstring

		slab

	as a function to format a very simple
	text encoded binary

		pdu.txo

	this allows just-in-time assembly to
	be a posix_spawn of masmx
	not a spawn of of bash->script[several binaries]
****************************************************/

static int compress_txo(unsigned char *odata)
{
   int		 f = open(path_txo, O_RDONLY, 0777);
   int		 bytes = 0,
		 words = 0,
		 x;

   unsigned char *p;

   unsigned char idata[120];

   unsigned char txo_symbol[24][6];

   if (f < 0) return f;

   for (;;)
   {
      p = idata;

      while ((x = read(f, p, 1)) && (*p ^ '\n')) p++;
      if (x < 0) break;

      x = p - idata;
      *p = 0;

      if (x == 0) continue;

      if (idata[0] == '$') continue;
      if (idata[0] == '+') break;

      x = sscanf(idata,
                "%6s%6s%6s%6s %6s%6s%6s%6s %6s%6s%6s%6s %6s%6s%6s%6s",
                 txo_symbol[0], txo_symbol[1], txo_symbol[2], txo_symbol[3],
                 txo_symbol[4], txo_symbol[5], txo_symbol[6], txo_symbol[7], 
                 txo_symbol[8], txo_symbol[9], txo_symbol[10],txo_symbol[11],
                 txo_symbol[12],txo_symbol[13],txo_symbol[14],txo_symbol[15]);
 
      if (x < 0) break;

      words += x;

      p = txo_symbol[0];

      while(x--)
      {
         sscanf(p, "%2hhX%2hhX%2hhX", odata, odata + 1, odata + 2);
         p += 6;
         odata += 3;
         bytes += 3;
      }
   }

   close(f);
   if (x < 0) return x;
   if ((uflag['V'-'A']) && (idata[0] == '+')) printf("%s = %d\n", idata, words);
   return bytes;
}

/****************************************************
	suppress if possible
	the cloning of open file descriptors
****************************************************/

static void isolate(int f)
{
   int		 x = fcntl(f, F_GETFD);

   if (x < 0) printf("error %d retrieving open flags handle %d\n", errno, f);
   else
   {
      x = fcntl(f, F_SETFD, x | FD_CLOEXEC);
      if (x < 0) printf("error %d updating open flags handle %d\n", errno, f);
   }
}

static int fixed_name(char *candidate)
{
   char		 text[128];
   char		 name[120];
   int		 x;
   char		*p = NULL;

   if (fixed_names_stream)
   {
      rewind(fixed_names_stream);
 
      for (;;)
      {
         p = fgets(text, 128, fixed_names_stream);
         if (!p) break;
         if (text[0] ^ '+') continue;
         name[0] = 0;
         sscanf(text + 1, "%[^:]", name);
         if (flag['w'-'a']) printf("+%s\n", name);
         x = strcmp(name, candidate);
         if (x == 0) return 1;
      }
   }

   return 0;
}

static void recall_store(int bytes, char *text)
{
   int           x = write(name_store, text, bytes);
   int		 y,
		 j;

   if (x < 0)
   {
      printf("name insert failed %d\n", errno);
      return;
   }
 
   #ifdef MSW_X86

   #elif defined(FORK)

   x = fork();

   if (x)
   {
      /****************************************************
                this is the caller
      ****************************************************/

      if (flag['v'-'a']) printf(" %d names reassembly launched\n", x);
      y = waitpid(x, &j, 0);
      if (flag['v'-'a']) printf(" %d names reassembly returned, %x\n", y, j);

      if (j & 0x8000)
      {
         printf("names reassembly errored. cached names unchanged\n");
      }
   }
   else
   {
      /***************************************************
                this is the clone
      ***************************************************/

      chdir(ramspace_d);
      x = execlp("masmx", "masmx", save_masmdef, "my_names", "-kw", (char *) 0);
      if (x < 0) printf("[%d] new name assembly not started\n", errno);

      return x;
   }


   #else

   /******************************************************
         a name is added and cached names are reassembled
   ******************************************************/

   x = 0;

   chdir(ramspace_d);
   y = posix_spawnp(&x, ONLINE "masmx", NULL, NULL, scriptl, environ);
   chdir(client_d);

   if (y)
   {
      printf("names reassembly launch fail %d\n", y);
      return;
   }
   else if (x == 0)
   {
      printf("reassembly process not identified\n");
      return;
   }
   else
   {
      if (flag['v'-'a']) printf(" %d launched names reassembly\n", x);
      y = waitpid(x, &j, 0);
      if (flag['v'-'a']) printf(" %d returned names reassembly, %x\n", y, j);

      if (j & 0x8000)
      {
         printf("names reassembly errored. cached names unchanged\n");
      }
   }

   #endif
}

static void dynamic_store(int bytes, char *text)
{
   int           x;
   off_t	 position;

   position = lseek(dynamic, (off_t) 0, SEEK_SET);
   x = write(dynamic, text, bytes);
   if (x < 0) printf("total update failed on rewrite %d\n", errno);
}

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

   int			 ostates;

   int			 x,
			 y,
			 j,
			 maxtry,
			 symbol,
			 symbol1;

   int			 resends,
			 bytes;

   unsigned char	*p;

   unsigned char	 newnet[ARGUMENTS][4];

   FILE			*config = fopen("config.fp", "r");

   int			 request,
			 pduf;

   unsigned char	 name[160];
   unsigned char	 added_name[160];

   #if 0
   struct stat		 path_present;
   #endif

   argue(argc, argv);

   if (flag['w'-'a']) printf("[%s]", environ[0]);

   if (uflag['F'-'A']) strcpy(bright, RED);
   if (uflag['G'-'A']) strcpy(bright, GREEN);

   sprintf(pdupath, RAMFS "fponline/%s/pduimage", getenv("USER"));
   sprintf(path, RAMFS "fponline/%s/request.msm", getenv("USER"));
   sprintf(path_txo, RAMFS "fponline/%s/pdu.txo", getenv("USER"));
   sprintf(msympath, "%s/../../target.rta/fponline/pdu.msm", getenv("RTA_BINARY"));
   sprintf(ramspace_d, RAMFS "fponline/%s", getenv("USER"));
   sprintf(fixed_names_path, RAMFS "fponline/%s/fixed.txo", getenv("USER"));

   fixed_names_stream = fopen(fixed_names_path, "r");

   getcwd(client_d, 240);

   if (arguments > 2)
   {
      sprintf(dynamic_masmdef, "%s/dynamic.rta/dynamic.def", argument[2]);
      sprintf(save_masmdef, "%s/dynamic.rta/my_names.def", argument[2]);
   }
   else
   {
      sprintf(dynamic_masmdef, "%s/dynamic.rta/dynamic.def", getenv("HOME"));
      sprintf(save_masmdef, "%s/dynamic.rta/my_names.def", getenv("HOME"));
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

   /**************************************************************
      network bound
   **************************************************************/

   #ifdef FORK

   x = fork();

   /*******************************************************
		for platforms where posix_spawnp
		can't be configured
		mercifully no files are yet open
		so they won't get cloned 
   *******************************************************/

   if (x)
   {
      /****************************************************
                this is the caller
      ****************************************************/

      if (flag['v'-'a']) printf(" %d launched fpconfig.cfg\n", x);
      y = waitpid(x, &j, 0);
      if (flag['v'-'a']) printf(" %d returned fpconfig.cfg, %x\n", y, j);

      if (j & 0x8000)
      {
         printf("please run rta/client/fponline.cfg\n");
         return 0;
      }
   }
   else
   {
      /***************************************************
                this is the clone
      ***************************************************/

      x = execlp("./fponline.cfg", "blanco", (char *) 0);
      if (x < 0) printf("[%d] please run rta/client/fponline.cfg\n\n", errno);
      return x;
   }

   #else

   x = 0;
   y = posix_spawnp(&x, "bash", NULL, NULL, scriptq, environ);

   if ((y) || (x == 0))
   {
      printf("please cd rta/client and run  ./fponline.cfg\n"
             "then execute fponline again: error %d \n", y);

      return 0;
   }
   else
   {
      if (flag['v'-'a']) printf(" %d launched fponline.cfg\n", x);
      y = waitpid(x, &j, 0);
      if (flag['v'-'a']) printf(" %d returned fponline.cfg, %x\n", y, j);

      if (j & 0x8000)
      {
         printf("please run rta/client/fponline.cfg\n");
         return 0;
      }
   }
   #endif

   name_store = open(save_masmdef, O_CREAT | O_RDWR | O_APPEND, 0777);
   if (name_store < 0) printf("%s fixed names store in error %d\n", save_masmdef, errno);
   p = mmap(NULL, 262144, PROT_READ | PROT_WRITE, MAP_SHARED, name_store, 0);

   if (p == MAP_FAILED)
   {
      printf("mmap names %p %d\n", p, errno);
   }

   dynamic = open(dynamic_masmdef, O_CREAT | O_RDWR, 0777);
   if (dynamic < 0) printf("%s total store in error %d\n", dynamic_masmdef, errno);
   p = mmap(NULL, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, dynamic, 0);

   if (p == MAP_FAILED)
   {  
      printf("mmap total %p %d\n", p, errno);
   }

   request = open(path, O_RDWR | O_CREAT | O_TRUNC, 0777);
   if (request < 0) printf("%s action store in error %d\n", path, errno);

   if (flag['f'-'a'])
   {
      for (;;)
      {
         x = read(name_store, sdata, FPONLINE_TEXTL);
         if (x < 0) break;
         x = write(1, sdata, x);
         if (x < FPONLINE_TEXTL) break;
      }

      printf("end stored names\n");
   }

   strcpy(rdata, "0.0");

   if (dynamic < 0)
   {
   }
   else
   {
      x = read(dynamic, sdata, 18);

      if (x == 18)
      {
         x = memcmp(sdata, "\"$TOTAL\"\t$set,168\t%s\n", 18);

         if (x == 0)
         {
            x = read(dynamic, rdata, TEXTL - 1);
            if (x < 0) printf("running total bad read [%d]\n", errno);
            rdata[x] = 0;
         }
      }
   }

   if (flag['g'-'a']) printf("running total %s\n", rdata);

   if (flag['v'-'a'])
   {
      printf("dynamic.def %d\n", dynamic);
      printf("my_names.def %d\n", name_store);
   }

   isolate(name_store);
   isolate(dynamic);

   for (;;)
   {
      if (ostates < 0) break;

      p = fgets(sdata, FPONLINE_TEXTL, stdin);
      if (!p) break;

      if (flag['p'-'a']) printf(p);

      if (sdata[0] == '.') break;
      j = strlen(p);

      lseek(request, (off_t) 0, SEEK_SET);

      while (symbol = *p++)
      {
         if ((symbol ^ ' ') && (symbol ^ '\t')) break;
      }

      if (symbol ==    0) continue;
      if (symbol == '\r') continue;
      if (symbol == '\n') continue;

      symbol1 = symbol;

      if ((symbol == '_')
      || ((symbol > 'a'-1) && (symbol < 'z'+1))
      || ((symbol > 'A'-1) && (symbol < 'Z'+1)))
      {
         x = 0;

         for(;;)
         {
            if (symbol == ' ') break;
            if (symbol == 0) break;
            if (symbol == '\n') break;
            if (symbol == '\r') break;
            if (symbol == '+') break;
            if (symbol == '-') break;
            if (symbol == '*') break;
            if (symbol == '/') break;

            name[x++] = symbol;
            symbol = *p++;
         }

         name[x] = 0;
         while ((symbol == ' ') || (symbol == '\t')) symbol = *p++;

         if ((symbol1 ^ '_') && (symbol ^ '+') && (symbol ^ '-') && (symbol ^ '*') && (symbol ^ '/'))
         {
            if (fixed_name(name))
            {
               printf("%s is a fixed name and may not be updated dynamically\n", name);
               continue;
            }

            bytes = sprintf(added_name, "%s*\t$set,168\t%s\n", name, rdata);
            
            recall_store(bytes, added_name);
            continue;

            /************************************************************
		running total copied to a label
            ************************************************************/
         }

         /***************************************************************
		otherwise it is an operator
                fall through and assemble it in expression
		the label should exist
         ***************************************************************/
      }

      if (symbol1 == '_') x = write(request, "\t", 1);
      else                x = write(request, "\t$xqt_fp,$192\t", 14);

      if (x > 0)
      {
         if ((symbol1 == '+') || (symbol1 == '-') || (symbol1 == '*') || (symbol1 == '/'))
         x = write(request, "$TOTAL", 6);
      }

      if (x > 0) x = write(request, sdata, j);

      if (x < 0)
      {
         printf("error submitting algorithm\n", errno);
         break;
      }

      write(request, "\0", 1);

      #ifdef X86_MSW

      x = CreateProcess("cmd.exe", "mpdu.bat", NULL, NULL, 0, 0, NULL, NULL, NULL, &pi);
      if (x) printf("process launch %s failed E %x", "mpdu", GetLastError());
      else WaitForSingleObject(pi.hProcess, INFINITE);

      #elif defined(FORK)

      x = fork();

      if (x)
      {
         /****************************************************
		this is the caller
         ****************************************************/

         if (flag['v'-'a']) printf(" %d launched mpdu\n", x);
         y = waitpid(x, &j, 0);
         if (flag['v'-'a']) printf(" %d returned mpdu, %x\n", y, j);

         if (j & 0x8000)
         {
            printf("mpdu process error return\n");
            continue;
         }
      }
      else
      {
         /***************************************************
		this is the clone
         ***************************************************/

         if (uflag['S'-'A'])
         {
            x = execlp("./mpdu", "blanco", (char *) 0);
            if (x < 0) printf("[%d] process script [rta/client/]mpdu not started\n", errno);
         }
         else
         {
            chdir(ramspace_d);
            remove("pdu.txo");
            x = execlp("masmx", "masmx", msympath, "pdu", "-kwX", (char *) 0);
            if (x < 0) printf("[%d] masmx process not started\n", errno);
         }

         return x;
      }

      #else

      /******************************************************
         the the upline PDU is just-in-time assembled
         every interaction
         there is no point in cloning this process
         in order to start that process
         so posix_spawn() is called instead of fork()/execlp()
      ******************************************************/

      x = 0;

      if (uflag['S'-'A']) y = posix_spawnp(&x, "bash", NULL, NULL, scriptp, environ);
      else
      {
         chdir(ramspace_d);
         remove("pdu.txo");
         y = posix_spawnp(&x, ONLINE "masmx", NULL, NULL, scriprt, environ);

         chdir(client_d);
      }

      if (y)
      {
         printf("mpdu launch fail %d\n", y);
         continue;
      }
      else if (x == 0)
      {
         printf("launch process not identified\n");
         continue;
      }
      else
      {
         if (flag['v'-'a']) printf(" %d launched mpdu\n", x);
         y = waitpid(x, &j, 0);
         if (flag['v'-'a']) printf(" %d returned mpdu, %x\n", y, j);

         if (j & 0x8000)
         {
            printf("mpdu process error return\n");
            continue;
         }
      }

      #endif

      if (uflag['S'-'A'] == 0)
      {
         x = compress_txo(sdata);

         if (x < 0)
         {
            printf("error %d reading pdu image\n", errno);
            continue;
         }
      }
      else
      {
         pduf = open(pdupath, O_RDONLY, 0777);

         if (pduf < 1)
         {
            printf("error %d obtaining pdu image\n", errno);
            continue;
         }

         x = read(pduf, sdata, FPONLINE_TEXTL);

         if (x < 0)
         {
            printf("error %d reading pdu image\n", errno);
            close(pduf);
            continue;
         }

         close(pduf);
      }


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
         printf("recv state %d/%d\n", x, (x < 0) ? errno : TEXTL);
         for (y = 0; y < x; y++) printf("%2.2x", rdata[y]);
         if (y) putchar(10);
      }
      else
      {
         if ((uflag['Q'-'A'] == 0) || (x < 0)) printf("recv state %d/%d ", x, (x < 0) ? errno : TEXTL);
         if (x > 0)
         {
            BRIGHT
            printf("%s", rdata);
            PRIMARY
         }

         putchar(10);
      }

      if (x > 0)
      {
         memset(added_name, 0, 160);
         bytes = sprintf(added_name, "\"$TOTAL\"\t$set,168\t%s\n", rdata);
         dynamic_store(160, added_name);
      }
   }

   #ifdef DOS
   closesocket(s);
   #else
   close(s);
   #endif

   if (flag['v'-'a']) printf("save symbols\n");

   close(name_store);
   close(dynamic);
   close(request);

   return 0;
}
