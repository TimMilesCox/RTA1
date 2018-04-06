#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <errno.h>

#include "../include.rta/argue.h"
#include "../netifx/ifconfig.h"
#include "../rta.run/settings.h"
#include "../include.rta/address.h"
#include "../netifx/sifr_mm.h"

#include "iph.h"
#include "tcph.h"
#include "apdata.h"

typedef struct { datagram	dgram;
		 segment	 segm;
		 apdata		 data; } test_pdu;

#define	HOW_WIDE	84
#define	HOW_MANY	5000000

static int		 fi,	/*  input file handle	*/
			 fo,	/* output file handle	*/
			 iconnects;

static mm_netdevice	*ifpointer;
static mm_netbuffer	*rxpointer;
static mm_netbuffer	*txpointer;

static unsigned char	 response[HOW_WIDE];
static unsigned char	 wide[HOW_WIDE * HOW_MANY];

static void first_rank()
{
   unsigned char	*p = wide;
   test_pdu		*q;
   int			 x;

   for (;;)
   {
      x = read(fi, p, HOW_WIDE);

      if (x < HOW_WIDE)
      {
         printf("anomaly loading 1st rank\n", errno);
         break;
      }

      q = (test_pdu *) p;
      p += HOW_WIDE;
      if (q->segm.hl_code & TCP_SYN) iconnects++;
      else break;
   }

   printf("%d iconnects\n", iconnects);
}
        
static unsigned short tcp_checksum(int bytes, char *tcp_segment, char *net_addresses)
{
   unsigned short               *p = (unsigned short *) net_addresses;
   unsigned short                x = (bytes+1) >> 1;

   unsigned long                 sum = bytes + IPPROTO_TCP;
   unsigned short                carry;

   #ifdef INTEL
   sum = ((sum >> 8) | (sum << 8)) & 65535;
   #endif

   sum += *p + p[1] + p[2] + p[3];

   p = (unsigned short *) tcp_segment;
   if (bytes & 1) tcp_segment[bytes] = 0;

   sum += *p + p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7]
                                                       + p[9];
   p += 10;
   x -= 10;

   while (x--) sum += *p++;
   carry = sum;
   while (sum >>= 16)
   {
      sum += carry;
      carry = sum;
   }

   /*
   p = (unsigned short *) tcp_segment;
   p[8] = carry ^ 65535;
   */
   return carry ^ 65535;
}

/*******************************************************
	ipresent offers one segment from each connection
	in a BPF-style buffer array
	skips the ended connections
	and returns the number of connections operating
*******************************************************/

static int ipresent()
{
   return 0;
}

/*******************************************************
	iprogress writes outputs to file
        and replaces acknowledged segments in the
	input array from the input traffic file
*******************************************************/

static int iprogress()
{
   return 0;
}

static void walk()
{
   int			 x;

   /****************************************************
	progress target responses
	while driving input to target
   ****************************************************/

   for (;;)
   {
      for (;;)
      {
         x = iprogress();
         if (x == 0) break;
      }

      x = ipresent();
      if (x < 0) break;
   }
   /***************************************************
	all TCP_DELETETCB
   ***************************************************/
}

int main(int argc, char *argv[])
{
   int			 x,
                         mhandle;

   char			*dpointer;

   argue(argc, argv);

   if (arguments)
   {
      fi = open(argument[0], O_RDONLY, 0777);
      x = errno;
      fo = open(argument[1], O_RDWR | O_CREAT, 0777);
      if      (fi < 0) printf(" input traffic open error %d\n", x);
      else if (fo < 0) printf("output traffic open error %d\n", errno);
      else
      {
         mhandle = shmget('aaaa', DEVICE_PAGE * DEVICE_PAGES * DEVICE_COMPARTMENTS, 0);

         printf("netbank %d\n", mhandle);

         if (mhandle < 0)
         {
            printf("device array not large enough for test traffic compartment\n");
            printf("code %d\n", errno);
            return 0;
         }

         dpointer = shmat(mhandle, NULL, 0);

         if ((long) dpointer < 0)
         {
            printf("error mapping device array %d\n", errno);
            return 0;
         }

         dpointer += DEVICE_PAGE * DEVICE_PAGES;
         printf("compartment base %p\n", dpointer);

         ifpointer = (mm_netdevice *) dpointer;
         rxpointer = (mm_netbuffer *) ifpointer;
         txpointer = rxpointer + DEVICE_PAGES / 2;

         first_rank();
         walk();
      }
   }
   else printf("argument 1 must be a test traffic file\n"
               "argument 2 must be an output capture file\n");

   return 0;
}

