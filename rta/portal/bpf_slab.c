
/***************************************************************

	bpf_slab

	the simplest in a chain of simple programs which make
	make RTA1 emulated machine active on real networks

	portal application calls masmx and then this routine
	in a child process script for each real interface
	on its command line

        portal connects emulated and virtual machines to real
        networks by binding with Berkeley Packet Filter

                portal  /dev/bpf3:en1:10.0.45.11+172.29.30.25/12        \
                        /dev/bpf0:lo0:172.29.30.26/32+192.168.5.103:23
	
	masmx and then this routine, bpf_slab, prepare filter programs
	for BPF from lines of assembly language code which portal has
	written out reflecting its command line arguments

	the filters are assembled with masmx meta-assembler
	steered by macro header files

	        $include ../bpfmasmx/bpfmasmx.def
	        $include ../bpfmasmx/host.def
		$include ../bpfmasmx/dgram.def

	It's done this way so that decision making in the
	BPF filter program can be steered and tuned using the
	macro capability of the powerful masmx meta-assembler

	the output of masmx is text encoded binary (.txo),
	a text readable format with code in hex and with
	information likewise in text about linkage, relocation,
	program sections, label exports and imports

	BPF filter "instructions" (in reality binary encoded
	rules) are 64 bits of binary encoded information of
	immaculately uniform format

	this routine reads .txo output from masmx and compresses
	the zero-based code part of it an into absolute binary 
	BPF "program", a small stack of eight-byte filter rules

	portal then applies its arguments to configure its
	platform-internal clients and starts delivering
	network traffic at link layer
	
	************************************************************

	portal replaces the service which emulated prototype machines
	have had from ipfw divert

	platform suppliers no longer offer ipfw divert and don't
	always make their replacement ideas work

	portations of pcap have not all delivered the nonblocking service
	which emulated and virtual machines need, even when the underlying
	BPF on the same platform delivers the required service

	some pcap portations also generate unnecessarily long filter
	programs, even when tcpdump on the same platform generates
	well-optimised filter programs

	***********************************************************

	portal with its child-process callout to masmx has also
	obtained optimally minimal BPF filter programs



*******************************************************************/


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include.rta/argue.h"

static int resolve(int symbol)
{
   if ((symbol > '0' - 1) && (symbol < '9' + 1)) return symbol - '0';
   if ((symbol > 'A' - 1) && (symbol < 'F' + 1)) return symbol - 'A' + 10;
   return -1;
}

static int rline(int f, char *data)
{
   int			 x = 0,
			 y,
   			 symbol;

   for (;;)
   {
      y = read(f, data, 1);

      if (y < 1)
      {
         if (x == 0) return -1;
         break;
      }
      symbol = *data;
      if (symbol == '\r') continue;
      if (symbol == '\n') break;
      data++;
      x++;
   }

   *data = 0;
   if (flag['v'-'a']) printf("[%d]\n", x);
   return x;
}

int main(int argc, char *argv[])
{
   int			 i,
			 o,
			 x,
			 symbol,
			 symbol1;

   unsigned char	 data[360],
			 frame[180],
			*p,
			*q;


   argue(argc, argv);
   i = open(argument[0], O_RDONLY, 0777);
   o = open(argument[1], O_WRONLY | O_TRUNC | O_CREAT, 0777);

   if      (i < 0) printf("file 1 not read\n");
   else if (o < 0) printf("file 2 not written\n");
   else for (;;)
   {
      x = rline(i, data);
      if (x < 0) break;
      if (data[0] == '$') continue;
      if (data[0] == ':') continue;

      p = data;
      q = frame;

      while (symbol = *p++)
      {
         if (symbol == ' ') continue;
         symbol = resolve(symbol);
         if (symbol < 0) break;
         symbol <<= 4;

         symbol1 = *p++;
         if (symbol1 == 0) break;
         symbol1 = resolve(symbol1);
         if (symbol1 < 0) break;

         symbol |= symbol1;
         *q++ = symbol;
      }

      x = q - frame;
      write(o, frame, x);           
   }

   close(i);
   close(o);
   return 0;
}

