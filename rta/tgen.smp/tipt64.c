#include <stdio.h>

#ifdef	X86_MSW
#include <windows.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <errno.h>
#include <process.h>
#else
#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>

extern char **environ;

#endif

// #define	RTA_SMP	8

#include "../include.rta/argue.h"
#include "../engine.smp/emulate.h"
#include "../engine.smp/smp.h"
#include "../engine.smp/ii.h"
#include "../engine.fs/device24.h"
#include "idisplay.h"
#include "../rta.run/settings.h"
#include "../rta.run/tzone.h"
#include "../rta.run/stepping.h"

#ifndef	__X64
#include "../rta.run/time32.h"
#endif

#ifdef	GCC
#else
#ifdef	LINUX
#include "../tgen.smp/_mnames.h"
#endif
#endif

#ifndef __X64
static step_second32		 step_second;
#endif

static unsigned int		 clockr[2];

static int			 interval_seconds_mask;
static int			 platform_interval = PLATFORM_INTERVAL;

/********************************************************

	global data whether here or extern

********************************************************/

//	#include "../tgen.x64/ioports.c"

#if	RTA_SMP
extern device                    devices[];             /* in rw */
#define	ROM_PAGE		 devices[0].pages

word				*breakpoint;

word				*touchpoint,
				*touchpoint2;

int				 general_indication;
extern int			 register_pointer;

smp				 core[RTA_SMP]
	= {	{	{	},
			NULL, NULL, NULL, NULL,
			{ 0, II, 0, RESTART1 },
			0, 0, 128,
			0x00870000, 0, 0, 0x7E7E7E7E,
			0, 0,
			0, 0, 0,
			{ 	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,	 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
							 0, 1, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, (1<<RTA_SMP)-1, 1, 0,

				SYSMEM_FLAG,
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),
				         0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0	 }    },

		{	{	},
			NULL, NULL, NULL, NULL,
			{ 0, II, 0, RESTART6 },
			0, 0, 128,
			0x00800000, 0, 0, 0x7E7E7E7E,
			0, 0,
			0, 0, 0,
			{ 	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,	 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
							 0, 2, 0, 0, 0, 1, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, (1<<RTA_SMP)-1, 1, 0,

				SYSMEM_FLAG,
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),
				         0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0	 }    },

		{	{	},
			NULL, NULL, NULL, NULL,
			{ 0, II, 0, RESTART6 },
			0, 0, 128,
			0x00800000, 0, 0, 0x7E7E7E7E,
			0, 0,
			0, 0, 0,
			{ 	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,	 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
							 0, 4, 0, 0, 0, 2, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, (1<<RTA_SMP)-1, 1, 0,

				SYSMEM_FLAG,
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),
				         0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0	 }    },

		{	{	},
			NULL, NULL, NULL, NULL,
			{ 0, II, 0, RESTART6 },
			0, 0, 128,
			0x00800000, 0, 0, 0x7E7E7E7E,
			0, 0,
			0, 0, 0,
			{ 	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,	 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
							 0, 8, 0, 0, 0, 3, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, (1<<RTA_SMP)-1, 1, 0,

				SYSMEM_FLAG,
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),
				         0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0	 }    },

		{	{	},
			NULL, NULL, NULL, NULL,
			{ 0, II, 0, RESTART6 },
			0, 0, 128,
			0x00800000, 0, 0, 0x7E7E7E7E,
			0, 0,
			0, 0, 0,
			{ 	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,	 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
							0, 16, 0, 0, 0, 4, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, (1<<RTA_SMP)-1, 1, 0,

				SYSMEM_FLAG,
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),
				         0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0	 }    },

		{	{	},
			NULL, NULL, NULL, NULL,
			{ 0, II, 0, RESTART6 },
			0, 0, 128,
			0x00800000, 0, 0, 0x7E7E7E7E,
			0, 0,
			0, 0, 0,
			{ 	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,	 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
							0, 32, 0, 0, 0, 5, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, (1<<RTA_SMP)-1, 1, 0,

				SYSMEM_FLAG,
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),
				         0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0	 }    },

		{	{	},
			NULL, NULL, NULL, NULL,
			{ 0, II, 0, RESTART6 },
			0, 0, 128,
			0x00800000, 0, 0, 0x7E7E7E7E,
			0, 0,
			0, 0, 0,
			{ 	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,	 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
							0, 64, 0, 0, 0, 6, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, (1<<RTA_SMP)-1, 1, 0,

				SYSMEM_FLAG,
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),
				         0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0	 }    },

		{	{	},
			NULL, NULL, NULL, NULL,
			{ 0, II, 0, RESTART6 },
			0, 0, 128,
			0x00800000, 0, 0, 0x7E7E7E7E,
			0, 0,
			0, 0, 0,
			{ 	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,	 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
						       0, 128, 0, 0, 0, 7, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, (1<<RTA_SMP)-1, 1, 0,

				SYSMEM_FLAG,
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),
				         0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0	 }    }    } ;

