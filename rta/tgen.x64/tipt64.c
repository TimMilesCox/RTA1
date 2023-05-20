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
#endif

#include "../include.rta/argue.h"
#include "../engine.rta/emulate.h"
#include "../engine.rta/ii.h"
#include "../engine.fs/device24.h"
#include "../rta.run/idisplay.h"
#include "../rta.run/settings.h"
#include "../rta.run/tzone.h"
#include "../rta.run/stepping.h"

#ifndef	__X64
#include "../rta.run/time32.h"
#endif

#ifdef	GCC
#else
#ifdef	LINUX
#include "../tgen.x64/_mnames.h"
#endif
#endif

#define ROM_PAGE	&memory.p4k[0].w[0]

#ifndef __X64
static step_second32		 step_second;
#endif

static unsigned int		 clockr[2];

static long			 interval_seconds_mask;
static int			 platform_interval = PLATFORM_INTERVAL;

/********************************************************

	global data whether here or extern

********************************************************/

#include "../tgen.x64/ioports.c"

extern device			 devices[];		/* in rw */
extern int			 register_pointer;	/* in console */
extern system_memory		 memory;

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
device				*pdevice = devices;

#ifdef METRIC
unsigned int			 delta,
				 metric;

unsigned long long		 delta_base,
				 total_delta,
				 total_metric;
#endif

/********************************************************

	system_memory must be last
	because some compilers can only see the front of it 

	end global data

	system memory is link-included last as memory.o

********************************************************/

/********************************************************
	only protoypes from here
********************************************************/

#ifdef	GCC
#include "../engine.rta/rw.h"
#else
static word		 memory_read(int ea);
static int device_array_read(int device_index, int block, int offset);
extern void		 device_read();
extern void		 bus_read();
#endif

extern void              netbank();

#ifdef	GCC_CC
extern void		 execute(word instruction);
#else
extern void		 leloup();
#endif

static void		*emulate();
static void		 statement();
extern void		 action(char request[]);
void			 load_fs(int device_id, char *path);
extern void		 assign_interface_relay(int device_id, char *text);
       void		 assign_array(int device_id, char *text);
static int		 msw2i(msw w);
extern int		 print_register_row(int index);
extern			 int print7_registers(int index);

#ifdef	METRIC
static void		 accumulate_metric();
#endif

#include "../tgen.x64/dayclock.c"

int main(int argc, char *argv[])
{
   int			 _x,
			 _y;

   int			 fields,
			 device_id;

   char			 text[72];
   char			*_p;

   #ifdef X86_MSW
   #else
   #define MID_PRIORITY	50

   pthread_attr_t        asyncb;
   pthread_t             asyncid;
   struct sched_param	 asyncp = { MID_PRIORITY } ;

   #endif

   int                   f, count, image_size = 0;

   word                 *load_address = memory.p4k[0].w;
   word                  data_word = { 0, 0, 0, 0 } ;

   argue(argc, argv);


   printf("RTA1 " RADIX " emulation stepping " STEPPING "\n");

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
      base[124] = image_size >> 12;             /*      ROM boundary port       */
      base[128] = (PAGES_IN_MEMORY - 1) | SYSMEM_FLAG;
						/*      system memory size port */

      close(f);
   }
   else
   {
      printf("rom image file required\n");
      return 0;
   }

   netbank();

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

   #ifdef X86_MSW

   _x = _beginthread(async, 0, NULL);
   if (_x < 0) printf("async thread start %d %d\n", _x, errno);
   else       printf("async thread ID %x\n", _x);

   #else

   _x = pthread_attr_init(&asyncb);

   if (_x < 0) printf("threadcbinit %d e %d\n", _x, errno);
   else
   {
      _x = pthread_create(&asyncid, &asyncb, &emulate, NULL);
      if (_x < 0) printf("async thread start %d %d\n", _x, errno);
      else
      {
         printf("async thread ID %p\n", asyncid);

         if (uflag['O'-'A'])
         {
            /******************************************************************

	       keeps the MIPs reading steady when doing many slower instructions
               which give SCHED_RR more chance of intervening

            ******************************************************************/

            _x = pthread_setschedparam(asyncid, SCHED_FIFO, &asyncp);
            if (_x < 0) printf("switch FIFO E%d\n", errno);

            _x = pthread_getschedparam(asyncid, &_y, &asyncp);

            if (_x < 0) printf("(E%d)", errno);
            printf("[PY %d S-P %d]\n", _y, asyncp.sched_priority);
         }
      }
   }

   #endif

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

            if (indication & LOCKSTEP) printf("command>");
            else                       printf("emulator running\n"
                                              "key s for single step:");

            fflush(stdout);

            continue;
         }

         action(_p);
   }

   return 0;
}

