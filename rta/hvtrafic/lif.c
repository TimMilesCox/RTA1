#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <errno.h>
#include "../include.rta/argue.h"
#include "../include.rta/address.h"
#include "../netifx/sifr_mm.h"
#include "../netifx/ifconfig.h"
#include "../portal/portal.h"

#define	DLT_NULL	0
#define	DLT_ENET	1
#define	DLT_RAW		12

#undef	DEVICE_NAMES
#define	LLHL		0
#define	RTA1_IFIDX	5-2
#define	IFCONFIG_OCTETS (sizeof(i_f_string) + 24)
#define	INTERFACES	4

#ifdef	__X64
#ifdef	LINUX
#define	RUNL_NAME	"linuxnet.x64/runL"
#endif
#ifdef	OSX
#define	RUNL_NAME	"portal64/runL"
#endif
#else			/*	__X64 [ /  __X86 ]	*/
#ifdef	LINUX
#define	RUNL_NAME	"linux.net/runL"
#endif
#ifdef	OSX
#define	RUNL_NAME	"portal/runL"
#endif
#endif			/*      __X64 [ /  __X86 ]      */

#define	BLUE	"\033[104;1;37m"
#define	RED	"\033[101;1;37m"
#define	PRIMARY	"\033[0m"

mm_netbuffer		*dpointer,
			*container_pointer,
			*threshold_i,
			*threshold_o,
			*limit_o;

mm_netbuffer		*rxdata,
                        *txdata,
			*netframe;

static int			 container_key;
static int			 container_index;
static int			 mhandle;
static struct shmid_ds		 info;

unsigned int			 i_f_first_address;
static unsigned char		*addresses;
static unsigned char		 address[ADDRESS_STRING];
static char			 refresh_phya[INTERFACES][8];

int				 tpexit;

long long		 payload_segments,
			 payload;

extern void procedure(void);
extern void callback(char *ipdgram);

unsigned char *peel(unsigned char *p, unsigned char *q)
{
   int           symbol,
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

         q = (unsigned char *) "";
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

static void configure_interface(int i_f, int	physa_octets,
                                unsigned char	*address)
{
   unsigned char	*component2 = (unsigned char *) strchr((char *) address, ':');

   char			 protocol_name[4] = "   \0";
   int			 port = 0;
   int			 ports = 1;
   int			 density = 4096 / 24 - 1;

   unsigned short	*upconf = (unsigned short *)
                                 ((char *) netframe->frame  + IFCONFIG_OCTETS);

   if (physa_octets == 0)                                                            
   {
      /*****************************************************************
	Dude, it is
      *****************************************************************/

      netframe->frame[0] = DLT_RAW;
      netframe->frame[1] = 0;
   }

   else if (physa_octets == 6)
   {
      /*****************************************************************
	likely to be Linux, all I/Fs have ethernet LLH structure
      *****************************************************************/

      netframe->frame[0] = DLT_ENET;
      netframe->frame[1] = 14;
   }

   else if (physa_octets == 4)
   {
      /*****************************************************************
        likely to be OSX, non-external I/F has 00000002 platform-endian
      *****************************************************************/

      netframe->frame[0] = DLT_NULL;
      netframe->frame[1] = 4;
   }

   if (component2) *component2 = 0;
   memset(netframe->frame + 4, 0, 48 - 10 - 4);

   sscanf((char *) address, "%hhd.%hhd.%hhd.%hhd/%hhd", netframe->frame + 4,
                                                        netframe->frame + 5,
                                                        netframe->frame + 6,
                                                        netframe->frame + 7,
                                                        netframe->frame + 8);

   netframe->frame[9] = physa_octets;
   netframe->preamble.frame_length = PORT((sizeof(i_f_string) + physa_octets));
   netframe->preamble.protocol = CONFIGURATION_MICROPROTOCOL;

   #ifdef DEVICE_NAMES
   if (flag['v'-'a']) printf("[%d %s %s]\n", i_f, netdevice, address);
   #endif

   memcpy(netframe->frame + 10, refresh_phya + i_f, physa_octets);

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

      sscanf((char *) component2, "%d/%3s/%d/%d", &port, protocol_name, &ports, &density);
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
            upconf[1] = PORT(port);
            upconf[2] = PORT(ports);
            upconf[3] = PORT(density);
            netframe->preamble.frame_length = PORT((IFCONFIG_OCTETS+8));
         }
      }
   }
}