#else

#define ROM_PAGE        &memory.p4k[0].w[0]

extern device			 devices[];		/* in rw */
extern int			 register_pointer;	/* in console */

int				 iselect = 128;
int				 iselectu = 256;
int				 indication;

word				*apc = ROM_PAGE;
word				*apcu;
word				*apcz = &memory.p4k[0].w[4095];
word				*breakpoint;

page				*b0p = memory.p4k;
unsigned int			 b0_name;
unsigned int			 psr = 0x00800000;
unsigned int			 _register[REGISTERS];
unsigned int			*register_set = _register + 128;

#endif

device				*pdevice = devices;

#ifndef	RTA_SMP

#ifdef METRIC
unsigned int			 delta,
				 metric;

unsigned long long		 delta_base,
				 total_delta,
				 total_metric;
#endif
#endif

extern int			 ultra;
extern int			 ultra1;
extern int			 ultra2;
extern int			 ultra3;
extern int			 ultra4;
extern int			 ultra5;
extern int			 ultra6;
extern int			 ultra7;

/********************************************************
	only protoypes from here
********************************************************/

#ifdef	GCC
#include "../engine.smp/rw.h"
#else
static word		 memory_read(int ea);
static int device_array_read(int device_index, int block, int offset);
extern void		 device_read();
extern void		 bus_read();
#endif

extern void              netbank();

#ifdef	GCC_CC
extern void		 execute(word instruction, smp *);
#else
extern void		 leloup(smp *);
#endif

static void		*emulate(smp *);
static void		 statement(smp *);
extern void		 action(char request[], smp *);
void			 load_fs(int device_id, char *path);
extern void		 assign_interface_relay(int device_id, char *text);
       void		 assign_array(int device_id, char *text);
static int		 msw2i(msw w);
extern int		 print_register_row(int index, smp *xcore);
extern			 int print7_registers(int index, smp *xcore);

#ifdef	METRIC
static void		 accumulate_metric(smp *);
#endif

#include "dayclock.c"
#include "fifoprio.c"

