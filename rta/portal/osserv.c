#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <net/bpf.h>

#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <errno.h>

#include <sys/shm.h>
#include <pthread.h>

#include "../include.rta/argue.h"
#include "../netifx/ifconfig.h"
#include "../rta.run/settings.h"
#include "../include.rta/address.h"
#include "../netifx/sifr_mm.h"
#include "../include.rta/physa.h"


//	#define	IP		PORT(0x0800)
//	#define	FRAME		FORWARD(0x8000)
#define	LLHL		0
//	#define FORWARD(X)      PORT(X) & 65535
#define	MASKS		120

static int			 iftype[INTERFACES];
static int			 s[INTERFACES];
static struct ifreq		 sandl[INTERFACES];
static struct bpf_program	 bpfp[INTERFACES];
static unsigned int		 one = 1;
static unsigned int		 zero;

static unsigned int		 maximum;
static int			 halt_flag;

/**********************************************
	shared memory anf r/w cursors
**********************************************/

static mm_netdevice     *netdata;
static mm_netbuffer	*rxdata;
static mm_netbuffer     *txdata;

/**********************************************
	filter rules and assignment pointer
**********************************************/

static unsigned char     instruction[MASKS][8];
static unsigned char	*heap = instruction[0];

static struct bpf_hdr	 intro = { { 0, 0 } , 0, 0, sizeof(struct bpf_hdr) } ;


/**********************************************

	async()
        reads stdin to allow option switching
        and clean halt if wished

**********************************************/


static unsigned short udp_checksum(int bytes, char *user_datagram, char *net_addresses);
static unsigned short tcp_checksum(int bytes, char *tcp_segment, char *net_addresses);


static void *async()
{
   char                  request[64];
   int                   symbol;
   int                   x, y;

   unsigned char        *p;

   for (;;)
   {
      p = fgets(request, 62, stdin);
      if (!p) continue;

      switch(request[0])
      {
         case '-':
            p++;

            while (symbol = *p++)
            {
               if ((symbol > 'A'-1) && (symbol < 'Z'+1)) uflag[symbol-'A'] ^= 1;
               if ((symbol > 'a'-1) && (symbol < 'z'+1))  flag[symbol-'a'] ^= 1;
            }

            break;

         case '.':
            printf("stop the interface? key . again>");
            p = fgets(request, 62, stdin);
            if (!p) continue;
            if (request[0] == '.') halt_flag = -1;
            break;

         case '|':
            y = open(request+2, O_CREAT | O_TRUNC | O_RDWR, 0777);
            if (y < 0) printf("oE %d\n", errno);
            else
            {
               for (x = 0; x < txdata - netdata->o; x++) write(y,  &netdata->o[x], sizeof(mm_netbuffer));
               close(y);
               printf("%d blocks\n", x);
            }
            break;
      }
   }
}