static void configure_route(int i_f,  unsigned char *address)
{
   char			 symbol = 0;
   unsigned char	*next_hop_location = netframe->frame + 8;
   unsigned int		*next_hop = (unsigned int *) next_hop_location;

   netframe->frame[8] = 0;
   netframe->frame[9] = 0;
   netframe->frame[10] = 0;
   netframe->frame[11] = 0;

   netframe->frame[2] = 0;	/* upper half big endian mask width */


   sscanf((char *) address, "%hhd.%hhd.%hhd.%hhd/%hhd%c%hhd.%hhd.%hhd.%hhd", 
                                                 netframe->frame + 4,
                                                 netframe->frame + 5,
                                                 netframe->frame + 6,
                                                 netframe->frame + 7,
                                                 netframe->frame + 3,
                                                           &symbol,
                                                 netframe->frame + 8,
                                                 netframe->frame + 9,
                                                 netframe->frame + 10,
                                                 netframe->frame + 11);
  
   netframe->frame[1] = i_f;
   netframe->frame[0] = 0;

   if (symbol == '*') netframe->frame[0] = 128;
   else *next_hop = i_f_first_address;

   netframe->preamble.frame_length = PORT(sizeof(route_microstring));
   netframe->preamble.protocol = ROUTE_CONFIGURATION_MICROPROTOCOL;

   #ifdef DEVICE_NAMES
   if (flag['v'-'a']) printf("[%d %s %s ->]\n", i_f, netdevice, address);
   #endif
}

static void deliver_configuration_tuple(int i_f)
{
   char			*p = (char *) netframe;

   unsigned short	 bytes = sizeof(mm_descriptor)
			 +  PORT(netframe->preamble.frame_length);

   netframe->preamble.ll_hl = LLHL;
   netframe->preamble.i_f = PORT(i_f);
   netframe->preamble.flag = FRAME;
   netframe++;

   printf("[<-");
   while (bytes--) printf("%2.2x", *p++);
   printf("]\n");
}

static void configure_view(int interface, int iftype,
                           char *addresses)
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

   #if	1		/* this is a  trunk RTA1..RTA1 one 1 platform
			   and has no link layer encapsulation ever
			   and is DLT_RAW which is not anything else	*/
			
   int			 physa_octets = 0;

   #else

   int			 physa_octets = (iftype) ? 6 : 4;

   #endif

   int			 symbol,
			 symbols;

   unsigned char	*p,
			*q = netframe->frame + 4;

   unsigned int		*initial_address = (unsigned int *) q;

   unsigned char	 address[ADDRESS_STRING];

   netframe->frame[0] = iftype;

   #ifdef LINUX
   netframe->frame[1] = 14;
   #endif

   #ifdef OSX
   netframe->frame[1] = (iftype) ? 14 : 4;
   #endif

   netframe->frame[2] = 0;
   netframe->frame[3] = 0;

   #if 0
   if (x) netframe->frame[3] = 2;   /* within RTA1 machine
                                     requeue output from this logical i/f
                                     to logical i/f 2:0
                                     which is managing the shared buffer
                                     they can't all manipulate that
                                  */
   #endif

   netframe->frame[8] = 32;         /* default mask size */

   #ifdef DEVICE_NAMES
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
   #endif

   q = peel(address, addresses);
   configure_interface(RTA1_IFIDX, physa_octets, address);
   i_f_first_address = *initial_address;
   deliver_configuration_tuple(RTA1_IFIDX);

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

            netframe->frame[0] = 0;
            netframe->frame[1] = 0;

            netframe->frame[2] = 0;
            netframe->frame[3] = 0;

            #if 0
            if (x) rxdata->frame[3] = 2;
            #endif

            netframe->frame[8] = 32;
            
            configure_interface(RTA1_IFIDX, physa_octets, address);
            break;

         case '@':
            configure_route(RTA1_IFIDX, address);
      }

      deliver_configuration_tuple(RTA1_IFIDX);
   }
}