int main(int argc, char *argv[])
{
   int			 _x = 8,
			 _y;

   int			 fields,
			 device_id;

   char			 text[72];
   char			*_p;

   pthread_attr_t        asyncb[RTA_SMP];
   pthread_t             asyncid[RTA_SMP];
   int			 mid_priority = derive_fifo_priority() ;

   struct sched_param	 asyncp;

   int                   f, count, image_size = 0;

   word                 *load_address;
   word                  data_word = { 0, 0, 0, 0 } ;

   smp			*ascor = core + 1;
   int			 cores;


   argue(argc, argv);

   printf("%4.4X cores %d\n", sizeof(smp), RTA_SMP);

   #ifdef GCC

   printf("%s\n", getenv("RTA_MBANKS"));
   _p = getenv("RTA_MBANKS");

   if (_p) _x = atoi(_p);
   else printf("default memory configuration\n");

   if ((_x > 0) && (_x < 65537)) rta_mbanks = _x;
   if (_x < 8) printf("memory < 8 banks may be too restricted\n");

   printf("memory %d banks\n", rta_mbanks);

   #ifdef ATRACE
   printf("%d:\n", argc);
   for (_x = 0; _x < argc; _x++) printf("%s\n", argv[_x]); 
   #endif

   devices[0].pages = calloc(262144 * 4, rta_mbanks);

   if (devices[0].pages == NULL)
   {
      printf("RTA1 system memory not assigned\n");
      return 0;
   }

   pages_in_memory = rta_mbanks << 6;
   words_in_memory = pages_in_memory << 12;

   printf("RTA1 " RADIX " emulation stepping " STEPPING " %d kwords memory\n",
   rta_mbanks << 8);

   #else		/*	GCC or not	*/

   printf("RTA1 " RADIX " emulation stepping " STEPPING " %d kwords memory\n",
   MBANKS << 8);	/*	GCC or x86 asm	*/

   #endif

   load_address = ROM_PAGE->w;

   core[0].APC  = ROM_PAGE->w;
   core[0].APCZ = ROM_PAGE->w + 4095;
   core[0].register_set = &core[0].REGISTER[128];
   core[0].B0P = ROM_PAGE;

   #ifdef GCC
   core[0].BASE[128] = (pages_in_memory - 1) | SYSMEM_FLAG;
   #else
   core[0].BASE[128] = (PAGES_IN_MEMORY - 1) | SYSMEM_FLAG;
   #endif

   if (arguments)
   {
      #ifdef X86_MSW
      f = open(argument[0], O_RDONLY | O_BINARY, 0444);
      #else
      f = open(argument[0], O_RDONLY, 0444);
      #endif

      if (f < 0)
      {
         printf("rom image file not available\n");
         return 0;
      }

      if (flag['v'-'a'])
      {
         printf("reading %s with options:\n", argument[0]);
         if (flag['s'-'a']) printf("-s\tsingle step\n");
         if (flag['v'-'a']) printf("-v\tvocal\n");
         if (flag['l'-'a']) printf("-l\tline per instruction\n");
         printf("[MEM %p LOAD %p]\n", ROM_PAGE, load_address);
      }

      for (;;)
      {
         count = read(f, (void *) &data_word.t1, 3);
         if (count < 1) break;
         if (count ^ 3) printf("last load word incomplete\n");

         *load_address++ = data_word;
         image_size++;
      }

      printf("rom image %d target words read\n", image_size);
      image_size += 4095;
      core[0].BASE[124] = image_size >> 12;	/*      ROM boundary port       */
						/*      system memory size port */
      close(f);
   }
   else
   {
      printf("rom image file required\n");
      return 0;
   }

   netbank();
   printf("network trunk running\n");
   if (arguments > 1) load_fs(1, argument[1]);

   for (_x = 2; _x < arguments; _x++)
   {
      _p = argument[_x];

      fields = sscanf(_p, "/%d/%s", &device_id, text);

      if      (fields < 2)    printf("argument %d skipped: requires /device number/device info\n", _x);
      else if (device_id < 3) printf("argument %d skipped: devices 0..2 are fixed\n", _x);
      else
      {
         if      (text[0] == '+') assign_array(device_id, text);
         else if (text[0] == '#') assign_interface_relay(device_id, text);
         else load_fs(device_id, text);
      }
   }

   printf("cores %d\n", RTA_SMP);

   cores = RTA_SMP - 1;

   while (cores--)
   {
      ascor->APC  = ROM_PAGE->w + RESTART6;
      ascor->APCZ = ROM_PAGE->w + 4095;
      ascor->register_set = ascor->REGISTER + 128;
      ascor->B0P = ROM_PAGE;
      ascor->BASE[124] = core[0].BASE[124];
      ascor->BASE[128] = core[0].BASE[128];
      ascor++;
   }

   printf("%d cores running\n", RTA_SMP);

   ascor = core;

   for (cores = 0; cores < RTA_SMP; cores++)
   {
//      usleep(5000);	/* give each thread in turn a chance	*/

      _x = pthread_attr_init(asyncb + cores);
      _y = pthread_create(asyncid + cores, asyncb + cores, emulate, ascor);
      if ((_x < 0) || (_y < 0)) printf("async thread start %d %d\n", _x, errno);
      else
      {
         printf("async thread ID %p\n", asyncid + cores);

         if (uflag['O'-'A'])
         {
            /******************************************************************

	       sched_param structure asyncp updated each core start
               because its address is passed to a kernel routine

            ******************************************************************/

            asyncp.sched_priority = mid_priority;

            _x = pthread_setschedparam(asyncid[cores], SCHED_FIFO, &asyncp);
            if (_x < 0) printf("switch FIFO E%d\n", errno);

            _x = pthread_getschedparam(asyncid[cores], &_y, &asyncp);

            if (_x < 0) printf("(E%d)", errno);
            printf("[PY %d S-P %d]\n", _y, asyncp.sched_priority);
         }
      }

      ascor++;
   }

   printf("key %s\n\n", (flag['s'-'a'])
                        ? "g[break:point] to run"
                        : "s to enter single step");
   #ifndef __X64
   if (uflag['Y'-'A'] == 0) start_second(&step_second);
   #endif

   for (;;)
   {
         _p = fgets(text, 71, stdin);

         if (_p == NULL)
         {
            printf("stdin terminated\n");
            break;
         }

         if (*_p == '.')
         {
            if (*(_p + 1) == '.') break;
            printf(".. to stop the emulator\n");

            if (ascor->INDICATION & LOCKSTEP) printf("command>");
            else                              printf("emulator running\n"
                                              "key s for single step:");

            fflush(stdout);

            continue;
         }

         action(_p, core);
   }

   return 0;
}

