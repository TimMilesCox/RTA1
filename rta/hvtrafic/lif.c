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

#undef	DEVICE_NAMES
#define	LLHL		0
#define	RTA1_IFIDX	5
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
                        *txdata;

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

static void configure_interface(int i_f, int physa_octets, unsigned char *address)
{
   unsigned char	*component2 = (unsigned char *) strchr((char *) address, ':');

   char			 protocol_name[4] = "   \0";
   int			 port = 0;
   int			 ports = 1;
   int			 density = 4096 / 24 - 1;

   unsigned short	*upconf = (unsigned short *)
                                 ((char *) rxdata->frame  + IFCONFIG_OCTETS);
                                                            
   if (physa_octets == 6)
   {
      /*****************************************************************
	likely to be Linux, all I/Fs have ethernet LLH structure
      *****************************************************************/

      rxdata->frame[0] = DLT_ENET;
      rxdata->frame[1] = 14;
   }

   if (physa_octets == 4)
   {
      /*****************************************************************
        likely to be OSX, non-external I/F has 00000002 platform-endian
      *****************************************************************/

      rxdata->frame[0] = DLT_NULL;
      rxdata->frame[1] = 4;
   }

   if (component2) *component2 = 0;
   memset(rxdata->frame + 4, 0, 48 - 10 - 4);

   sscanf((char *) address, "%hhd.%hhd.%hhd.%hhd/%hhd", rxdata->frame + 4,
                                                        rxdata->frame + 5,
                                                        rxdata->frame + 6,
                                                        rxdata->frame + 7,
                                                        rxdata->frame + 8);

   rxdata->frame[9] = physa_octets;
   rxdata->preamble.frame_length = PORT((sizeof(i_f_string) + physa_octets));
   rxdata->preamble.protocol = CONFIGURATION_MICROPROTOCOL;

   #ifdef DEVICE_NAMES
   if (flag['v'-'a']) printf("[%d %s %s]\n", i_f, netdevice, address);
   #endif

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
            rxdata->preamble.frame_length = PORT((IFCONFIG_OCTETS+8));
         }
      }
   }
}

