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

word			*breakpoint;

int			 indication;

#ifndef	X86_MSW
static struct pollfd	 attention = { 0, POLLIN } ;
#endif

#ifndef	X86_MSW
struct timeval		 xronos;
#endif

#ifdef	GCC

extern int		 iselect;
extern word		*apc;

#ifdef	INSTRUCTION_U
extern word		*apcz;
#endif

extern page		*b0p;
extern unsigned int	 b0_name;
extern unsigned int	 psr;
extern unsigned int	_register[];
extern int		*register_set;
extern unsigned int	 base[];
extern device		 devices[];
extern system_memory	 memory;

#else

system_memory            memory;

int              	 iselect = 128;
int			 iselectu = 256;
word		    	*apc = ROM_PAGE;
word			*apcu;

#ifdef	INSTRUCTION_U
word			*apcz = &memory.p4k[0].w[4095];
#endif

page            	*b0p  = memory.p4k;
unsigned int		 b0_name;
unsigned int    	 psr = 0x00800000;
unsigned int    	 _register[REGISTERS];
 
unsigned int		*register_set = _register+128;
// unsigned int     	 base[192];
// system_memory    	 memory;
extern device          	 devices[64];
#include "../tgen.x64/ioports.c"
// device                           devices[64] = { { memory.p4k } } ;
device			*pdevice = devices;

#endif

#ifdef	GCC
#include "../engine.rta/rw.h"
#else
static word		 memory_read(int ea);
static int device_array_read(int device_index, int block, int offset);
extern void		 device_read();
extern void		 bus_read();
#endif

extern void              netbank();

#ifdef	GCC
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
extern int		 register_pointer;

#ifdef METRIC
unsigned int		 delta,
			 metric;

unsigned long long	 delta_base,
			 total_delta,
			 total_metric;

static void		 accumulate_metric();
#endif

#ifdef	__X64
static unsigned long	 dayclock_update;
static long		 clockwise;
#else
static unsigned long long dayclock_update;
static step_second32	 step_second;
static unsigned 	 clockr[2];
#endif

static int		 interval_seconds_mask;
int			 platform_interval = PLATFORM_INTERVAL;
int			 dayclock_revision = DAYCLOCK_REVISION;
int			 dayclock_increment = DAYCLOCK_REVISION;

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
      #ifdef X86_MSW
      _x = _kbhit(1);
      #else
      _x = poll(&attention, 1, 0);

      /**************************************************
	poll is immediate return
	usleep(dayclock_revision) is microsecond tunable
      **************************************************/

      if (_x < 0)
      {
         printf("problem with input request %d. exit emulator\n", errno);
         break;
      }
      else
      #endif

      if (_x)
      {
         #if 0
         /************************************************
            this is examined properly in function action()
            sometimes poll() produces an unimportant event 
         ************************************************/
         flag['s'-'a'] = 1;
         #endif

         #if 0
         putchar('>');
         fflush(stdout);
         #endif

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

         #if 0
         continue;
         #endif
      }

      #ifdef X86_MSW
      Sleep(1000);
      u = GetTickCount64();
      #else

      usleep(dayclock_revision);
      dayclock_revision = dayclock_increment;

      /*****************************************************
	instruction emulator may have lengthened last usleep
        by out to powersave port 103
	change to millisecond tact or similar when awaking
      *****************************************************/

      if (psr & 0x00800000) continue;

      gettimeofday(&xronos, NULL);

      #ifdef __X64

      clockwise = (xronos.tv_sec  * 1000)
                + (xronos.tv_usec / 1000);

      
      dayclock_update = ((clockwise >> 24) & 0x00FFFFFF)
                      | ((clockwise & 0x00FFFFFF) << 32);

      /******************************************************
        the integer modelling dayclock left DAYCLOCK$U
	is physically on the left, DAYCLOCK$ is after hee.
        RTA1 target registers are modelled platform endian.
        64-bit write of 00ABCDEF must store CBA0.FED0
        target "registers" are storage locations until bus read
      ******************************************************/

      #else


      /*****************************************************
	for 32-bit execution with unchanged library
	on platforms the same age as this emulator or newer

	it's not known if tv_sec will really wrap for ever
	because systems may be set to do diagnostics instead

	but if tv_sec does keep wrapping:
	
	this tweak casts tv_sec to unsigned and prepends
	32 more bits which are all zeros until year 2110 approx.

	the 64-bit seconds-from-1970 count is store on file
	every 70 years when tv_sec high-order bit flips. 

	On unsigned overflow every 140 years
	the 32-bit prepend is incremented

	The 2 words on file are read every emulator startup
	and written once per 70 years
      *****************************************************/

      if (uflag['Y'-'A'] == 0)
      {
         if ((xronos.tv_sec & 0x80000000) ^ (step_second.low & 0x80000000))
         {
            /**************************************************
               ms bit of tv_sec has changed
               change is 1 to 0 approximately every 140 years
               and must carry into the high number half
            ***************************************************/

            if (step_second.low & 0x80000000) step_second.high++;
            step_second.low = (unsigned) xronos.tv_sec;
            store_second(&step_second);
         }
      }

      clockr[1] = (unsigned) xronos.tv_usec / 1000
                + (xronos.tv_sec & 0xFFFF)  * 1000;

      clockr[0] = ((unsigned) xronos.tv_sec >> 16) * 1000
                + ((step_second.high * 1000) << 16)
                + (clockr[1] >> 16);

      clockr[1] = (clockr[1] & 0xFFFF)
                | ((clockr[0] & 255) << 16);

      clockr[0] = (clockr[0] >>= 8) & 0x00FFFFFF;

      /*	store in emulated register stack
		in 1 or 2 instructions in emulate() thread	*/

      dayclock_update = (* (unsigned long long *) clockr);

      #endif	/*	__X64	*/

      /****************************************************************
	 no need for time zone update more often than 2 second interval
         it might do some GPS I/O 
      *****************************************************************/

      if ((xronos.tv_sec ^ interval_seconds_mask) & 0xFFFFFFFE) tzone();
      interval_seconds_mask = xronos.tv_sec;

      #endif	/*	POSIX or WINDOWS	*/

      indication |= TIME_UPDATE;

      /****************************************************************
         let instructions emulation thread copy the dayclock update
      ****************************************************************/
   }

   return 0;
}

