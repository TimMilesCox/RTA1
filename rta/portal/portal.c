
#ifdef	X86_MSW
#define	INTEL
#include <stdio.h>
#include <fcntl.h>
#define	DLT_NULL	0
#define	DLT_EN10MB	1
//	#include <net/bpf.h>
#include <windows.h>
#include <errno.h>
#else
#include <stdio.h>
#include <fcntl.h>

#include <sys/wait.h>
#ifdef	LINUX
#define DLT_NULL        0
#define DLT_EN10MB      1
#include <linux/bpf.h>
#else
#include <net/bpf.h>
#endif

#include <string.h>
#include <unistd.h>
#include <errno.h>
#endif

#include "../include.rta/argue.h"

#ifdef	FORK
#else
#include <spawn.h>
extern char		**environ;
pid_t			 masmx_process;
static char		*spawn_arguments[] = { "bash", NULL, NULL } ;
#endif

#include "../portal/portal.h"

static int write_program(int iftype, int offset,
                         unsigned char *net_addresses, unsigned char *ofile2)
{
   #ifdef REVISION1
   unsigned char	host[ADDRESS_STRING];
   #else
   unsigned char        host[HOSTS][ASTRING];
   #endif

   unsigned char	text[ADDRESS_STRING+80];

   int			 x,
			 y,
			 bytes;

   #ifdef X86_MSW
   int f = open(ofile2, O_WRONLY | O_TRUNC | O_CREAT | O_BINARY, 0777);
   #else
   int f = open(ofile2, O_WRONLY | O_TRUNC | O_CREAT, 0777);
   #endif

   printf("symbolic %d %s\n", f, ofile2);

   if (f < 0) return 0 - errno;

   #ifdef REVISION1
   #else
   y = sscanf(net_addresses,
            "%[^+\n\r]+%[^+\n\r]+%[^+\n\r]+%[^+\n\r]+%[^+\n\r]+%[^+\n\r]",
              host[0], host[1], host[2], host[3], host[4], host[5]);
   #endif

   #ifdef NET
   bytes = sprintf(text, "\t$path ../../bpfmasmx/\n");
   #else
   bytes = sprintf(text, "\t$path ../bpfmasmx/\n");
   #endif

   write(f, text, bytes);

   bytes = sprintf(text, "\t$include bpfmasmx.def\n");
   write(f, text, bytes);

   bytes = sprintf(text, "\t$include host.def\n");
   write(f, text, bytes);

   bytes = sprintf(text, "dgram\t$set\t%d\n", offset);
   write(f, text, bytes);

   bytes = sprintf(text, "\t$include dgram.def\n");
   write(f, text, bytes);

   bytes = sprintf(text, "\t$path\n");
   write(f, text, bytes);
   
   #ifdef INTEL
   bytes = sprintf(text, "INTEL\t$set\t%d\n", 1);
   #else
   bytes = sprintf(text, "INTEL\t$set\t%d\n", 0);
   #endif

   write(f, text, bytes);

   bytes = sprintf(text, "__RELOAD\t$set\t1\n");
   write(f, text, bytes);

   switch (iftype)
   {
      case DLT_NULL:
	 #ifdef	LINUX
	 bytes = sprintf(text,	"\tl,h\tdgram-2\n"
			 	"\tj\tETHERTYPE_IP,no\n");
	 #else
         #ifdef INTEL
         bytes = sprintf(text, "\tl\t0\n\tj\tAF_INET_INVERSE,no\n");
         #else
         bytes = sprintf(text, "\tl\t0\n\tj\tAF_INET,no\n");
         #endif
         #endif
         break;

      case DLT_EN10MB:
         bytes = sprintf(text, "\tl,h\tdgram-2\n"
                               "\tj\tETHERTYPE_ARP,,yes\n"
                               "\tj\tETHERTYPE_IP,no\n");
         break;

      default:
         printf("link header size %d, link protocol void\n", offset);
         bytes = 0;
   }

   write(f, text, bytes);

   #ifdef REVISION1

   while (*net_addresses)
   {
      /*******************************************************
         1st clause is the interface primary network address
         any more clauses are +net_address
                           or @route
      *******************************************************/

      net_addresses = peel(host, net_addresses);
      bytes = sprintf(text, "\tdest\t%s\tyes\n", host);
      write(f, text, bytes);

      while (*net_addresses == '@')
      {
         /***************************************************
            router clauses are not part of the filter program
         ***************************************************/

          net_addresses = peel(host, net_addresses + 1);
      }

      if (*net_addresses++ == '+') continue;
      break;
   }

   #else

   for (x = 0; x < y; x++)
   {
      bytes = sprintf(text, "\tdest\t%s\tyes\n", host[x]);
      write(f, text, bytes);
   }

   #endif

   bytes = sprintf(text, "no\tret\t0\nyes\tret\t65535");
   write(f, text, bytes);

   close(f);
   return 0;
}


