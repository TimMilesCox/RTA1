#include <stdio.h>
#include <fcntl.h>
#include <net/bpf.h>
#include <unistd.h>
#include <errno.h>

#define	HOSTS	6

static int write_program(int iftype, int offset,
                         unsigned char *net_addresses, unsigned char *ofile2)
{
   unsigned char        host[HOSTS][72];
   unsigned char	text[72];

   int			 x,
			 y,
			 bytes;


   int f = open(ofile2, O_WRONLY | O_TRUNC | O_CREAT, 0777);

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

   bytes = sprintf(text, "INTEL\t$set\t%d\n", INTEL);
   write(f, text, bytes);

   switch (iftype)
   {
      case DLT_NULL:
         bytes = sprintf(text, "\tl\t0\n\tj\tAF_INET_INVERSE,no\n");
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
}


int portal(int iftype, unsigned char *ifname, unsigned char *net_addresses)
{
   unsigned char	 ofile1[36],
			 ofile2[36];

   int			 x,
			 y,
			 j;


   sprintf(ofile1, "../temp.%s/bpf_gate.msm", ifname);
   sprintf(ofile2, "./fgen.%s", ifname);

   switch (iftype)
   {
      case DLT_NULL:
         x = write_program(iftype,  4, net_addresses, ofile1); 
         break;

      case DLT_EN10MB:
         x = write_program(iftype, 14, net_addresses, ofile1);
         break;

      default:
         printf("link protocol %d unidentified\n", iftype);
         x = write_program(iftype,  0, net_addresses, ofile1);
   }

   x = fork();

   if (x)
   {
      printf(" %d launched %s\n", x, ofile2);
      y = waitpid(x, &j, 0);
      printf(" %d returned %s, %d\n", y, ofile2, j);
      if (y < 0) printf("%d\n", errno);
   }
   else execlp(ofile2, "blanco", (char *) 0);

   return 0;
}