static void outputq()
{
   mm_netbuffer         *q = txdata;

   int                   x,
                         iphl,
                         bytes,
                         y;

   unsigned short        psum, csum;

   unsigned char	*dgram;
   unsigned char        *ipayload;
   unsigned short       *wpayload;

   char                  pbuffer[24];
   int                   k, symbol;

   unsigned short        interface,
                         llhl,
                         tx_bytes,
			 dgraml;

   int			 fdes;
   unsigned char	*p;


   while (q->preamble.flag & FRAME)
   {
      interface = PORT(q->preamble.i_f);

      switch (iftype[interface])
      {
         case DLT_NULL:
            llhl = 4;
            break;

         default:
            llhl = 14;
      }

      dgram = q->frame + llhl;

      tx_bytes = PORT(q->preamble.frame_length);
      dgraml = tx_bytes - llhl;

      if (flag['v'-'a']) printf("[%x:tx %x]\n", interface, tx_bytes);

      x = tx_bytes;
      fdes = s[interface];

      p = q->frame;
      if (iftype[interface] == DLT_NULL)
      {
         printf("[%p:%x--", p, x);
         p += 4;
         x -= 4;
         printf("%p:%x]\n", p, x);
      }
      #if 0
      else
      {
         intro.bh_caplen = x;
         intro.bh_datalen = x;
         write(fdes, &intro, sizeof(intro));
      }
      #endif

      y = write(fdes, p, x);

      if (flag['v'-'a'])
      {
         #if 0
         oversee(netdata->o, q);
         #endif

         printf("TX %d\n", y);
         if (y < 0) printf("%d\n", errno);

         if ((x) && (flag['u'-'a']))
         {
            iphl = (*dgram << 2) & 60;
            ipayload = dgram + iphl;
            wpayload = (unsigned short *) ipayload;

            switch(dgram[9])
            {
               case IPPROTO_UDP:
                  psum = wpayload[3];
                  csum = udp_checksum(dgraml - iphl, ipayload, dgram + 12);

                  #ifdef INTEL
                  csum = (csum>>8)|(csum<<8);
                  psum = (psum>>8)|(psum<<8);
                  #endif

                  printf("[%x:%x]\n", psum, csum);
                  break;
               case IPPROTO_TCP:
                  psum = wpayload[8];
                  csum = tcp_checksum(dgraml - iphl, ipayload, dgram + 12);

                  #ifdef INTEL
                  csum = (csum>>8)|(csum<<8);
                  psum = (psum>>8)|(psum<<8);
                  #endif

                  printf("[%x:%x]\n", psum, csum);
                  break;
            }

            if (flag['w'-'a'])
            {
               y = 0;
               k = 0;
               bytes = 0;
               pbuffer[20] = 0;

               putchar('[');
               while (y < llhl) printf("%2.2x", q->frame[y++]);
               printf("]\n");

               while  (y < tx_bytes)
               {
                  if (k == 0) printf("%4.4x  ", bytes);
                  symbol = (unsigned char) q->frame[y];
                  printf("%2.2x", symbol);
                  if (symbol < ' ') symbol = '.';
                  if (symbol > 126) symbol = '.';
                  pbuffer[k] = symbol;
                  y++;
                  k++;
                  if (k == 20)
                  {
                     k = 0;
                     bytes += 20;
                     printf("    %s\n", pbuffer);
                  }
               }

               if (k)
               {
                  pbuffer[k] = 0;
                  while (k++ < 20) printf("  ");
                  printf("    %s\n", pbuffer);
               }

               #if 0
               for (y = 0; y < x; y++) printf("%2.2x", q->frame[y]);
               putchar('\n');
               #endif
            }
            putchar('\n');
         }
      }

      q->preamble.flag = 0;
      q++;

      if (q > &netdata->o[DEVICE_PAGES / 2 - 1]) q = netdata->o;
   }

   txdata = q;
}



/**********************************************

        adds a checksum if there is none
        and returns IP header length in
        bytes

**********************************************/


static int ip_checksum(unsigned short *q)
{
   #ifdef INTEL
   int                   iphl = ((*q) & 0x000F) << 1;
   #else
   int                   iphl = ((*q) & 0x0F00) >> 7;
   #endif

   int                   x = iphl << 1;

   unsigned short       *uph = q;
   unsigned long         sum = 0;
   unsigned short        checksum;

   q[5] = 0;
   while (iphl--) sum += *uph++;

   checksum = sum;

   while (sum >>= 16)
   {
      sum += checksum;
      checksum = sum;
   }

   q[5] = checksum ^ 0xFFFF;

   return x;
}

static void icmp_checksum(int bytes, char *icmp_message)
{
   unsigned short               *p = (unsigned short *) icmp_message;
   unsigned long                 sum = *p++;
   unsigned short                delivered = *p++;
   unsigned short                carry;
   unsigned short                x = (bytes + 1) >> 1;


   if (bytes & 1) icmp_message[bytes] = 0;
   x -= 2;

   while (x--) sum += *p++;
   carry = sum;

   while (sum >>= 16)
   {
      sum += carry;
      carry = sum;
   }

   /************************************************************

        just checking
        ICMP checksum appears to be correct even from
        OSX loopback

   ************************************************************/

   carry ^= 65535;
   if (carry ^ delivered) printf("[icmpCS>>%x/%x]", delivered, carry);
}