#ifdef	LINUX
#define	DLT_PLATFORM	DLT_ENET
#endif

#ifdef	OSX
#define	DLT_PLATFORM	DLT_NULL
#endif

static void configure(int interface, int iftype)
{
   netframe = rxdata;
   configure_view(RTA1_IFIDX, DLT_PLATFORM, argument[0]);
   rxdata = netframe;
   netframe = txdata;
   configure_view(RTA1_IFIDX, DLT_PLATFORM, argument[1]);
   txdata = netframe;
}

void restart()
{
   rxdata = limit_o;

   while (rxdata > threshold_i)
   {
      rxdata--;
      rxdata->preamble.flag = 0;
   }

   txdata = threshold_o;

   configure(RTA1_IFIDX, DLT_PLATFORM);

   /*****************************************************
        2, argument is DLT_ENET for LINUX
	2. argument is device type 0 = sloopback for OSX
   *****************************************************/

   printf("trunk %p:%p restarted\n", threshold_i, limit_o);
}

int main(int argc, char *argv[])
{
   int			 x;
   int			 blocks;

   pthread_t		 tthread_id;
   pthread_attr_t	 tthread_attr;

   argue(argc, argv);

   if (arguments > 2)
   {
      /*
      addresses  = (unsigned char *) argument[0];
      addresses1 = (unsigned char *) argument[1];
      */
   }
   else
   {
      printf("arg1 = / arg2 = "
      "address"
      "[+addresses[/maskw]]"
      "[@route[/maskw][*gateway]]"
      "[,port/protocol/how_many]\n"
      "arg3 = (for eyample) #7070xxxx (banks in hex)\n");

      return 0;
   }

   /******************************************************
	trunk to be assigned here
   ******************************************************/

   sscanf(argument[2], "%x", &container_key);

   blocks = (container_key & 65535) << 19;
   mhandle = shmget(container_key, blocks, IPC_CREAT);

   if (mhandle < 0)
   {
      printf("assignment failed trunk %x : %x\n", container_key, blocks);
      printf("E %d\n", errno);
      return 0;
   }

   x = shmctl(mhandle, IPC_STAT, &info);

   if (x < 0)
   {
      printf("can't calibrate trunk storage E %d: sudo lif ...\n", errno);
      return 0;
   }

   dpointer = (mm_netbuffer *) (void *) shmat(mhandle, NULL, 0);
   container_pointer = dpointer;
   threshold_i = container_pointer;
   threshold_o = container_pointer + (info.shm_segsz >> 12);
   limit_o = container_pointer + (info.shm_segsz >> 11);
   printf("reactive size RX %p .. %p TX %p .. %p\n",
           threshold_i, threshold_o, threshold_o, limit_o);
 
   printf("netbank %d\n", mhandle);

   #if	0	/* def TRAFFIC_TEST */
   if (uflag['M'-'A'])
   {
      x = pthread_attr_init(&tthread_attr);

      if (x < 0) printf("threadcbinit %d e %d\n", x, errno);
      else
      {
         x = pthread_create(&tthread_id, &tthread_attr, test_procedure, NULL);
         if (x < 0) printf("test procedure start error %d\n", errno);
         printf("test thread %lx\n", tthread_id);
      }
   }
   #endif

   for (;;)
   {
      /*****************************************************************
         this is the configuration part. Break back to here on restart
      *****************************************************************/

      restart();

      for (;;)
      {
         procedure();
         if (tpexit < 0) return 0;
      }
   }

   return 0;
}