void *emulate()	/* thread start */
{
   #ifdef METRIC
   struct timeval	 time2;
   #endif

   #ifdef RATIO
   int			 ratio;
   #endif

   printf("emulation start\n");

   dayclock(1);

   printf("[%6.6x:%6.6x %6.6x:%6.6x]\n", clockr[0], clockr[1], _register[DAYCLOCK_U], _register[DAYCLOCK]);

   for (;;)
   {
      #ifdef __X64
      #define FORMAT1 "[%x %p %x %p %lx]\n"
      #else
      #define FORMAT1 "[%x %p %x %p %x]\n"
      #endif

      if (flag['s'-'a']) indication |= LOCKSTEP;
      if (flag['e'-'a']) printf(FORMAT1,
                                indication, register_set, psr, apc,
                                apc - memory.p4k[0].w);

      #ifdef METRIC
      gettimeofday(&time2, NULL);
      delta_base = time2.tv_sec * 1000000 + time2.tv_usec;
      #endif

      /*********************************************************
	time and emulated instructions
        are measured from start of this block
        to end of this block
      *********************************************************/

      #ifdef GCC_CC
      #ifdef RATIO
      ratio = RATIO;
      #endif

      for (;;)
      {
         if (indication & (EXTERNAL_INDICATIONS))
         {
            if ((psr & 0x00070000) ^ 0x00070000) xi();
         }
         
         #ifdef INSTRUCTION_U
         if (apc > apcz)
         {
            GUARD_INTERRUPT;
         }
         #endif

         execute(*apc++);

         #ifdef RATIO
         ratio--;
         if (ratio < 0) indication |= TIME_UPDATE;
         #else
         #ifdef METRIC
         metric++;
         #endif
         #endif

         if (indication & (CHILLDOWN|TIME_UPDATE|LOCKSTEP|BREAKPOINT)) break;
      }
      #else	/*	machine code not GCC	*/

      #ifdef RATIO
      _register[283] = RATIO;
      #endif

      leloup();

      #ifdef RATIO
      ratio = _register[283];
      #endif

      #endif

      #ifdef METRIC
      #ifdef RATIO
      metric = RATIO - ratio;
      #endif
      accumulate_metric();
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
		chilldown is ordered from an spplication
		so dayclock(0) zero argument takes effect
         ***********************************************/
      }

      if (indication & TIME_UPDATE)
      {
         indication &= -1 ^ TIME_UPDATE;

         #ifdef RATIO

         dayclock(0);

         #else

         _register[DAYCLOCK]   = clockr[1];
         _register[DAYCLOCK_U] = clockr[0];

         #endif
      }

      if (indication & LOCKSTEP) flag['s'-'a'] = 1;

      if (indication & BREAKPOINT)
      {
         if (apc == breakpoint)
         {
            flag['s'-'a'] = 1;
            indication |= LOCKSTEP;
         }
      }

      if ((indication & LOCKSTEP) == 0) continue;

      statement();

      putchar('>');
      fflush(stdout);

      while (indication & LOCKSTEP) usleep(platform_interval);
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
      base[128 + device_id] = FSYS24_FLAG | (banks - 1);

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
      base[128 + device_id] = SYSMEM_FLAG | ((banks << 6) - 1);
      printf("device %d %ld words memory array added\n", device_id, words);
   }
   else
   {
      printf("device %d requested %ld octets are not available\n", device_id, words << 2);
   }
}

static void statement()
{
   int                   index = iselect;
   int                   index2;

   #ifdef __X64
   #define FORMAT2 "%6.6x %12.12lx\n"
   #else
   #define FORMAT2 "%6.6x %12.12x\n"
   #endif

   instruction_display(apc - 1, 1, flag['l'-'a']);
   if (flag['e'-'a']) printf("[RP %p]", register_set);
   printf(FORMAT2, psr, (word *) apc - memory.p4k[0].w);
   instruction_display(apc, 6, flag['l'-'a']);

   while (index < (iselect | 24))
   {
      print_register_row(index);
      putchar('\n');
      index += 8;
   }

   #if 1

   index = sp;
   printf("[%6.6x]->", index);

   if (index & 127) index = print7_registers(index);

   if (index & 127)
   {
      putchar('\n');
      index = print_register_row(index);
   }

   if (index & 127)
   {  
      putchar('\n');
      index = print_register_row(index);
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
static void accumulate_metric()
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
