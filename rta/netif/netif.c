/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is part of the real-external-network-interaction
    of the software-emulation of the freeware processor architecture

                RTA1

    RTA1 is a free processor architecture design.

    The executable emulation of RTA1 is free software.

    Instruction code for the target RTA1 architecture is free software
    if it is delivered with this software

    Software programs delivered with this software to connect the
    emulated RTA1 with real network interfaces in the emulator host
    are free software

    Scripts and programs delivered with this software for running
    on other computers and interacting with the RTA1 are free software

    Scripts and utility programs for constructing RTA1 target
    executable software are free software

    You can redistribute and/or modify RTA1 and it materials
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RTA1 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with RTA1.  If not, see <http://www.gnu.org/licenses/>.

**********************************************************************/



#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/time.h>

#ifdef  PCAP
#define	PCAP_RX
#define	PCAP_TX
#undef	RULE_80
#include <pcap/pcap.h>
#define PCAP_BYTES      8192
#define PCAP_MS         20
#define	PCAP_HANDLES	3
#define	NETMASK32	(long) 0xFFFFFFFF
#endif

#define ASYNC

#ifdef  ASYNC
#include <pthread.h>
#endif

#include "../include.rta/argue.h"
#include "../netifx/sifr_mm.h"
#include "../rta.run/settings.h"

#define	MTU		16384
#define	TWARP		3300

#ifdef INTEL
#define	FRAME		0x0080
#define	IP		0x0008
#define LLHL		0x0000
#define TARGET_PORT     0x0012
#else
#define	FRAME		0x8000
#define	IP		0x0800
#define LLHL		0x0000
#define TARGET_PORT     0x1200
#endif

#define	CONFIGURATION_MICROPROTOCOL 0x6969

#ifdef	PCAP_RX
#ifndef	PCAP_TX
#if	defined(LINUX)
static struct sockaddr_in	remote
			= {     AF_INET              } ;
#elif	defined(OSX)
static struct sockaddr_in        remote
                        = { 16, AF_INET              } ;
#elif	defined(DOS)
static struct sockaddr_in	remote
			= {     AF_INET              } ;
#else
static struct sockaddr_in	remote;
#error	-DOSX or -DLINUX or /DDOS
#endif

static int			 one = 1;
#endif
#endif

#ifdef	RULE_80
#if	defined(LINUX)
static struct sockaddr_in        target
                        = {     AF_INET, TARGET_PORT } ;
#elif	defined(OSX)
static struct sockaddr_in        target
                        = { 16, AF_INET, TARGET_PORT } ;
#elif	defined(DOS)
static struct sockaddr_in	 target
			= {     AF_INET, TARGET_PORT } ;
static WSADATA			 wsad;
#else
static struct sockaddr_in	 target;
#error	-DOSX or -DLINUX or /DDOS
#endif
#endif


static mm_netdevice     *netdata;

static mm_netbuffer	*txdata;

static int		 alert_pid;
static int		 halt_flag;

static pcap_t               *sandl[PCAP_HANDLES];
static int                   ll_hl[PCAP_HANDLES];

/**********************************************

	reads stdin to allow option switching
	and clean halt if wished

**********************************************/

