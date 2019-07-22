
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

#include <unistd.h>
#include <errno.h>
#endif


#define	HOSTS	6

static int write_program(int iftype, int offset,
                         unsigned char *net_addresses, unsigned char *ofile2)
{
   unsigned char        host[HOSTS][72];
   unsigned char	text[72];

   int			 x,
			 y,
			 bytes;

   #ifdef X86_MSW
   int f = open(ofile2, O_WRONLY | O_TRUNC | O_CREAT | O_BINARY, 0777);
   #else
   int f = open(ofile2, O_WRONLY | O_TRUNC | O_CREAT, 0777);
   #endif

   if (f < 0) return 0 - errno;
   y = sscanf(net_addresses,
            "%[^+\n\r]+%[^+\n\r]+%[^+\n\r]+%[^+\n\r]+%[^+\n\r]+%[^+\n\r]",
              host[0], host[1], host[2], host[3], host[4], host[5]);

   bytes = sprintf(text, "\t$include ../bpfmasmx/bpfmasmx.def\n");
   write(f, text, bytes);

   bytes = sprintf(text, "\t$include ../bpfmasmx/host.def\n");
   write(f, text, bytes);

   bytes = sprintf(text, "dgram\t$set\t%d\n", offset);
   write(f, text, bytes);

   bytes = sprintf(text, "\t$include ../bpfmasmx/dgram.def\n");
   write(f, text, bytes);

   #ifdef INTEL
   bytes = sprintf(text, "INTEL\t$set\t%d\n", 1);
   #else
   bytes = sprintf(text, "INTEL\t$set\t%d\n", 0);
   #endif

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

   for (x = 0; x < y; x++)
   {
      bytes = sprintf(text, "\tdest\t%s\tyes\n", host[x]);
      write(f, text, bytes);
   }

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
   sprintf(ofile1, "../temp.%s/bpf_gate.msm", ifname);
   sprintf(ofile2, "./fgen.%s", ifname);
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
   #else
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
   #endif

   return 0;
}

