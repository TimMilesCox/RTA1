#include <stdio.h>
#include <fcntl.h>
#include <net/bpf.h>
#include <errno.h>

#include "../include.rta/argue.h"
#include "../include.rta/physa.h"

#define	MASKS	48
#define	HOSTS	6

static unsigned char	 instruction[MASKS][8];

static int write_program(int iftype, int offset, unsigned char *p,
                                                 unsigned char *ofile1)
{
   unsigned char        host[HOSTS][72];
   unsigned char	text[72];

   int			 x,
			 y,
			 bytes;

   int			 f = open(ofile1, O_WRONLY | O_TRUNC | O_CREAT, 0777);

   if (f < 0)
   {
      printf("%s open error %d\n", ofile1, errno);
      return 0 - errno;
   }

   y = sscanf(p, "%[^+\n\r]+%[^+\n\r]+%[^+\n\r]+%[^+\n\r]+%[^+\n\r]+%[^+\n\r]",
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

   if (iftype) bytes = sprintf(text, "\tl,h\tdgram-2\n"
                                  "\tj\tETHERTYPE_ARP,,yes\n"
                                  "\tj\tETHERTYPE_IP,no\n", offset);
   else     bytes = sprintf(text, "\tl\tdgram-4\n\tj\tAF_INET,no\n", offset);
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


static int preport(int iftype, unsigned char *p, unsigned char *ofile1)
{
   static unsigned char	*heap = instruction[0];

   int			 x;

   switch (iftype)
   {
      case DLT_NULL:
         x = write_program(0, 4,  p, ofile1); 
         break;

      case DLT_EN10MB:
         x = write_program(1, 14, p, ofile1);
         break;

      default:
         return -2;
   }

   return 0;
}

int main(int argc, char *argv[])
{
   unsigned char	 device[24],
			 netdevice[24],
			 addresses[360];

   int			 x,
			 y,
                         f,
			 iftype;

   unsigned char	*p,
			*q;


   unsigned char	 ofile1[72];		 


   argue(argc, argv);

   for (x = 0; x < arguments; x++)
   {
      y = sscanf(argument[x], "%[^:]:%[^:]:%s", device, netdevice, addresses);

      if (y < 3)
      {
         printf("device argument %d not well formed\n ", x + 1);
         continue;
      }

      q = physa(netdevice);
      p = q - 11;
      
      sprintf(ofile1, "../temp.%s/physa", netdevice);
      f = open(ofile1, O_WRONLY | O_TRUNC | O_CREAT, 0777);

      if (f < 0) printf("%s open error %d\n", ofile1, errno);
      else
      {
         y = write(f, q, 16);
         y = write(f, p, 64);
         if (y < 0) printf("%s write error %d\n", ofile1, errno);
         close(f);
      }

      sprintf(ofile1, "../temp.%s/bpf_gate.msm", netdevice);
      iftype = 0;
      y = 6;
      while (y--) iftype |= *q++;
      if (iftype) iftype = 1;

      preport(iftype, addresses, ofile1);
   }

   return 0;
}