static void *async()
{
   char			 request[64];
   int			 symbol;
   int			 x, y;

   unsigned char	*p;

   for (;;)
   {
      p = fgets(request, 62, stdin);
      if (!p) continue;

      switch(request[0])
      {
         case '/':
            p++;

            while (symbol = *p++)
            {
               if ((symbol > 'A'-1) && (symbol < 'Z'+1)) symbol |= 'a'-'A';
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
   unsigned short		*p = (unsigned short *) icmp_message;
   unsigned long		 sum = *p++;
   unsigned short		 delivered = *p++;
   unsigned short		 carry;
   unsigned short		 x = (bytes + 1) >> 1;


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

   p = (unsigned short *) user_datagram;
   if (bytes & 1) user_datagram[bytes] = 0;
   p[3] = 0;

   while (x--) sum += *p++;

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

#define REVERSE(h) ((h >> 8) & 255) | ((h & 255) << 8)

display(int x, mm_netbuffer *p)
{
   if (x > (DEVICE_PAGES/2-1)) x = 0;
   if (x < 0) x = DEVICE_PAGES/2-1;

   p += x;

   #ifdef INTEL
   printf("%4.4hx:%4.4hx:%4.4hx:%4.4hx", REVERSE(p->preamble.flag),
                                         REVERSE(p->preamble.frame_length), 
                                         REVERSE(p->preamble.ll_hl),
                                         REVERSE(p->preamble.protocol));
   #else
   printf("%4.4hx:%4.4hx:%4.4hx:%4.4hx", p->preamble.flag,
                                         p->preamble.frame_length, 
                                         p->preamble.ll_hl,
                                         p->preamble.protocol);
   #endif
}

#define RADIX_MASK (DEVICE_PAGES/2-1)

static void oversee(mm_netbuffer *p, mm_netbuffer *q)
{
   struct timeval	 time;
   int			 x = q - p;


   gettimeofday(&time, NULL);

   if (p == netdata->i) printf("rx");
   if (p == netdata->o) printf("tx");

   printf("[%ld:%d]", time.tv_sec, time.tv_usec);

   putchar(10);

   printf(" %d-1 [", x);
   display(x-1, p);
   printf("] %d [", x);
   display(x, p);
   printf("] %d+1 [", x);
   display(x + 1, p);
   printf("]\n");
}

static void outputq()
{
   mm_netbuffer		*q = txdata;

   int			 x,
			 tag,
			 iphl,
                         bytes,
			 y;

   unsigned short	 psum, csum;

   unsigned char	*ipayload;
   unsigned short	*wpayload;

   char			 pbuffer[24];
   int			 k, symbol;

   unsigned short	 interface,
			 llhl,
                         tx_bytes;

   pcap_t		*s;

   while (tag = q->preamble.flag & FRAME)
   {
      x = (q->frame[2] << 8) | q->frame[3];

      #if	defined(PCAP_TX)
   
      #ifdef INTEL
      interface = REVERSE(q->preamble.interface);
      llhl = REVERSE(q->preamble.ll_hl);
      tx_bytes = REVERSE(q->preamble.frame_length);
      #else
      interface = q->preamble.interface;
      llhl = q->preamble.ll_hl;
      tx_bytes = q->preamble.frame_length;
      #endif

      if (flag['v'-'a']) printf("[%x:tx %x]\n", interface, tx_bytes);

      x = tx_bytes;
      s = sandl[0];
       
      y = pcap_sendpacket(s, q->frame, x);

      #else

      remote.sin_addr.s_addr = *((long *) ((char *) q->frame + 16)) ;
      if (flag['v'-'a'])
      {
         printf("[<-");
         ipayload = (unsigned char *) &remote;
         y = 16;
         while (y--) printf("%2.2x", *ipayload++);
         printf("]\n");
      }
      y = sendto(s, q->frame, x, 0, (struct sockaddr *) &remote, 16);
      if (y < 0) printf("send error %d\n", errno);

      #endif

      if (flag['v'-'a'])
      {
         #if 1
	 oversee(netdata->o, q);
         #endif

         printf("TX %d\n", y);

         if (flag['u'-'a'])
         {
            iphl = (q->frame[0] << 2) & 60;
            ipayload = q->frame + iphl;
            wpayload = (unsigned short *) ipayload;

            switch(q->frame[9])
            {
               case IPPROTO_UDP:
                  psum = wpayload[3];
                  csum = udp_checksum(x - iphl, ipayload, q->frame + 12);

		  #ifdef INTEL
                  csum = (csum>>8)|(csum<<8);
                  psum = (psum>>8)|(psum<<8);
                  #endif

                  printf("[%x:%x]\n", psum, csum);
                  break;
               case IPPROTO_TCP:
                  psum = wpayload[8];
                  csum = tcp_checksum(x - iphl, ipayload, q->frame + 12);

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

               while  (y < x)
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

            #if 0
            switch(q->frame[9])
            {
               case IPPROTO_UDP:
                  psum = wpayload[3]; 
                  csum = udp_checksum(x - iphl, ipayload, q->frame + 12);
                  printf("[%x:%x]\n", psum, csum);
                  break;
               case IPPROTO_TCP:
                  psum = wpayload[8];
                  csum = tcp_checksum(x - iphl, ipayload, q->frame + 12);
                  printf("[%x:%x]\n", psum, csum);
                  break;
            }
            #endif

            putchar('\n');
         }
      }

      q->preamble.flag = 0;
      q++;

      if (q > &netdata->o[DEVICE_PAGES / 2 - 1]) q = netdata->o;
   }

   txdata = q;
}


int main(int argc, char *argv[])
{
   #ifdef PCAP
   struct bpf_program	 bpf_p;
   struct pcap_pkthdr	 descriptor;
   #endif

   #ifndef PCAP_TX
   int			 s = socket(AF_INET, SOCK_RAW, IPPROTO_DIVERT);
   int			 j = (s < 0) ? errno : 0;
   #endif

   int			 x,
			 y,
			 dgraml,
			 proto,
			 textl,
			 physa_octets,
			 symbol;

   mm_netbuffer         *rxdata;

   unsigned short	 psum, csum;

   #ifdef PCAP

   unsigned char	 device_string[48],
			 network_string[48],
			 rule_string[240],
                         rule2[36],
                         report[12],
			 diagnostic_string[240];
   #else


   #endif

   unsigned char	 showrow[16];
   unsigned char	*p;

   #ifdef ASYNC

   pthread_attr_t        asyncb;
   pthread_t             asyncid;

   #endif

   #if	defined(PCAP_RX) && defined(PCAP_TX)
   #else
   if (s < 0) printf("socket error %d\n", j);
   #endif

   argue(argc, argv);

   #ifdef PCAP_RX

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
      y = sscanf(argument[x], "%[^:]:%[^:+\r\n]+%s", device_string, network_string, rule2);
      sandl[x] = pcap_open_live(device_string, PCAP_BYTES, 0, PCAP_MS, diagnostic_string);

      if (sandl[x])
      {
         textl = sprintf(rule_string, "dst host %s", network_string);

         if (y > 2)
         {
            strcpy(report, "8080");
            y = sscanf(rule2, "%[^:\r\n]:%s", network_string, report);
            textl += sprintf(rule_string + textl, 
                             " or ( tcp dst port %s and dst host %s )", report, network_string);
         }

         if (flag['v'-'a']) printf("[%s]\n", rule_string);

         y = pcap_compile(sandl[x], &bpf_p, rule_string, 0, NETMASK32);
         if (y < 0) printf("filter compile %s failed %s %s\n", device_string, rule_string, diagnostic_string);
         y = pcap_setfilter(sandl[x], &bpf_p);
         if (y < 0) printf("filter load %s failed %s\n", device_string, diagnostic_string);
         y = pcap_datalink(sandl[x]);
         printf("%s datalink type %d %s\n", device_string, y, pcap_datalink_val_to_name(y)); 

         switch (y)
         {
            case DLT_NULL:
            case DLT_LOOP:
               ll_hl[x] = 4;
               physa_octets = 0;

               break;

            case DLT_EN10MB:
            case DLT_IEEE802:
               ll_hl[x] = 14;
               physa_octets = 6;
               break;

            default:
               ll_hl[x] = 14;
         }

         rxdata->frame[0] = y;
         rxdata->frame[1] = ll_hl[x];

         rxdata->frame[6] = 32;

         sscanf(network_string, "%hhd.%hhd.%hhd.%hhd/%hhd", rxdata->frame + 2,
                                                            rxdata->frame + 3, 
                                                            rxdata->frame + 4,
                                                            rxdata->frame + 5,
                                                            rxdata->frame + 6);

         rxdata->frame[7] = physa_octets;
         rxdata->preamble.frame_length = 8 + physa_octets;
         rxdata->preamble.ll_hl = LLHL;

         #ifdef INTEL
         rxdata->preamble.interface = REVERSE(x);
         #else
         rxdata->preamble.interface = x;
         #endif

         rxdata->preamble.protocol = CONFIGURATION_MICROPROTOCOL;

         rxdata->preamble.flag = FRAME;
         rxdata++;
      }
      else printf("pcap handle %d not live %s %s\n", x, device_string, diagnostic_string);
   }


   #ifndef PCAP_TX
   y = setsockopt(s, SOL_SOCKET, SO_DONTROUTE, &one, 4);
   printf("soco DNR %d %d\n", y, (y < 0) ? errno : 0);
   #endif

   #endif


   #ifdef RULE_80
   y = fcntl(s, F_GETFL, 0);
   x = fcntl(s, F_SETFL, y | O_NONBLOCK);

   x = bind(s, (struct sockaddr *) &target, 16);
   printf("descriptor %d bind %d %x\n", s, x, y);
   #endif

   if (x < 0) printf("%d\n", errno);
   else
   {
      #ifdef ASYNC

      x = pthread_attr_init(&asyncb);

      if (x < 0) printf("threadcbinit %d e %d\n", x, errno);
      else
      {
         x = pthread_create(&asyncid, &asyncb, &async, NULL);
         if (x < 0) printf("async thread start %d %d\n", x, errno);
         else       printf("async thread ID %p\n", asyncid);
      }

      #endif

      for (;;)
      {
	 if (flag['p'-'a']) usleep(8000);
         if (halt_flag < 0) break;

         #ifdef PCAP_RX
         for (y = 0; y < arguments; y++)
         {
            p = pcap_next(sandl[y], &descriptor);

            if (p)
            {
               x = ll_hl[y];

               dgraml = descriptor.len;
               printf("[rx %d]\n", dgraml);

               #ifdef PCAP_BOUNCE
               pcap_sendpacket(sandl[0], p, dgraml);
               #endif

               dgraml -= x;

               #define PCAP_RXDISPLAY
               #ifdef PCAP_RXDISPLAY
               if (flag['v'-'a'])
               {
                  putchar('[');
                  while (x--) printf("%2.2x", *p++);
                  printf("]\n");
               }
               else p += x;
               #endif

               memcpy(rxdata->frame, p, dgraml);
               x = dgraml;
               break;
            }
         }

         if (y < arguments)
         {
         }
         else
         {
            #if 1
            outputq();
            #else
            outputq(s);
            #endif
            usleep(TWARP);
            x = 0;
         }

         #endif

         #ifdef RULE_80
         if (x == 0)
         {
            x = recv(s, rxdata->frame, MTU, 0);

            if (x < 0)
            {
               if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
               {
                  outputq(s);
                  usleep(TWARP);
                  x = 0;
               }
               else break;
            }
         }
         #endif

         if (x)
         {
            #ifdef INTEL
            rxdata->preamble.frame_length = (x << 8) | (x >> 8);
            #else
            rxdata->preamble.frame_length = x;
            #endif

            rxdata->preamble.ll_hl = LLHL;
            #ifdef INTEL
            rxdata->preamble.interface = REVERSE(y);
            #else
            rxdata->preamble.interface = y;
            #endif
            rxdata->preamble.protocol = IP;

            rxdata->preamble.flag = FRAME;


            if (flag['z'-'a'])
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


               y = ip_checksum((unsigned short *) rxdata->frame);

               switch (rxdata->frame[9])
               {
                  case IPPROTO_UDP:
                     if (flag['x'-'a'])
                     {
                        psum = (rxdata->frame[y + 6] << 8)
                             |  rxdata->frame[y + 7];
                        udp_checksum(x - y, rxdata->frame + y,
                                            rxdata->frame + 12);
                        csum = (rxdata->frame[y + 6] << 8)
                             |  rxdata->frame[y + 7];
                        if (flag['v'-'a'])
                        printf("[UDP:%4.4x:%4.4x]\n", psum, csum);
                     }
                     else
                     {
                        rxdata->frame[y + 6] = 0;
                        rxdata->frame[y + 7] = 0;
                     }

                     break;

                  case IPPROTO_ICMP:
                     icmp_checksum(x - y,rxdata->frame + y);
                     break;

                  case IPPROTO_TCP:
                     psum = (rxdata->frame[y + 16] << 8)
                          |  rxdata->frame[y + 17];

                     csum = tcp_checksum(x - y, rxdata->frame + y,
                                                rxdata->frame + 12);
                     if (flag['v'-'a'])
                     printf("[TCP:%4.4x:%4.4x]\n", psum, csum);

                     #ifdef INTEL
                     rxdata->frame[y + 16] = csum;
                     rxdata->frame[y + 17] = csum >> 8;
                     #else
                     rxdata->frame[y + 17] = csum;
                     rxdata->frame[y + 16] = csum >> 8;
                     #endif

                     break;
               }
            }

            if ((flag['v'-'a']) && (x))
            {
               p = rxdata->frame;

               y = ((*p) & 15) << 2;		/* IP header length		*/
               dgraml = (p[2] << 8) | p[3];

               proto = p[9];

               x -= dgraml;
               if (x < 0) dgraml += x;		/* if packet < datagram length ! */

               dgraml -= y;
               if (dgraml < 0) y += dgraml;

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

               if (x > 0)
               {
                  while (x--) printf("%2.2x", *p++);
                  putchar('\n');
               }
            }

            if (flag['w'-'a'])
            {
               oversee(netdata->i, rxdata);
            }

            rxdata++;
            if (rxdata > &netdata->i[DEVICE_PAGES/2-1]) rxdata = netdata->i;
         }
      }
   }

   #ifdef RULE_80
   if (x < 0) printf("xfinal1 %d E %d\n", x, errno);
   #endif

   #if 	defined(PCAP_RX) || defined(PCAP_TX)
   for (x = 0; x < arguments; x++)
   {
      if (sandl[x]) pcap_close(sandl[x]);
   } 
   #endif

   #ifdef RULE_80
   x = close(s);
   if (x < 0) printf("xfinal2 %d E %d\n", x, errno);
   #endif
   return 0;
}