extern int which;

void *emulate(smp *xcore)	/* thread start */
{
   int			*register_set = xcore->register_set;

   #ifdef METRIC
   struct timeval	 time2;
   #endif

   #ifdef RATIO
   int			 ratio;
   #endif

   #if 0
   *((long *) &_register[280]) = (long) xcore;
   *((long *) &_register[280-128]) = (long) xcore;
   #endif

   printf("emulation start\n");

   if (base[77] == 0)
   {
      dayclock(1, xcore);
      printf("[%6.6x:%6.6x %6.6x:%6.6x]\n", clockr[0], clockr[1], _register[DAYCLOCK_U], _register[DAYCLOCK]);
   }

   for (;;)
   {
      #ifdef __X64
      #define FORMAT1 "[ %x %x %x %x | %p %p %p | %x %x %x | %p %p %lx %lx | %x %x *%p *%p ]\n"
      #else
      #define FORMAT1 "[ %x %x %x %x | %p %p %p | %x %x %x | %p %p %x %x | %x %x *%p *%p ]\n"
      #endif

      if (flag['s'-'a']) general_indication |= LOCKSTEP;
      if ((flag['e'-'a']) && ((general_indication & LOCKSTEP)
                          ||  ((flag['e'-'a']))))
      printf(FORMAT1, base[77], indication, general_indication, breakpoint,
		      xcore, core, register_set,
                      iselect, register_set - _register, psr,
                      apc, apcz, apc - ROM_PAGE->w, apcz - ROM_PAGE->w,
		      devices[0].pages, sp, touchpoint, touchpoint2);

      #ifdef METRIC
      gettimeofday(&time2, NULL);
      delta_base = time2.tv_sec * 1000000 + time2.tv_usec;
      #endif

      /*********************************************************
	time and emulated instructions
        are measured from start of this block
        to end of this block
      *********************************************************/

      #ifdef RATIO
      _register[283] = RATIO;

      if (flag['s'-'a'])
      {
          while ((which) && (base[77] ^ (which & 15)))
          {
             usleep(PLATFORM_INTERVAL);
             continue;
          }

          _register[283] = 0;	/* stops at -1 */
      }

      #ifndef GCC
      if (flag['w'-'a'])
      {
         printf("[%d]>\n", _register[283]);
         printf("[X]\n");
      }
      #endif

      leloup(xcore);

      register_set = xcore->register_set;

      #ifndef GCC
      if (flag['w'-'a'])
      {
         printf("[Y %lx:%lx]\n", ultra, ultra1);
         printf("<[%d]\n", _register[283]);
      }
      #endif

      ratio = _register[283];
      #endif

      #ifdef METRIC
      #ifdef RATIO
      metric = RATIO - ratio;
      #endif
      accumulate_metric(xcore);
      #endif

      if (indication & CHILLDOWN)
      {
         indication &= -1 ^ CHILLDOWN;
         usleep(base[103]);

         /***********************************************
		chilldown granularity
		is in the order of milliseconds

		realign the dayclock
		to stop time driven events going slack
         ***********************************************/

         indication |= TIME_UPDATE;

         /***********************************************
		chilldown is ordered from an application
		so dayclock(0) zero argument takes effect
         ***********************************************/

         #if 0
         printf("[%d.%d]", base[77], base[103]);
         #endif
      }

      if (indication & TIME_UPDATE)
      {
         indication &= -1 ^ TIME_UPDATE;

         #ifdef RATIO

         if ((base[77] == 0) && (flag['s'-'a'] == 0)) dayclock(1, xcore);
         
         #else
	 #error YOU ARE NOT HERE
         _register[DAYCLOCK]   = clockr[1];
         _register[DAYCLOCK_U] = clockr[0];

         #endif
      }

      if (general_indication & LOCKSTEP) flag['s'-'a'] = 1;

      if (general_indication & BREAKPOINT)
      {
         if (apc == breakpoint)
         {
            flag['s'-'a'] = 1;
            general_indication |= LOCKSTEP;
         }
      }

      if ((general_indication & LOCKSTEP) == 0) continue;
      if ((which) && ((which & 15) ^ base[77])) continue;

      statement(xcore);
      putchar('>');
      fflush(stdout);

      while (general_indication & LOCKSTEP) usleep(PLATFORM_INTERVAL);
   } 
}

