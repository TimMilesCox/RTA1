
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef  LINUX
#define DLT_NULL        0
#define DLT_EN10MB      1
#include <linux/if_arp.h>
#include <linux/bpf.h>
#include <linux/filter.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#ifdef	INTEL
#define	EVERYTHING ETH_P_ALL << 8
#else
#define	EVERYTHING ETH_P_ALL
#endif
#endif
#ifdef	OSX
#include <net/bpf.h>
#include <net/if.h>
#include <net/if_dl.h>
#endif

#include <netinet/in.h>

#include <fcntl.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>

#include "../include.rta/argue.h"
#include "../netifx/ifconfig.h"
#include "../rta.run/settings.h"
#include "../include.rta/address.h"
#include "../netifx/sifr_mm.h"
#include "../include.rta/physa.h"
#include "../portal/portal.h"

#define	RED		"\033[38;5;9m"
#define	PRIMARY		"\033[0m"
#define	LLHL		0
#define	MASKS		120

#define	TRAFFIC_VOLUME

static int			 iftype[INTERFACES];
static int			 s[INTERFACES];
static struct ifreq		 sandl[INTERFACES];
static char			 refresh_phya[INTERFACES][8];

#ifdef	LINUX
static struct sock_fprog	 bpfp[INTERFACES];
static struct sockaddr_ll	 rxtxa[INTERFACES];
static int			 twenty = sizeof(struct sockaddr_ll);
unsigned char			 netdevice[24];
#endif

#ifdef	OSX
static struct bpf_program	 bpfp[INTERFACES];
static struct sockaddr_dl	 rxtxa[INTERFACES];
static unsigned int		 one = 1;
static unsigned int		 zero;
unsigned char			 device[24];
unsigned char			 netdevice[24];
#endif

#ifdef  REVISION1
unsigned char			*addresses;
#else
unsigned char			 addresses[ADDRESS_STRING];
#endif

static unsigned int		 maximum;
static int			 halt_flag;

unsigned int			 i_f_first_address;

/**********************************************
	shared memory r/w cursors
**********************************************/

static mm_netdevice     *netdata;
static mm_netbuffer	*rxdata;
static mm_netbuffer     *txdata;

/**********************************************
	filter rules and assignment pointer
**********************************************/

static unsigned char     instruction[MASKS][8];
static unsigned char	*heap = instruction[0];


/**********************************************

	async()
        reads stdin to allow option switching
        and clean halt if wished

**********************************************/

static int			 console_handle;

#ifdef	OSX
static struct sockaddr_in	 console_sockf = { 16, AF_INET, 0,	 { 0	 } } ;
static struct sockaddr_in	 console_socka = { 16, AF_INET, USTDOUT, { LOCAL } } ;
#endif

#ifdef	LINUX
static struct sockaddr_in	 console_sockf = { AF_INET, 0,		 { 0     } } ;
static struct sockaddr_in	 console_socka = { AF_INET, USTDOUT,	 { LOCAL } } ;
#endif

static struct timeval		 xronos;

static int			 rest_granule = REST_GRANULE;


static int			 net_granule = NET_GRANULE;
static int			 net_revision;

static unsigned short udp_checksum(int bytes, char *user_datagram, char *net_addresses);
static unsigned short tcp_checksum(int bytes, char *tcp_segment, char *net_addresses);

static void restart();

#ifdef	OSX
static void diagnose(unsigned char *buffer_start,
                     unsigned char *scan_point,
                     int header_bytes, int packet_bytes, int buffer_bytes);
#endif