int portal(int iftype, unsigned char *ifname, unsigned char *net_addresses)
{
   unsigned char	 ofile1[36],
			 ofile2[36];

   int			 x,
			 y,
			 j;

   #ifdef X86_MSW
   static PROCESS_INFORMATION pi;

   sprintf(ofile1, "..\temp.%s\bpf_gate.msm", ifname);
   sprintf(ofile2, "/c .\fgen_%s.bat", ifname);
   #else

   if (flag['a'-'a'])
   {
      printf("generate alternate filters\n");
      #ifdef ANET
      sprintf(ofile1, ANET "%s/bpf_gate.msm", ifname);
      sprintf(ofile2, "./fgena.%s", ifname);
      #else
      sprintf(ofile1, "../atemp.%s/bpf_gate.msm", ifname);
      sprintf(ofile2, "./fgen.%s", ifname);
      #endif
   }
   else
   {
      #ifdef NET
      sprintf(ofile1, NET "%s/bpf_gate.msm", ifname);
      sprintf(ofile2, "./fgen.%s", ifname);
      #else
      sprintf(ofile1, "../temp.%s/bpf_gate.msm", ifname);
      sprintf(ofile2, "./fgen.%s", ifname);
      #endif
   }
   #endif

   switch (iftype)
   {
      case DLT_NULL:
	 #ifdef LINUX
	 x = write_program(iftype, 14, net_addresses, ofile1);
	 #else
         x = write_program(iftype,  4, net_addresses, ofile1); 
         break;
         #endif

      case DLT_EN10MB:
         x = write_program(iftype, 14, net_addresses, ofile1);
         break;

      default:
         printf("link protocol %d unidentified\n", iftype);
         x = write_program(iftype,  0, net_addresses, ofile1);
   }

   #ifdef X86_MSW
   x = CreateProcess("cmd.exe", ofile2, NULL, NULL, 0, 0, NULL, NULL, NULL, &pi);
   if (x) printf("process launch fgen_%s failed E ", ifname, GetLastError());
   else WaitForSingleObject(pi.hProcess, INFINITE);
   #elif defined(FORK)
   x = fork();

   if (x)
   {
      printf(" %d launched %s\n", x, ofile2);
      y = waitpid(x, &j, 0);
      printf(" %d returned %s, %d\n", y, ofile2, j);
      if (y < 0) printf("**E** %d\n", errno);
   }
   else
   {
      y = execlp(ofile2, "blanco", (char *) 0);
      if (y < 0) printf("E %d nonstart ./%s\n", errno, ofile2);
   }
   #else
   spawn_arguments[1] = ofile2;
   y = posix_spawnp(&masmx_process, "bash", NULL, NULL, spawn_arguments, environ);

   if (y < 0)
   {
      printf("filter assembly launch fail %d\n", errno);
   }
   else if (masmx_process == 0)
   {
      printf("filter assembly process not identified\n");
   }
   else
   {
      if (flag['v'-'a']) printf(" %d launched filter assembly %s\n", masmx_process, ofile2);
      y = waitpid(masmx_process, &j, 0);
      if (y < 0) printf("waitpid reported problem %s\n", strerror(errno));
      if (flag['v'-'a']) printf(" %d returned filter assembly %s, %x\n", y, ofile2, j);

      if (j & 0x8000)
      {
         printf("%s filter assembly errored\n", ofile2);
      }
   }

   #endif

   return 0;
}

