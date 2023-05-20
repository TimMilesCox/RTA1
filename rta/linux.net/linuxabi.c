#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <linux/filter.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

#include <errno.h>

#define	DATA	16384

#ifdef	INTEL
#define	PLATFORM_LE	1
#define	EVERYTHING	ETH_P_ALL << 8
#else
#define	PLATFORM_LE	0
#define	EVERYTHING	ETH_P_ALL
#endif

#define	FILTER_CANONICAL 0

#define	REVERSE	PLATFORM_LE & FILTER_CANONICAL

#if	REVERSE
#define	LOAD16	0x2800
#define	LOAD32	0x2000
#define	TEST	0x1500
#define	RETURN	0x0600
#define	ETHTYPE	0x0C000000
#define	ARP	0x06080000
#define	IP	0x00080000
#define	IPDEST	0x1e000000
#else
#define	LOAD16	40
#define	LOAD32	32
#define	TEST	21
#define	RETURN	6
#define	ETHTYPE	12
#define	ARP	0x0806
#define	IP	0x0800
#define	IPDEST	30
#endif

/*******************************************************************************
	Linux: Software Loopback lo gets 6-octet zero physical addresses
	plus ETHTYP 0800
	so loopback and external interfaces have the same LL header
*******************************************************************************/


static struct sock_filter	 filter[] = { 	{ LOAD16,	0, 0, ETHTYPE	} ,
						{ TEST,		1, 0, IP	} ,
						{ TEST,		5, 4, ARP	} ,
						{ LOAD32,	0, 0, IPDEST	} ,
						{ TEST,		3, 0, 0xac1d0707 } ,
						{ TEST,		2, 0, 0xac1d0715 } ,
						{ TEST,		1, 0, 0xac1d074b } ,
                                                { RETURN,	0, 0, 0		} ,
                                                { RETURN,	0, 0, -1	} } ;

static struct sock_fprog	 bpfilters =	{ 9, filter } ;

static struct sockaddr_ll	 here = { AF_PACKET, EVERYTHING, 0, 0, 0, 6 } ;
static long			 twenty = 20;

int main(int argc, char *argv[])
{
   unsigned char	 data[DATA];
//   char			*device = argv[1];
   int			 s = socket (AF_PACKET, SOCK_RAW, EVERYTHING);
   int			 x;
   int			 laf;
   unsigned char	*p;

   if (s < 0)
   {
      printf("socket error %d\n", errno);
   }
   else
   {
      printf("socket %d\n", s);
      x = setsockopt(s, SOL_SOCKET, SO_ATTACH_FILTER, &bpfilters, sizeof(bpfilters));
      printf("[socopt %d %d ?device]\n", x, (x < 0) ? errno : 0 /*, device */);

      if (x < 0) printf("nonstart\n");
      else
      {
         if (argc > 1)
	 {
            /**************************************************************************************
		notes
		bind a handle to an interface in order to execute a filter with few tests
  		i.e is it: [ARP?] IP? an IP address assigned to this interface? 

		do not imagine sockaddr_ll is 16 bytes long. bind() will never work if you do

		alternatively don't do this bind
		let the kernel multiplex all your packets on one socket
		and demultiplex them according to ifindex
            **************************************************************************************/

	    here.sll_ifindex = atoi(argv[1]);

            if (argc > 2)
	    {
	       laf = open(argv[2], O_RDONLY, 0777);
	       if (laf < 0) printf("link address file not available %d\n", errno);
	       else read(laf, here.sll_addr, 6);
	       close(laf);
            }

            x = bind(s, (struct sockaddr *) &here, sizeof(here));

	    if (x < 0)
	    {
	        printf("bind wrong %d\n", errno);
                printf("[af:%4.4x p:%4.4x ifidx:%8.8x haspa:%4.4x ptyp:%2.2x PhyAL %x::%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x]\n",
                   here.sll_family, here.sll_protocol, here.sll_ifindex, here.sll_hatype, here.sll_pkttype,
                   here.sll_halen, here.sll_addr[0], here.sll_addr[1], here.sll_addr[2], here.sll_addr[3], here.sll_addr[4], here.sll_addr[5]);
            }
	 }

         printf("start listen\n");

         for (;;)
         {
            #ifdef RXMUX
	    x = recvfrom(s, data, DATA, MSG_DONTWAIT, (struct sockaddr *) &here, &twenty);
            #else
	    x = recv(s, data, DATA, MSG_DONTWAIT);
            #endif

  	    if (x < 0)
            {
	       if (errno == EAGAIN)
	       {
	         usleep(3000);
	         continue;
               }

	       break;
	    }

            #ifdef RXMUX
	    printf("[af:%4.4x p:%4.4x ifidx:%8.8x haspa:%4.4x ptyp:%2.2x PhyAL %x::%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x]",
	           here.sll_family, here.sll_protocol, here.sll_ifindex, here.sll_hatype, here.sll_pkttype, 
		   here.sll_halen, here.sll_addr[0], here.sll_addr[1], here.sll_addr[2], here.sll_addr[3], here.sll_addr[4], here.sll_addr[5]);
            #endif

            putchar('[');
            p = data;
   	    while (x--) printf("%2.2x", *p++);
	    printf("]\n");
         }
      }

      printf("end listen\n");
      if (x < 0) printf("E %d\n", errno);
   }

   close(s);
   return 0;
}