static unsigned short udp_checksum(int bytes,
                                   char *user_datagram, char *net_addresses)
{
   unsigned short               *p = (unsigned short *) net_addresses;
   unsigned short                x = (bytes+1) >> 1;

   unsigned long                 sum = bytes + IPPROTO_UDP;
   unsigned short                carry;

   #ifdef INTEL
   sum = ((sum >> 8) | (sum << 8)) & 65535;
   #endif

   sum += *p + p[1] + p[2] + p[3];
   if (flag['v'-'a']) printf("[%x:%lx:", bytes, sum);

   p = (unsigned short *) user_datagram;
   if (bytes & 1) user_datagram[bytes] = 0;
   p[3] = 0;

   while (x--) sum += *p++;

   if (flag['v'-'a']) printf("%lx]", sum);
   carry = sum;

   while (sum >>= 16)
   {
      sum += carry;
      carry = sum;
   }

   p = (unsigned short *) user_datagram;
   carry ^= 65535;
   p[3] = carry;
   return carry;
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

/*****************************************************
	b2b is like strcpy
	but does not copy the trailing zero byte
	although it would not matter if it did
*****************************************************/

static void b2b(unsigned char *to, unsigned char *from)
{
   int			 symbol;

   while (symbol = *from++) *to++ = symbol;
}

#undef TRACE
#ifdef TRACE

static void sloco(unsigned char *to, unsigned char *from, int bytes)
{
   int			 symbol;

   putchar('|');
   while (bytes--)
   {
      symbol = *from++;
      printf("%2.2x", symbol);
      *to++ = symbol;
   }
   printf("|\n");
}

#endif

static void forward(int x, unsigned char *p, int bytes)
{
   int			 j,
			 y,
			 dgraml,
			 ll_hl,
			 proto,
			 symbol;

   unsigned short	 csum, psum;

   unsigned char	 showrow[16];

         switch (iftype[x])
         {
            case DLT_NULL:
               ll_hl = 4;
               rxdata->preamble.protocol = IP;
               break;

            case DLT_EN10MB:
               ll_hl = 14;
               rxdata->preamble.protocol = *((unsigned short *) ((unsigned char *) p + 12));
               break;

            default:
               ll_hl = 14;
               rxdata->preamble.protocol = *((unsigned short *) ((unsigned char *) p + 12));
         }

         if (uflag['L'-'A']) rxdata->preamble.ll_hl = PORT(ll_hl);
         else
         {
            /********************************************
		only the datagram will be forwarded
		advance pointer past link header
		reduce octet count by link header
		optionally print the link header
            ********************************************/

            if (flag['v'-'a'])
            {
               j = ll_hl;
               while (j--) printf("%2.2x", *p++);
               printf("]\n");
            }
            else p += ll_hl;

            rxdata->preamble.ll_hl = 0;
            bytes -= ll_hl;
         }

         rxdata->preamble.frame_length = PORT(bytes);
         rxdata->preamble.i_f = PORT(x);

         #ifdef TRACE
         sloco(rxdata->frame, p, bytes);
         #else
         memcpy(rxdata->frame, p, bytes);
         #endif

         p = rxdata->frame;

         if (uflag['L'-'A'])
         {
            /********************************************
		the whole frame has been forwarded
		now for display purposes
		optionally print the link header
		advance pointer to datagram
		reduce octet count to datagram
            ********************************************/

            if (flag['v'-'a'])
            {
               j = ll_hl;
               while (j--) printf("%2.2x", *p++);
               printf("]\n");
            }
            else  p += ll_hl;

            bytes -= ll_hl;
         }

         if ((flag['z'-'a']) && (rxdata->preamble.protocol == IP & 65535))
         {
            /************************************************

                OSX on loopback writes zero IP checksum

                and header part of UDP checksum (neither
                zero nor the correct UDP checksum)

                TCP checksum is also junk on loopback

                although ICMP checksum is always correct

                so execute with -Z or -z option on loopback

                option -X or -x regenerates UDP checksums
                instead of just clearing thayum

            ***********************************************/

            if (flag['v'-'a']) printf("[%4.4x]\n", rxdata->preamble.protocol);

            dgraml = (p[2] << 8) | p[3];
            y = ip_checksum((unsigned short *) p);

            switch (p[9])
            {
               case IPPROTO_UDP:
                  if (flag['x'-'a'])
                  {
                     psum = (p[y + 6] << 8)
                          |  p[y + 7];
                     udp_checksum(dgraml - y, p + y,
                                              p + 12);
                     csum = (p[y + 6] << 8)
                          |  p[y + 7];
                     if (flag['v'-'a'])
                     printf("[UDP:%4.4x:%4.4x]\n", psum, csum);
                  }
                  else
                  {
                     p[y + 6] = 0;
                     p[y + 7] = 0;
                  }

                  break;

               case IPPROTO_ICMP:
                  icmp_checksum(dgraml - y, p + y);
                  break;

               case IPPROTO_TCP:
                  psum = (p[y + 16] << 8)
                       |  p[y + 17];

                  csum = tcp_checksum(dgraml - y, p + y,
                                                  p + 12);
                  if (flag['v'-'a'])
                  printf("[TCP:%4.4x:%4.4x]\n", psum, csum);

                  #ifdef INTEL
                  p[y + 16] = csum;
                  p[y + 17] = csum >> 8;
                  #else
                  p[y + 17] = csum;
                  p[y + 16] = csum >> 8;
                  #endif

                  break;
            }
         }

         if (flag['v'-'a'])
         {
            if (rxdata->preamble.protocol == IP & 65535)
            {
               y = ((*p) & 15) << 2;            /* IP header length             */
               dgraml = (p[2] << 8) | p[3];

               proto = p[9];

               bytes -= dgraml;
               if (bytes < 0) dgraml += bytes; /* if packet < datagram length ! */

               dgraml -= y;
               if (dgraml < 0) y += dgraml;

               printf("h[%d]\n", y);
               printf("d[%d]\n", dgraml);

               while (y--) printf("%2.2x", *p++);

               putchar('\n');

               switch (proto)
               {
                  case IPPROTO_ICMP:
                  case IPPROTO_UDP:
                     y = 8;
                     break;

                  case IPPROTO_TCP:
                     y = (p[12] >> 2) & 60;
                     break;

                  default:
                     y = dgraml;
               }

               dgraml -= y;
               if (dgraml < 0) y += dgraml;

               while (y--) printf("%2.2x", *p++);
               putchar('\n');
               if (dgraml > 0)
               {
                  y = 0;

                  while (dgraml--)
                  {
                     symbol = *p++;
                     printf("%2.2x", symbol);

                     showrow[y++] = symbol;

                     if (y & 16)
                     {
                        putchar('\t');
                        for (y = 0; y < 16; y++)
                        {
                           symbol = showrow[y];
                           if ((symbol < ' ') || (symbol > 126)) putchar('.');
                           else putchar(symbol);
                        }
                        putchar('\n');
                        y = 0;
                     }
                  }

                  if (y)
                  {
                     dgraml = y;
                     while (y++ < 16) printf("  ");
                     putchar('\t');

                     y = 0;

                     while (dgraml--)
                     {
                        symbol = showrow[y++];
                        if ((symbol < ' ') || (symbol > 126)) putchar('.');
                        else putchar(symbol);
                     }
                  }

                  putchar('\n');
               }
            }

            if (bytes > 0)
            {
               while (bytes--) printf("%2.2x", *p++);
               putchar('\n');
            }
         }
 
         rxdata->preamble.flag = FRAME;
         rxdata++;
         if (rxdata > &netdata->i[DEVICE_PAGES/2-1]) rxdata = netdata->i;
}

int main(int argc, char *argv[])
{
   int			 j,
			 physa_octets,
                         bytes,
   			 x,
			 y = 5;

   unsigned char	 data[4096];
   unsigned char	*p;


   unsigned char	 device[24],
			 netdevice[24],
			 addresses[240];

   unsigned char	 ipath1[24],
                         ipath2[24];

   int			 irules,
                         iphysa;

   pthread_attr_t        asyncb;
   pthread_t             asyncid;

   int			 fdes;


   argue(argc, argv);

   if (arguments == 0)
   {
      printf("\n\t       \tosserv /dev/bpfX:netdeviceY:addresses[:port][:udp][+addresses[:ports]"
	     "\n\t		[ /dev/bpfY:netdeviceZ:addresses...\n"
             "\n\t       \t 	  /dev/bpfZ:loopbackY:addresses...\n"
             "\n\tdon\'t opt -o for a software loopback device\n\n");

      return 0;
   }


   x = shmget('aaaa', DEVICE_PAGE * DEVICE_PAGES, IPC_CREAT);
   printf("shared core handle %d code %d size %d\n", x, errno,
           DEVICE_PAGE * DEVICE_PAGES);

   if (x < 0) return 0;

   netdata = shmat(x, NULL, 0);
   printf("shared core based @ %p code %d\n", netdata, errno);

   rxdata = netdata->i;
   txdata = netdata->o;

   rxdata->preamble.flag = 0;
   txdata->preamble.flag = 0;

   x = DEVICE_PAGES/2;
   while (x--) rxdata[x].preamble.flag = 0;

   x = DEVICE_PAGES/2;
   while (x--) txdata[x].preamble.flag = 0;


   for (x = 0; x < arguments; x++)
   {
      sscanf(argument[x], "%[^:]:%[^:]:%s", device, netdevice, addresses);

      sprintf(ipath1, "../temp.%s/filter", netdevice);
      sprintf(ipath2, "../temp.%s/physa", netdevice);

      s[x] = fdes = open(device, O_RDWR | O_NONBLOCK, 0777);
      printf("[s %d %d %s]\n", fdes, (fdes < 0) ? errno : 0, device);

      if (fdes < 0)
      {
      }
      else
      {
         b2b((char *) &sandl[x], netdevice);
         y = ioctl(fdes, BIOCSETIF, &sandl[x]);
         printf("[i %d %d %s]\n", y, (y < 0) ? errno : 0, netdevice);

         if (y < 0)
         {
         }
         else
         {
            p = (unsigned char *) &sandl;
            y = sizeof(struct ifreq);
            putchar('[');
            while (y--) printf("%2.2x", *p++);
            printf("]\n");
         }

         y = ioctl(fdes, BIOCIMMEDIATE, &one);
         printf("[i %d %d ]\n", y, (y < 0) ? errno : 0);

         y = ioctl(fdes, BIOCGBLEN, &maximum);
         printf("[L %d %d %d]\n", y, (y < 0) ? errno : 0, maximum);

         y = ioctl(fdes, BIOCGDLT, &j);
         printf("[LL %d %d]\n", y, (y < 0) ? errno : j);

         switch (j)
         {
            case DLT_NULL:
            case DLT_LOOP:
               rxdata->frame[1] = 4;
               physa_octets = 0;

               break;

            case DLT_EN10MB:
            case DLT_IEEE802:
               rxdata->frame[1] = 14;
               physa_octets = 6;

               y = ioctl(fdes, BIOCSSEESENT, &zero);
               printf("[> %d %d %d]\n", y, (y < 0) ? errno : 0, maximum);

               /********************************************************
			external device, switch no- see transmitted frames
			but for loopback you must see transmitted frames
			because you don't otherwise see received frames
               ********************************************************/

               break;

            default:
               rxdata->frame[1] = 14;
         }

         iftype[x] = j;
         rxdata->frame[0] = j;

         rxdata->frame[2] = 0;
         rxdata->frame[3] = 0;
         if (x) rxdata->frame[3] = 2;	/* within RTA1 machine
					   requeue output from this logical i/f
					   to logical i/f 2:0
					   which is managing the shared buffer
					   they can't all manipulate that
					*/

         rxdata->frame[8] = 32;		/* default mask size */

         sscanf(addresses, "%hhd.%hhd.%hhd.%hhd/%hhd", rxdata->frame + 4,
                                                       rxdata->frame + 5,
                                                       rxdata->frame + 6,
                                                       rxdata->frame + 7,
                                                       rxdata->frame + 8);

         rxdata->frame[9] = physa_octets;
         rxdata->preamble.frame_length = sizeof(i_f_string) + physa_octets;
         rxdata->preamble.ll_hl = LLHL;

         rxdata->preamble.i_f = PORT(x);

         rxdata->preamble.protocol = CONFIGURATION_MICROPROTOCOL;

         if (flag['v'-'a']) printf("[%d %s %s]\n", x, netdevice, addresses);
         portal(j, netdevice, addresses);
//         portal((x) ? 1 : 0, netdevice, addresses);

         irules = open(ipath1, O_RDONLY, 0777);
         printf("%d/%s\n", irules, ipath1);
         j = 0;
         bpfp[x].bf_insns = (struct bpf_insn *) heap;

         for (;;)
         {
            /********************************************************
		read assembled filter rules
		from ../temp.device/filter
            ********************************************************/

            y = read(irules, heap, 8);
            if (y < 8) break;
            j++;
            if (flag['v'-'a'])
            {
               printf("[:");
               for (y = 0; y < 8; y++) printf("%2.2x", heap[y]);
               printf("]\n");
            }
            heap += 8;
         }

         close(irules);
         bpfp[x].bf_len = j;

         y = ioctl(fdes, BIOCSETF, &bpfp[x]);
         printf("[SF %d]\n", y, (y < 0) ? errno : j);

         iphysa = open(ipath2, O_RDONLY, 0777);
         printf("%d/%s\n", iphysa, ipath2);
	 read(iphysa, rxdata->frame + 10, physa_octets);
         close(iphysa);

         rxdata->preamble.flag = FRAME;
         rxdata++;
      }
   }

   if (flag['h'-'a']) return 0;

   x = pthread_attr_init(&asyncb);

   if (x < 0) printf("threadcbinit %d e %d\n", x, errno);
   else
   {
      x = pthread_create(&asyncid, &asyncb, &async, NULL);
      if (x < 0) printf("async thread start %d %d\n", x, errno);
      else       printf("async thread ID %p\n", asyncid);
   }

   for (;;)
   {
      if (halt_flag < 0) break;

      for (x = 0; x < arguments; x++)
      {
         fdes = s[x];

         if (fdes < 0) continue;

         bytes = read(fdes, data, 4096);

         if (bytes < 0)
         {
            if (uflag['O'-'A']) putchar('.');
            if (errno == EAGAIN) continue;
            close(fdes);
            s[x] = -1;
	    printf("-%d", x);
            continue;
         }

         p = data;

         while (bytes > (18 + 8))
         {
            j = ((struct bpf_hdr *) p)->bh_hdrlen;
            y = ((struct bpf_hdr *) p)->bh_datalen;

	    if (j < 18)
            {
               printf("[1.%p/%p H%d F%d/%d]\n", data, p, j, y, bytes);
               break;
            }

            bytes -= j;
            p += j;

            if (y > bytes)
            {
               printf("[2.%p/%p H%d F%d/%d]\n", data, p, j, y, bytes);
               break;
            }

            if (flag['v'-'a']) printf("[(%d,%d)%d:%d-%d]\n[",
                                    y,
                                    ((struct bpf_hdr *) p)->bh_caplen,
                                    x, bytes, j);

            forward(x, p, y);

            p += y;
            bytes -= y;
         }
      }

      outputq();
      usleep(2000);

      /**********************************************************
		drive output here
		and a usleep() if the platform is heating
      **********************************************************/
   }

   for (x = 0; x < arguments; x++) close(s[x]);
   return 0;
}