static void configure_route(int i_f, unsigned char *address)
{
   char			 symbol = 0;
   unsigned char	*next_hop_location = rxdata->frame + 8;
   unsigned int		*next_hop = (unsigned int *) next_hop_location;

   rxdata->frame[8] = 0;
   rxdata->frame[9] = 0;
   rxdata->frame[10] = 0;
   rxdata->frame[11] = 0;

   rxdata->frame[2] = 0;	/* upper half big endian mask width */


   sscanf((char *) address, "%hhd.%hhd.%hhd.%hhd/%hhd%c%hhd.%hhd.%hhd.%hhd", 
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
  
   rxdata->frame[1] = i_f - 2;
   rxdata->frame[0] = 0;

   if (symbol == '*') rxdata->frame[0] = 128;
   else *next_hop = i_f_first_address;

   rxdata->preamble.frame_length = PORT(sizeof(route_microstring));
   rxdata->preamble.protocol = ROUTE_CONFIGURATION_MICROPROTOCOL;

   #ifdef DEVICE_NAMES
   if (flag['v'-'a']) printf("[%d %s %s ->]\n", i_f, netdevice, address);
   #endif
}

static void deliver_configuration_tuple(int i_f)
{
   char			*p = (char *) rxdata;

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

static void configure(int interface, int iftype)
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

   int			 physa_octets = (iftype) ? 6 : 4;
   int			 symbol,
			 symbols;

   unsigned char	*p,
			*q = rxdata->frame + 4;

   unsigned int		*initial_address = (unsigned int *) q;

   unsigned char	 address[ADDRESS_STRING];

   rxdata->frame[0] = iftype;

   #ifdef LINUX
   rxdata->frame[1] = 14;
   #endif

   #ifdef OSX
   rxdata->frame[1] = (iftype) ? 14 : 4;
   #endif

   rxdata->frame[2] = 0;
   rxdata->frame[3] = 0;

   #if 0
   if (x) rxdata->frame[3] = 2;   /* within RTA1 machine
                                     requeue output from this logical i/f
                                     to logical i/f 2:0
                                     which is managing the shared buffer
                                     they can't all manipulate that
                                  */
   #endif

   rxdata->frame[8] = 32;         /* default mask size */

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

            rxdata->frame[0] = 0;
            rxdata->frame[1] = 0;

            rxdata->frame[2] = 0;
            rxdata->frame[3] = 0;

            #if 0
            if (x) rxdata->frame[3] = 2;
            #endif

            rxdata->frame[8] = 32;
            
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

   if (arguments)
   {
      addresses = (unsigned char *) argument[0];
   }
   else
   {
      printf("arg1 = "
      "address"
      "[+addresses[/maskw]]"
      "[@route[/maskw][*gateway]]"
      "[,port/protocol/how_many]\n");
      return 0;
   }

   if (uflag['K'-'A'])
   {
      /*******************************************************
                trunk is a compartment of device 2
                already assigned or no go
                1st 226144-word block is all default interfaces
		uflags [ [ A ] BCDEFGHIJ ] = which compartment
      *******************************************************/
         
      container_index = 0; 
      for (x = 0; x < 10; x++) if (uflag[x]) container_index = x;
      mhandle = shmget('aaaa', DEVICE_PAGE * DEVICE_PAGES, 0);

      if (mhandle < 0)
      {
         printf("flag 'K': primary network trunk not assigned. runL first\n");
         printf(RUNL_NAME " first\n");
         printf("E %d\n", errno);
         return 0;
      }

      x = shmctl(mhandle, IPC_STAT, &info);

      if (x < 0)
      {
         printf("can't calibrate trunk storage E %d: sudo lif ...\n", errno);
         return 0;
      }

      if (info.shm_segsz < ((container_index + 1)
                         *  CONTAINER_PAGES
                         *  DEVICE_PAGE))
      {
         printf("container = octets %x .. %x of trunk = %x octets\n",
                 container_index * CONTAINER_PAGES * DEVICE_PAGE,
            (container_index+ 1) * CONTAINER_PAGES * DEVICE_PAGE - 1,
                 (int) info.shm_segsz);

         printf("trunk compartment %c not assigned\n", container_index + 'A');
         return 0;
      }

      dpointer = (mm_netbuffer *) (void *) shmat(mhandle, NULL, 0);
      container_pointer = dpointer + CONTAINER_PAGES * container_index;

      threshold_i = container_pointer;
      if (container_index == 0) threshold_i = container_pointer + DEVICE_PAGES;

      /*****************************************************************
         1st 262122 words of 1st container is "normal" traffic rx / tx
      *****************************************************************/

      threshold_o = container_pointer + CONTAINER_PAGES / 2;
      limit_o = container_pointer + CONTAINER_PAGES;
   }
   else if (arguments > 1)
   {
      sscanf(argument[1], "%x", &container_key);

      if (container_key < 0x61000000)
      {  
         /*******************************************************
                trunk already assigned by RTA1 console operation
         *******************************************************/
         
         mhandle = shmget(container_key, DEVICE_PAGE * DEVICE_PAGES, 0);

         if (mhandle < 0)
         {
            printf("assign trunk %x @ RTA1 startupfirst\n", container_key);
            printf("E %d\n", errno);
            return 0;
         }
      }
      else
      {
         /******************************************************
		trunk to be assigned here
         ******************************************************/

	 blocks = (container_key & 65535) << 19;
         mhandle = shmget(container_key, blocks, IPC_CREAT);

         if (mhandle < 0)
         {
            printf("assignment failed trunk %x : %x\n", container_key, blocks);
            printf("E %d\n", errno);
            return 0;
         }
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
   }
   else
   {
      printf("argument 2 hex device key or flag -K[ABCDEFGHIJ]\n");
      return 0;
   }

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