void *emulate()	/* thread start */
{
   #ifdef METRIC
   struct timeval	 time2;
   #endif

   #ifdef LP_TSLICE_HERE
   /* It's not. It's in engine.rta/rta.c:execute() */
   int			 icount;
   #endif

   printf("emulation start\n");

   #ifdef METRIC
   gettimeofday(&time2, NULL);
   delta_base = time2.tv_sec * 1000000 + time2.tv_usec;
   #endif

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

      #ifdef GCC
      for (;;)
      {
         if (indication & (EXTERNAL_INDICATIONS)) xi();
         
         #ifdef INSTRUCTION_U
         if (apc > apcz)
         {
            GUARD_INTERRUPT;
         }
         #endif

         execute(*apc++);

         #ifdef METRIC
         metric++;
         #endif

         #ifdef LP_TSLICE_HERE
         /* It's not. It's in engine.rta/rta.c:execute() */

         if (psr & 0x00870000)
         {
            /***************************************************
                not during interrupt
                or with interrupt lock
            ***************************************************/
         }
         else
         {
            if (icount = _register[REALTIME_CLOCK] & 0x00FFFFFF)
            {
               icount--;
               _register[REALTIME_CLOCK] = icount;
               if (icount == 0) ii(YIELD,LP_TSLICE);
            }
         }
         #endif

         if (indication & (CHILLDOWN|TIME_UPDATE|LOCKSTEP|BREAKPOINT)) break;
      }
      #else
      #if 1
      leloup();
      #else
      __asm__
      {
		call	leloup 
      }
      #endif
      #endif

      if (indication & CHILLDOWN)
      {
         #ifdef METRIC
         accumulate_metric();
         #endif

         indication &= -1 ^ CHILLDOWN;
         dayclock_revision = base[103] * 7 / 8;
         usleep(base[103]);

         #ifdef METRIC
         gettimeofday(&time2, NULL);
         delta_base = time2.tv_sec * 1000000 + time2.tv_usec;
         #endif
      }

      if (indication & TIME_UPDATE)
      {
         #ifdef __X64
         *((unsigned long *) (_register + DAYCLOCK_U)) = dayclock_update;
         #else
         *((unsigned long long *) (_register + DAYCLOCK_U)) = dayclock_update;
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

      #ifdef METRIC
      accumulate_metric();
      #endif

      statement();

      #if 1
      putchar('>');
      fflush(stdout);
      #endif

      while (indication & LOCKSTEP) usleep(platform_interval);

      #ifdef METRIC
      gettimeofday(&time2, NULL);
      delta_base = time2.tv_sec * 1000000 + time2.tv_usec;
      #endif
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