static void *async()
{
   char                  request[72];
   int                   symbol;
   int                   x, y;

   unsigned char        *p;

   for (;;)
   {
      p = fgets(request, 60, stdin);
      if (!p) continue;

      switch(request[0])
      {
         case '-':
            p++;

            while ((symbol = *p++))
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
            if (flag['v'-'a']) printf("service %c\n", *p);
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

         case '<':
            x = sscanf(request + 1, "%d %d", &net_granule, &rest_granule);

            if ((x < 1) || (net_granule < 0) || (net_granule > NET_GRANULE))
               net_granule = NET_GRANULE;

            if (x > 1)
            {
               if ((rest_granule < 5000) || (rest_granule > REST_GRANULE))
               rest_granule = REST_GRANULE;
            }

            printf("frame sample %dµs / %dµs\n", net_granule, rest_granule);

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

   struct timeval	 txtime;
   int			 interface_type;
   unsigned short	 protocol;

   if (q->preamble.flag & FRAME)
   {
      /*********************************************************
		reset the usleep on idle interval
      *********************************************************/

      net_revision = 0;

      protocol = q->preamble.protocol;

      if (protocol == CONFIGURATION_MICROPROTOCOL)
      {
         printf("\n\nRESET REQUEST FROM RTA1\n\n");
         restart();
         return;
      }

      interface = PORT(q->preamble.i_f);
      interface_type = iftype[interface];

      switch (interface_type)
      {
         case DLT_NULL:
            #ifdef LINUX
            llhl = 14;
            #endif
            #ifdef OSX
            llhl = 4;
            #endif
            break;

         default:
            llhl = 14;
      }

      dgram = q->frame + llhl;

      tx_bytes = PORT(q->preamble.frame_length);
      dgraml = tx_bytes - llhl;

      if (flag['v'-'a'])
      {
         if (protocol == ARP)
         {
            printf("[%x:tx %x / %x %x %x %x %x ]\n", interface, tx_bytes,
            q->preamble.flag, q->preamble.frame_length,q->preamble.ll_hl,
            q->preamble.i_f, protocol);
            for (x = 0; x < 42; x++) printf("%2.2x", q->frame[x]);
            putchar(10);
         }
         else printf("[%x:tx %x]\n", interface, tx_bytes);
      }

      x = tx_bytes;

      fdes = s[interface];

      p = q->frame;

      #ifdef OSX
      if (interface_type == DLT_NULL)
      {
         /*********************************************************
            DLT_NULL is changed to ethernet for Linux
            only OSX gets in here
         *********************************************************/
         p += 4;
         x -= 4;
      }
      #endif

      y = write(fdes, p, x);

      if (flag['o'-'a'])
      {
         iphl = (*dgram << 2) & 60;
         ipayload = dgram + iphl;
         wpayload = (unsigned short *) ipayload;

         if (wpayload[1] == USTDOUT)
         {
            bytes = sendto(console_handle, dgram, dgraml, 0,
                      (struct sockaddr *) &console_socka, 16);
            if (bytes < 0) printf("[<-E%d]\n", errno);
         }

      }

      if (flag['v'-'a'])
      {
         if (uflag['W'-'A'])
         {
            gettimeofday(&txtime, NULL);
            printf("[%d:%3.3d]", txtime.tv_sec % 86400, txtime.tv_usec / 1000);
         }

         printf("TX %d\n", y);
         if (y < 0) printf("%d:%d\n", errno, fdes);

         if ((x) && (flag['u'-'a']) && (protocol == IP))
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
            putchar('\n');
         }
      }

      q->preamble.flag = 0;
      q++;

      if (q > &netdata->o[DEVICE_PAGES / 2 - 1]) q = netdata->o;
   }

   else
   {
      if (net_revision < 50000) net_revision += net_granule;
      if (flag['y'-'a']) printf("[%d]\n", net_revision);
      usleep(net_revision);
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
   unsigned int		 sum = 0;
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
   unsigned int			 sum = *p++;
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

   unsigned int			 sum = bytes + IPPROTO_UDP;
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

   unsigned int			 sum = bytes + IPPROTO_TCP;
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

   while ((symbol = *from++)) *to++ = symbol;
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
			 symbol,
			 interface_type = iftype[x];

   unsigned short	 csum, psum;
   unsigned short	 e_proto;

   unsigned char	 showrow[16];

   if (rxdata->preamble.flag) return;

         switch (interface_type)
         {
            case DLT_NULL:
               #ifdef LINUX
               ll_hl = 14;
               #endif
               #ifdef OSX
               ll_hl = 4;
               #endif

               e_proto = IP;
               break;

            case DLT_EN10MB:
               ll_hl = 14;
               e_proto = *((unsigned short *) ((unsigned char *) p + 12));
               break;

            default:
               ll_hl = 14;
               e_proto = *((unsigned short *) ((unsigned char *) p + 12));
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

         if ((bytes > (2048 - 10))
         ||  ((uflag['K'-'A'] == 0) && (e_proto ^ IP) && (e_proto ^ ARP)))
         {
            if (flag['k'-'a']) printf("[* %4.4x:%x]\n", PORT(e_proto), bytes);
            return;
         }

         rxdata->preamble.protocol = e_proto;
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

         if (((flag['z'-'a']) && (rxdata->preamble.protocol == (IP & 65535)) && (interface_type == DLT_NULL))
         ||  ((uflag['Z'-'A']) && (rxdata->preamble.protocol == (IP & 65535))))
         {
            /************************************************

                OSX on loopback writes zero IP checksum

                and header part of UDP checksum (neither
                zero nor the correct UDP checksum)

                TCP checksum is also junk on loopback

                although ICMP checksum is always correct

                so execute with -z option on OSX loopback

                big -Z option checks and corrects
		incoming checksums on all interface types

                option -x regenerates UDP checksums
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
            if (rxdata->preamble.protocol == (IP & 65535))
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

         net_revision = 0;
}

unsigned char *peel(unsigned char *p, unsigned char *q)
{
   int		 symbol,
		 limit = ADDRESS_STRING - 1;

   while ((symbol = *q))
   {
      limit--;

      if (limit < 0)
      {
         /*****************************************************
		the limit is very large
		so that just-in-time assembly can do
		names as well as addresses 
         *****************************************************/

         q = "";
         break;
      }

      if (symbol == '+') break;
      if (symbol == '@') break;

      *p++ = symbol;
      q++;
   }

   *p = 0;
   return q;
}

#define	IFCONFIG_OCTETS (sizeof(i_f_string) + 24)

static void configure_interface(int i_f, int physa_octets, char *address)
{
   char			*component2 = strchr(address, ':');

   char			 protocol_name[4] = "   \0";
   int			 port = 0;
   int			 ports = 1;
   int			 density = 4096 / 24 - 1;

   unsigned short	*upconf = (unsigned short *)
                                 ((char *) rxdata->frame  + IFCONFIG_OCTETS);
                                                            

   if (component2) *component2 = 0;
   memset(rxdata->frame + 4, 0, 48 - 10 - 4);

   sscanf(address, "%hhd.%hhd.%hhd.%hhd/%hhd", rxdata->frame + 4,
                                               rxdata->frame + 5,
                                               rxdata->frame + 6,
                                               rxdata->frame + 7,
                                               rxdata->frame + 8);

   rxdata->frame[9] = physa_octets;
   rxdata->preamble.frame_length = PORT((sizeof(i_f_string) + physa_octets));
   rxdata->preamble.protocol = CONFIGURATION_MICROPROTOCOL;

   if (flag['v'-'a']) printf("[%d %s %s]\n", i_f, netdevice, address);

   memcpy(rxdata->frame + 10, refresh_phya + i_f, physa_octets);

   upconf[0] = 0;	/* transmission protocol config */
   upconf[1] = 0;	/* default = none		*/
   upconf[2] = 0;
   upconf[3] = 0;

   if (component2)
   {
      /**********************************************************
		transmission protocol configuration
		for large traffic
      **********************************************************/

      component2++;

      if (flag['v'-'a']) printf("[%s]\n", component2);

      port = 0;
      ports = 0;
      density = 0;

      sscanf(component2, "%d/%3s/%d/%d", &port, protocol_name, &ports, &density);
      printf("[%x.%s.%x.%x]\n", port, protocol_name, ports, density);
// rxdata->preamble.frame_length = PORT(IFCONFIG_OCTETS + 8 + 30);
      if ((port > 3072)
      &&  (ports > 0)
      && ((port + ports) < 65537))
      {
//         printf("%s\n", protocol_name);
         if (memcmp(protocol_name, "udp", 4) == 0) upconf[0] = PORT(17);
         if (memcmp(protocol_name, "tcp", 4) == 0) upconf[0] = PORT(6);
//         printf("%x %x\n", rxdata->frame[IFCONFIG_OCTETS+1], upconf[0]);

         if (upconf[0])
         {
            /****************************************************
		at least 3 of 4 fields = these are
		configuration parameters forwarded to RTA1
		for high-capacity TCP ports

		if just a port number and protocol
		those are assembled into the packet filter 
            ****************************************************/

            upconf[1] = PORT(port);
            upconf[2] = PORT(ports);
            upconf[3] = PORT(density);
            rxdata->preamble.frame_length = PORT((IFCONFIG_OCTETS+8));
         }
      }
   }
}

static void configure_route(int i_f, char *address)
{
   char			 symbol = 0;
   unsigned char	*next_hop_location = rxdata->frame + 8;
   unsigned int		*next_hop = (unsigned int *) next_hop_location;

   rxdata->frame[8] = 0;
   rxdata->frame[9] = 0;
   rxdata->frame[10] = 0;
   rxdata->frame[11] = 0;

   rxdata->frame[2] = 0;	/* upper half big endian mask width */


   sscanf(address, "%hhd.%hhd.%hhd.%hhd/%hhd%c%hhd.%hhd.%hhd.%hhd", 
                                                 rxdata->frame + 4,
                                                 rxdata->frame + 5,
                                                 rxdata->frame + 6,
                                                 rxdata->frame + 7,
                                                 rxdata->frame + 3,
                                                           &symbol,
                                                 rxdata->frame + 8,
                                                 rxdata->frame + 9,
                                                 rxdata->frame + 10,
                                                 rxdata->frame + 11);
  
   rxdata->frame[1] = i_f;
   rxdata->frame[0] = 0;

   if (symbol == '*') rxdata->frame[0] = 128;
   else *next_hop = i_f_first_address;

   rxdata->preamble.frame_length = PORT(sizeof(route_microstring));
   rxdata->preamble.protocol = ROUTE_CONFIGURATION_MICROPROTOCOL;

   if (flag['v'-'a']) printf("[%d %s %s ->]\n", i_f, netdevice, address);
}

static void deliver_configuration_tuple(int i_f)
{
   char			*p = (unsigned char *) rxdata;

   unsigned short	 bytes = sizeof(mm_descriptor)
			 +  PORT(rxdata->preamble.frame_length);

   rxdata->preamble.ll_hl = LLHL;
   rxdata->preamble.i_f = PORT(i_f);
   rxdata->preamble.flag = FRAME;
   rxdata++;

   printf("[<-");
   while (bytes--) printf("%2.2x", *p++);
   printf("]\n");
}

static void configure(int x, int j)
{
   /*********************************************************************
	j == 0 == ARPHRD_LOOPBACK
	for Linux we soft-changed ARPHRD_LOOPBACK to ARPHRD_ETHER
	because openSUSE uses quasi-ethernet link encapsulation
	but RTA1 should not generate any ARP frames

	any ARPHRD_LOOPBACK i/f here is the emulator platform loopback
	which for RTA1 is an external point2point link

	OSX is not as openSUSE and has loopbank link encapsulation
	totalling value AF_INET in 4 bytes platform-endian

	so far we have only done ARPHRD_LOOPBACK and ARPHRD_ETHER
   *********************************************************************/

   int			 physa_octets = (j) ? 6 : 0;
   int			 symbol,
			 symbols;

   unsigned char	*p,
			*q = rxdata->frame + 4;

   unsigned int		*initial_address = (unsigned int *) q;

   unsigned char	 address[ADDRESS_STRING];

   rxdata->frame[0] = j;

   #ifdef LINUX
   rxdata->frame[1] = 14;
   #endif

   #ifdef OSX
   rxdata->frame[1] = (j) ? 14 : 4;
   #endif

   rxdata->frame[2] = 0;
   rxdata->frame[3] = 0;
   if (x) rxdata->frame[3] = 2;   /* within RTA1 machine
                                     requeue output from this logical i/f
                                     to logical i/f 2:0
                                     which is managing the shared buffer
                                     they can't all manipulate that
                                  */

   rxdata->frame[8] = 32;         /* default mask size */

   #ifdef REVISION1

   /************************************************************
	lift the limit on argument size
	by marking the start of the big field not storing it

	correspondingly addresses is a pointer not new array
   ************************************************************/

   #ifdef LINUX
   sscanf(argument[x], "%[^:]:%n", netdevice, &symbols);
   #endif

   #ifdef OSX
   sscanf(argument[x], "%[^:]:%[^:]:%n", device, netdevice, &symbols);
   #endif

   addresses = argument[x] + symbols;

   #else

   #ifdef LINUX
   sscanf(argument[x], "%[^:]:%s", netdevice, addresses);
   #endif

   #ifdef OSX
   sscanf(argument[x], "%[^:]:%[^:]:%s", device, netdevice, addresses);
   #endif

   #endif

   q = peel(address, addresses);
   configure_interface(x, physa_octets, address);
   i_f_first_address = *initial_address;
   deliver_configuration_tuple(x);

   for (;;)
   {
      symbol = *q++;
      if ((symbol ^ '+') && (symbol ^ '@')) break;
      q = peel(address, q);

      switch (symbol)
      {
         case '+':

            /***************************************************************
		this is not the primary network address on this interface
		so suppress reprogramming of the physical information
            **************************************************************/

            rxdata->frame[0] = 0;
            rxdata->frame[1] = 0;

            rxdata->frame[2] = 0;
            rxdata->frame[3] = 0;

            if (x) rxdata->frame[3] = 2;
            rxdata->frame[8] = 32;
            
            configure_interface(x, physa_octets, address);
            break;

         case '@':
            configure_route(x, address);
      }

      deliver_configuration_tuple(x);
   }
}

static void restart()
{
   int		 x,
		 j;

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
      #ifdef LINUX
      j = DLT_EN10MB;
      #elif defined(OSX)
      j = iftype[x];
      #else
      #error	we only do LINUX or OSX so far
      #endif

      configure(x, j);
   }

   net_revision = 0;
}

int main(int argc, char *argv[])
{
   static int		 trunk_key = 'aaaa';

   int			 j,
			 physa_octets,
                         bytes,
                         symbol,
   			 x,
			 y = 5;

   unsigned char	 data[4096];
   unsigned char	*p,
			*q;

   unsigned char	 ipath1[84],
                         ipath2[84];
 
   int			 irules,
                         iphysa;

   pthread_attr_t        asyncb;
   pthread_t             asyncid;

   int			 fdes;
   int			 lanes = 1;

   #ifdef LINUX
   struct sockaddr_ll	*rxtx;
   #endif
   #ifdef OSX
   struct sockaddr_dl	*rxtx;
   #endif

   argue(argc, argv);

   if ((flag['h'-'a'])
   ||  (arguments == 0))
   {
      #ifdef LINUX
      printf("\n\n\tlinuxnet command line is generated from addresses in $HOME/.rta_net\n");
      #endif

      #ifdef OSX
      printf("\n\n\tosserv command line is generated from addresses in $HOME/.rta_net\n");
      #endif

      printf("\tdetails in http://172.29.7.7/connect.html#route\n\n");

      printf("\tscript ./runL [-options]\n\n");
      printf("\t-v\t display received frame contents plus:\n\n");

      printf("\t-u\ttransmitted frame contents\n");
      printf("\t-W\ttransmitted frame timestamps\n\n");

      #ifdef OSX
      printf("\t-V\tdisplay BPF metadata anomalies on receive\n\n");
      #endif

      printf("\t-z\tverify and correct checksums received over platform loopback\n");
      printf("\t-Z\tverify and correct checksums received on all interfaces\n");
      printf("\t-x\tcorrect instead of zeroing wrong UDP checksums received\n\n");

      printf("\t-h\tdisplay this information and exit\n\n");
      return 0;
   }

   #ifdef TRAFFIC_VOLUME
   if (uflag['A'-'A']) lanes = 8;
   if (uflag['B'-'A']) lanes = 8 * 2;
   if (uflag['C'-'A']) lanes = 8 * 3;
   if (uflag['D'-'A']) lanes = 8 * 4;
   if (uflag['E'-'A']) lanes = 8 * 5;
   if (uflag['F'-'A']) lanes = 8 * 6;
   if (uflag['G'-'A']) lanes = 8 * 7;
   if (uflag['H'-'A']) lanes = 8 * 8;
   if (uflag['I'-'A']) lanes = 8 * 9;
   if (uflag['J'-'A']) lanes = 8 * 10;
   #endif

   if (flag['a'-'a']) trunk_key = 'aaaz';

   x = shmget(trunk_key, DEVICE_PAGE * DEVICE_PAGES * lanes, IPC_CREAT);
   printf("shared core handle %d code %d size %d\n", x, errno,
           DEVICE_PAGE * DEVICE_PAGES * lanes);

   if (x < 0) return 0;

   netdata = shmat(x, NULL, 0);
   printf("shared core based @ %p code %d\n", netdata, errno);

   /*******************************************************************
	bind unconditionally to relay to console broadcast receiver
	so option -o may be switched on during running
	if console broadcast receiver is not receiving
   *******************************************************************/

   console_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (console_handle < 0) printf("console supplementary relay not assigned %d\n", errno);
   else
   {
      x = bind(console_handle, (struct sockaddr *) &console_sockf, 16);
      if (x < 0) printf("console supplementary relay not bound %d\n", errno);
      else printf("console supplementary relay\n");
   }

   for (x = 0; x < arguments; x++)
   {
      p = argument[x];

      #ifdef REVISION1
   
      /************************************************************
           lift the limit on argument size
           by marking the start of the big field not storing it

           correspondingly addresses is a pointer not new array
      ************************************************************/

      #ifdef LINUX
      sscanf(argument[x], "%[^:]:%n", netdevice, &bytes);
      #endif

      #ifdef OSX
      sscanf(argument[x], "%[^:]:%[^:]:%n", device, netdevice, &bytes);
      #endif

      addresses = argument[x] + bytes;

      #else

      if (strlen(p) > (ASTRING - 1))
      {
         p[ASTRING - 1] = 0;
         printf("argument %x limited:%s\n", x, p);         
      }

      #ifdef LINUX
      sscanf(p, "%[^:]:%s", netdevice, addresses);
      #endif
      #ifdef OSX
      sscanf(p, "%[^:]:%[^:]:%s", device, netdevice, addresses);
      #endif

      #endif	/* REVISION1 */

      if (flag['a'-'a'])
      {
         #ifdef ANET
         sprintf(ipath1, ANET "%s/filter", netdevice);
         sprintf(ipath2, ANET "%s/physa", netdevice);
         #else
         sprintf(ipath1, "../atemp.%s/filter", netdevice);
         sprintf(ipath2, "../atemp.%s/physa", netdevice);
         #endif
      }
      else
      {
         #ifdef NET
         sprintf(ipath1, "%s%s/filter", NET, netdevice);
         sprintf(ipath2, "%s%s/physa", NET, netdevice);

	 printf("%s\n\n", ipath1);
         printf("%s\n\n", ipath2);
         #else
         sprintf(ipath1, "../temp.%s/filter", netdevice);
         sprintf(ipath2, "../temp.%s/physa", netdevice);
         #endif
      }

      #ifdef LINUX
      s[x] = fdes = socket(AF_PACKET, SOCK_RAW, ETH_P_ALL);
      printf("[s %d %d %s]\n", fdes, (fdes < 0) ? errno : 0, netdevice);
      #endif
      #ifdef OSX
      s[x] = fdes = open(device, O_RDWR | O_NONBLOCK, 0777);
      printf("[s %d %d %s %s]\n", fdes, (fdes < 0) ? errno : 0, device, netdevice);
      #endif
      
      if (fdes < 0)
      {
         #ifdef OSX
         printf(RED
               "\n%s FILE OPEN FAILURE. USE ANOTHER bpf FILE"
		PRIMARY	" ERROR %d\a\n\n", device, errno);
         #endif

         #ifdef LINUX
         printf(RED
               "\n%s BPF SOCKET OPEN FAILURE"
                PRIMARY " ERROR %d\a\n\n", netdevice, errno);
         #endif

         halt_flag = -1;
         break;

         /*************************************************************
		easier to see and fix the problem
		if no more initialisation is done
         *************************************************************/
      }
      else
      {
         b2b((char *) &sandl[x], netdevice);

         rxtx = &rxtxa[x];
         /*************************************************************************
             ifidxa() copies sockaddr_ll from the kernel
         *************************************************************************/

         ifidxa(netdevice, rxtx);

         #ifdef OSX
         y = ioctl(fdes, BIOCSETIF, &sandl[x]);
         printf("[i %d %d %s]\n", y, (y < 0) ? errno : 0, netdevice);

         printf("%p %p\n", netdevice, rxtx);
         printf("%s\n", netdevice);
         printf("return link_addr\n");

         if (rxtx->sdl_family ^ AF_LINK)
         {
            printf("%x not LL tuple\n", rxtx->sdl_family);
         }
         else
         {
            printf("[%x] af %x if %x type %x nbytes %x abytes %x sebytes %x:"
                   "%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x"
                   "%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x\n",
                    rxtx->sdl_len, rxtx->sdl_family, rxtx->sdl_index,
                    rxtx->sdl_type, rxtx->sdl_nlen, rxtx->sdl_alen, rxtx->sdl_slen,
                    rxtx->sdl_data[0], rxtx->sdl_data[1], rxtx->sdl_data[2],
                    rxtx->sdl_data[3], rxtx->sdl_data[4], rxtx->sdl_data[5],
                    rxtx->sdl_data[6], rxtx->sdl_data[7], rxtx->sdl_data[8],
                    rxtx->sdl_data[9], rxtx->sdl_data[10],rxtx->sdl_data[11]);
         }

         #endif

         #ifdef LINUX
	 rxtx->sll_protocol = EVERYTHING;

	 y = bind(fdes, (struct sockaddr *) rxtx, sizeof(struct sockaddr_ll));

         if (y < 0)printf("bind wrong %d\n", errno);

	 printf("[af:%4.4x p:%4.4x ifidx:%8.8x haspa:%4.4x ptyp:%2.2x PhyAL %x::%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x]\n",
                 rxtx->sll_family, rxtx->sll_protocol, rxtx->sll_ifindex, rxtx->sll_hatype, rxtx->sll_pkttype,
                 rxtx->sll_halen, rxtx->sll_addr[0], rxtx->sll_addr[1], rxtx->sll_addr[2], rxtx->sll_addr[3], rxtx->sll_addr[4], rxtx->sll_addr[5]);


	 printf("[i %d %d %s]\n", y, (y < 0) ? errno : 0, netdevice);
         #endif

         if (y < 0)
         {
         }
         else
         {
            p = (unsigned char *) &sandl[x];
            y = sizeof(struct ifreq);
            putchar('[');
            while (y--) printf("%2.2x", *p++);
            printf("]\n");
         }

         #ifdef LINUX

         /***********************************************************
	    LINUX platform loopback frames are Ethernet-shaped
	    which makes all frames Ethernet-shaped currently
	    RTA1 system image must must build 14-byte LL header
	    with 0800 protocol field when sending
	    but not do any ARP when its own interface physa is zero
	 ***********************************************************/

	 j = rxtx->sll_hatype;

	 switch (j)
	 {
	    case ARPHRD_LOOPBACK:
               printf("loopback detected\n");
               
	       j = DLT_NULL;
               rxtx->sll_protocol = DLT_NULL;

	       /*****************************************************

		  change loopback to soft ethernet

                  distros openSUSE loopback have link encapsulation
		  as ethernet with address 00:00:00:00:00:00

	          zero local address in case the Linux distro did not
		  it tells RTA1 system image on startup
		  packets are like ethernet with 0800
		  but suppress ARP. Do two zero addresses

                  if other distros don't have that encapsulation
		  for loopback, it will be necessary to find out
		  how the link encapsulation should be

	       *****************************************************/

	       rxtx->sll_addr[0] = 0;
               rxtx->sll_addr[1] = 0;
               rxtx->sll_addr[2] = 0;
               rxtx->sll_addr[3] = 0;
               rxtx->sll_addr[4] = 0;
               rxtx->sll_addr[5] = 0;

               physa_octets = 6;
	       break;

	    case ARPHRD_ETHER:
               printf("enet detected\n");
               physa_octets = 6;
	       break;

	    default:
	       printf("interface %d new type: please add case %d:\n", x, j);
               continue;
	 }

         #endif

         #ifdef OSX

	 /***********************************************************
	    do the OSX things instead of Linux
	 ***********************************************************/

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
               printf("loopback detected\n");
               physa_octets = 0;

               break;

            case DLT_EN10MB:
            case DLT_IEEE802:
               printf("enet detected\n");
               physa_octets = 6;

               break;

            default:
               printf("interface %d new type: please add case %d:\n", x, j);
               continue;
         }

         #endif		/*	Linux or OSX	*/

         iftype[x] = j;

         if (flag['v'-'a']) printf("[%d %s %s]\n", x, netdevice, addresses);
         portal(j, netdevice, addresses);

         irules = open(ipath1, O_RDONLY, 0777);
         printf("%d/%s\n", irules, ipath1);
         j = 0;

         #ifdef LINUX
         bpfp[x].filter = (struct sock_filter *) heap;
         #endif
         #ifdef OSX
         bpfp[x].bf_insns = (struct bpf_insn *) heap;
         #endif

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

         #ifdef LINUX
         bpfp[x].len = j;
         
         printf("socket %d fprog %4.4x:%p, \n", fdes, bpfp[x].len, bpfp[x].filter);
         y = setsockopt(fdes, SOL_SOCKET, SO_ATTACH_FILTER, &bpfp[x], sizeof(struct sock_fprog));
         printf("[socopt %d %d %s]\n", y, (y < 0) ? errno : 0, netdevice);


	 printf("[SF %d]\n", y, (y < 0) ? errno : j);
	 memcpy(refresh_phya + x, rxtx->sll_addr, physa_octets);
         #endif	/*	LINUX			*/

         #ifdef OSX
         bpfp[x].bf_len = j;
         y = ioctl(fdes, BIOCSETF, &bpfp[x]);
         printf("[SF %d %d]\n", y, (y < 0) ? errno : j);

         memcpy(refresh_phya + x, rxtx->sdl_data + rxtx->sdl_nlen, physa_octets);
         #endif	/*	OSX			*/
      }
   }

   if (halt_flag < 0)
   {
      /******************************************************
	startup snags, don't activate
	else no snags, activate
      ******************************************************/
   }
   else restart();

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

         #ifdef LINUX
         #define SEEFROM
         #ifdef SEEFROM
	 rxtx = rxtxa + x;
	 bytes = recvfrom(fdes, data, 4096, MSG_DONTWAIT, (struct sockaddr *) rxtx, &twenty);
         #else
	 bytes = recv(fdes, data, 4096, MSG_DONTWAIT);
         #endif
         #else
	 bytes = read(fdes, data, 4096);
         #endif

         if (bytes < 0)
         {
            if (uflag['O'-'A']) putchar('.');
            if (errno == EAGAIN) continue;

	    printf("[rer %d:%d:%d]\n", x, errno, fdes);
            continue;
         }

         #ifdef LINUX
         #ifdef SEEFROM

	 j = rxtx->sll_pkttype;

	 if (j)
         {
            if (flag['v'-'a']) printf("[%x->|%x]", j, rxtx->sll_protocol);

            #if 1
	    switch (j)
            {
               case 4:
                 continue;
            }
            #endif
         }
         #endif
         #endif

	 p = data;

         #ifdef LINUX

         if (flag['v'-'a']) printf("[%d:%d]\n", x, bytes);

         if (flag['w'-'a'])
         {
            printf("[%d:%d[", x, bytes);
	    q = p;
            y = bytes;
            while (y--) printf("%2.2x", *q++);
            printf("]\n");
         }

         forward(x, p, bytes);

         /*************************************************************
		RX datagrams on the input trunk
		usleep interval restarted
         *************************************************************/

         #endif

         #ifdef OSX

	 while (bytes > (18 + 4 + 20))
         {
            /**********************************************************

		to avoid confusion a buffer descriptor bpf_hdr *
		at the start of the frame buffer is here called a
		preamble because it is not a network protocol header

		it is a descriptor physically within the packet buffer

		BPF preamble is mostly 18 octets when ethernet header
                14 bytes is expected.  That aligns the following datagram

		On platform loopback BPF preamble is 20 bytes and aligns
		the following 4-byte link pseudo-header and the datagram

		In any case bpf_hdr is whatever size ->bh_hdrlen says

		if a frame < 25 is presented it must be some mistake
		so BPF's buffer isn't processed after less than that
		remains in it. Running correctly count (bytes) reduces
		to zero when the last preamble + frame is taken from it

            *********************************************************/


            /********************************************************
		extract the preamble and frame sizes
		optionally display sizes / buffer remainder / millisecond
		check preamble size
            ********************************************************/

            j = ((struct bpf_hdr *) p)->bh_hdrlen;
            y = ((struct bpf_hdr *) p)->bh_datalen;

            if (flag['v'-'a']) printf("[%d:%3.3d(%d,%d)%d:%d-%d]\n[",
                                    ((struct bpf_hdr *) p)->bh_tstamp.tv_sec % 86400,
                                    ((struct bpf_hdr *) p)->bh_tstamp.tv_usec / 1000,
                                    y,
                                    ((struct bpf_hdr *) p)->bh_caplen,
                                    x, bytes, j);

	    if ((j < 18) || (j > 20))
            {
               printf("[1.%p/%p H%d F%d/%d]\n", data, p, j, y, bytes);
               if (uflag['V'-'A']) diagnose(data, p, j, y, bytes);
               break;
            }

            /*****************************************************
		construct a pointer to the received frame
		= buffer location + BPF preamble length
            *****************************************************/

            q = p + j;

            /****************************************************
		check for anomaly frame longer than BPF buffer
            ****************************************************/

            if ((bytes - y - j) < 0)
            {
               printf("[2.%p/%p H%d F%d/%d]\n", data, p, j, y, bytes);
               if (uflag['V'-'A']) diagnose(data, p, j, y, bytes);
               break;
            }


            /****************************************************
		send the datagram to RTA1 via shared buffer
                and restart the usleep interval
            ****************************************************/

            forward(x, q, y);

            /****************************************************
		construct in y
		BPF-preamble + frame length rounded to 4 octets
            ****************************************************/

            y += j;
            y += 3;
            y &= -4;

            /****************************************************
		advance pointer by consumed data length
		take consumed data length from oustanding count
            ****************************************************/

            p += y;
            bytes -= y;
         }

         #endif
      }

      outputq();

      /**********************************************************
		drive output here
		and a usleep() if the platform is heating
      **********************************************************/
   }

   if (flag['v'-'a']) printf("halt1\n");
   for (x = 0; x < arguments; x++) close(s[x]);
   if (flag['v'-'a']) printf("halt2\n");

   return 0;
}

#ifdef	OSX

static void diagnose(unsigned char *buffer_start,
                     unsigned char *scan_point,
                     int header_bytes, int packet_bytes, int buffer_bytes)
{
   int			 x = scan_point - buffer_start;
   unsigned char	*q = buffer_start;
   int			 poi = 0;

   printf("input scan desynchronised %d+ bytes at %p\n", x, buffer_start);
   printf("spurious header information at %p "
          "header %d/ packet %d/ buffer %d\n",
           scan_point, header_bytes, packet_bytes, buffer_bytes);

   while (x--)
   {
      if ((poi & 15) == 0) printf("\n%4.4x: ", poi);
      printf("%2.2x", *q++);
      poi++;
   }

   printf("\n%d more bytes in buffer\n", buffer_bytes);

   if (poi & 15)
   {
      printf("%4.4x: ", poi);
      x = (poi & 15);
      while (x--) printf("  ");

      while (buffer_bytes--)
      {
         printf("%2.2x", *q++);
         poi++;
         if ((poi & 15) == 0) break;
      }
   }

   while (buffer_bytes--)
   {
      if ((poi & 15) == 0) printf("\n%4.4x: ", poi);
      printf("%2.2x", *q++);
      poi++;
   }
}
#endif