void load_fs(int device_id, char *path)
{
   int		 xx,
		 banks,
		 index = 0;

   char		*loader = (char *) devices[device_id].dev24;

   #ifdef X86_MSW
   int		 f = open(path, O_RDONLY | O_BINARY, 0444);
   #else
   int		 f = open(path, O_RDONLY, 0444);
   #endif

   smp		*ascor = core;
   int		 cores = RTA_SMP;

   vpage	 page;

   if (f < 0) printf("file system image %s error %d\n", path, errno);
   else
   {
      printf("loading file system image %s\n", path);
      xx = read(f, (void *) &page, sizeof(page));

      /**************************************************
	more exactly volume root directory initial page
      **************************************************/

      if (xx ^ sizeof(page))
      {
         printf("volume header read unsuccessful\n");
         return;
      }

      if (flag['v'-'a'])
      {
         printf("[%2.2x%2.2x%2.2x]\n",
                 page.label.ex.rfw.t1,
                 page.label.ex.rfw.t2,
                 page.label.ex.rfw.t3);
      }

      if (page.label.ex.rfw.t1 ^ 'V')
      {
         if (flag['v'-'a'])
         {
            index = 0;
            loader = (char *) &page;

            while (xx--)
            {
               if ((index & 15) == 0) printf("\n%4.4x: ", index);
               printf("%2.2x", *loader++);
               index++;
            }

            putchar('\n');
         }

         return;
      }

      xx = 3 * (page.label.ex.rfw.t3 - VOLUME1_WORDS);
      banks = msw2i(page.label.ex.granules);

      printf("%.*s %d storage banks\n", xx, &page.label.name[0].t1, banks);
      if (banks == 0) return;

      if (loader) free(loader);

      loader = (char *) calloc(banks, sizeof(fsbank));

      if (loader == NULL)
      {
         printf("%d unable to cache file system\n", errno);
         return;
      }

//      devices[device_id].flags = DEVICE | FSYS24;
      devices[device_id].dev24 = (fsbank *) loader;
      
      while (cores--)
      {
         ascor->BASE[128 + device_id] = FSYS24_FLAG | (banks - 1);
         ascor++;
      }

      memcpy(loader, (char *) &page, sizeof(page));
      loader += sizeof(page);
      index = DIRECTORY_BLOCK / GRANULE;

      for (;;)
      {
         xx = read(f, loader, 192);
         if (xx < 0) break;
         if (!xx) break;
         index++;
         loader += 192;
      }

      close(f);
      printf("fs %d %d granules loaded\n", device_id, index);
   }
}

void assign_array(int device_id, char *text)
{
   int		 banks;
   long		 words;

   char		*where;

   int 		 cores = RTA_SMP;
   smp		*ascor = core;

   int		 array_attributes;

   sscanf(text, "%d", &banks);

   if ((banks < 1) || banks > 65536)
   {
      printf("+storage banks requested must be decimal 1..65536\n");
      return;
   }

   words = (long) banks << 18;
   where = malloc(words << 2);

   if (where)
   {
//      devices[device_id].flags = DEVICE | SYSMEM;
      devices[device_id].pages = (page *) where;
      array_attributes = SYSMEM_FLAG | ((banks << 6) - 1);

      while (cores--)
      {
         ascor->BASE[128 + device_id] = array_attributes;
         ascor++;
      }

      printf("device %d %ld words memory array added\n", device_id, words);
   }
   else
   {
      printf("device %d requested %ld octets are not available\n", device_id, words << 2);
   }
}

static void statement(smp *xcore)
{
   int                   index = iselect;
   int                   index2;
   int			*register_set = xcore->register_set;

   #ifdef __X64
   #define FORMAT2 "%6.6x %12.12lx\n"
   #else
   #define FORMAT2 "%6.6x %12.12x\n"
   #endif

   #define RED             "\033[91m"
   #define PRIMARY         "\033[0m"

   printf("[" RED "%d" PRIMARY "]\n", base[77]);

   instruction_display(apc - 1, 1, flag['l'-'a'], xcore);
   if (flag['e'-'a']) printf("[RP %p]", register_set);
   printf(FORMAT2, psr, (word *) apc - ROM_PAGE->w);
   instruction_display(apc, 6, flag['l'-'a'], xcore);

   while (index < (iselect | 24))
   {
      print_register_row(index, xcore);
      putchar('\n');
      index += 8;
   }

   #if 1

   index = sp;
   printf("[%6.6x]->", index);

   if (index & 127) index = print7_registers(index, xcore);

   if (index & 127)
   {
      putchar('\n');
      index = print_register_row(index, xcore);
   }

   if (index & 127)
   {  
      putchar('\n');
      index = print_register_row(index, xcore);
   }

   putchar('\n');
   register_pointer = index;

   #else

   index = sp;
   printf("[%6.6x]->", index);

   index2 = 7;

   while (index2--)
   {
      if (index > 255) break;
      printf(" %6.6x", _register[index++]);
   }

   printf("\n   ");

   index2 = 8;

   while (index2--)
   {
      if (index > 255) break;
      printf(" %6.6x", _register[index++]);
   }

   printf("\n   ");

   index2 = 8;

   while (index2--)
   {
      if (index > 255) break;
      printf(" %6.6x", _register[index++]);
   }
   putchar('\n');

   #endif
}

static int msw2i(msw w)
{
   return (w.t1 << 16) | (w.t2 << 8) | w.t3;
}

#ifdef METRIC
static void accumulate_metric(smp *xcore)
{
   /*******************************************************
	this happens at leading edge of emulation sleep
	requested by I/O chilldown indication
   *******************************************************/

   struct timeval	 time3;
   unsigned long long	 trailing_edge;

   gettimeofday(&time3, NULL);
   trailing_edge = time3.tv_sec * 1000000 + time3.tv_usec;

   delta = trailing_edge - delta_base;
   total_metric += metric;
   metric = 0;
   total_delta += delta;
}
#endif
