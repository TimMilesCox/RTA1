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

#define ROM_PAGE	&memory.p4k[0].w[0]

static word		*breakpoint;

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
unsigned int		*register_set = _register+128;
extern unsigned int	 base[];
extern device		 devices[];
extern system_memory	 memory;

#else

system_memory            memory;

int              	 iselect = 128;
word		    	*apc = ROM_PAGE;

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
// device           	 devices[64];
#include "ioports.c"
#include "devices.c"
device			*pdevice = devices;

#endif

#ifdef	GCC
extern word		 memory_read(int ea);
extern int		 device_read(int device_index, int relocation_base, int offset);
#else
static word		 memory_read(int ea);
static int device_array_read(int descriptor, int offset);
extern void		 device_read();
extern void		 bus_read();
#endif
//	extern int               bus_read(int device, int pointer);
extern void              netbank();

#ifdef	GCC
extern void		 execute(word instruction);
#else
extern void		 leloup();
#endif

static void		*emulate();
static void		 statement();
static void		 action(char request[]);
static void		 load_fs(int device_id, char *path);
extern void		 assign_interface_relay(int device_id, char *text);
static void		 assign_array(int device_id, char *text);
static int		 msw2i(msw w);
static void		 print_register_row(int index);

#ifdef METRIC
unsigned int		 delta,
			 metric;

static unsigned long	 delta_base,
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


int main(int argc, char *argv[])
{
   int			 _x;

   int			 fields,
			 device_id;

   char			 text[72];
   char			*_p;

   #ifdef X86_MSW
   #else
   pthread_attr_t        asyncb;
   pthread_t             asyncid;
   #endif

   int                   f, count, image_size = 0;

   word                 *load_address = memory.array;
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
      else       printf("async thread ID %p\n", asyncid);
   }

   #endif

   printf("key %s\n\n", (flag['s'-'a'])
                        ? "g[break:point] to run"
                        : "s to enter single step");
   #ifndef __X64
   if (uflag['Z'-'A'] == 0) start_second(&step_second);
   #endif

   for (;;)
   {
      #ifdef X86_MSW
      _x = _kbhit(1);
      #else
      _x = poll(&attention, 1, 3);

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

         if (*_p == '.') break;

         action(_p);

         #if 0
         continue;
         #endif
      }

      #ifdef X86_MSW
      Sleep(1000);
      u = GetTickCount64();
      #else

      usleep(1000);
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

      if (uflag['Z'-'A'] == 0)
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
      if (flag['s'-'a']) indication |= LOCKSTEP;
      if (flag['e'-'a']) printf("[%x %p %x %p %lx]\n",
                                 indication, register_set, psr, apc,
                                 apc - memory.array);

      #ifdef GCC
      for (;;)
      {
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
      __asm__
      {
		call	leloup 
      }
      #endif

      if (indication & CHILLDOWN)
      {
         #ifdef METRIC
         accumulate_metric();
         #endif

         indication &= -1 ^ CHILLDOWN;
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

      while (indication & LOCKSTEP) usleep(10000);

      #ifdef METRIC
      gettimeofday(&time2, NULL);
      delta_base = time2.tv_sec * 1000000 + time2.tv_usec;
      #endif
   } 
}

static void action(char request[])
{
   word			 sample;
   word			*departure;

   int			 xx,
			 device_id,
                         base_tag,
                         base_index,
                         offset;

   long			 index,
                         absolute;

   int			 symbol = request[0];
   int			 prompt = 0;

   char			 path[360];


   #ifdef METRIC
   double		 average,
                         quantum;

   if (symbol == 'i')
   {
      average = total_metric;
      quantum = total_delta;
      average /= quantum;
      printf("instructions %ld usecs %ld approximate MIPS %f\n",
              total_metric, total_delta, average);

      putchar(':');
      fflush(stdout);
      return;
   }
   #endif

   if (flag['s'-'a'] == 0)
   {
      if (symbol ^ 's')
      {
         printf("key s for single step\n");
         return;
      }
   }

   prompt = 1;

   switch(symbol)
   {
      case 's':
	 indication |= LOCKSTEP;
         flag['s'-'a'] = 1;
         break;

      case 'g':
         xx = sscanf(request + 1, "%lx:%x", &index, &offset);

         if (xx == 1) breakpoint = &b0p->w[index];
         if (xx == 2) breakpoint = &memory.p4k[index].w[offset];

         flag['s'-'a'] = 0;
         indication &= -1 ^ LOCKSTEP;

         /***************************************************
		some systems give -1 with errno = 0
		when stream terminates at 1st byte
         ***************************************************/

         if (xx < 1) xx = 0;

         if (xx) indication |= BREAKPOINT;

         if (breakpoint)
         {
            if (flag['v'-'a']) printf("[>%x,%lx]",
                                       indication, breakpoint - memory.array);
            if (flag['e'-'a']) printf("[@%p:%p]", memory.array, breakpoint);
         }

         prompt = 0;
         putchar(':');
         fflush(stdout);
         break;

      case '+':
         index = sp;
         index += 23;
      case '0':
      case 'r':
         if (symbol ^ '+') index = iselect | 24;

         if      (symbol == '0') sscanf(request,     "%lx", &index);
         else if (symbol == 'r') sscanf(request + 1, "%ld", &index);

         while (index < REGISTERS)
         {
            print_register_row(index);
            index += 8;
            fgets(request, 48, stdin);
            if (request[0] == '.') break;
         }

         break;

      case 'm':

         xx = sscanf(&request[1], "%x:%x", &base_index, &offset);

         if (xx < 2)
         {
            offset = base_index;

            for (;;)
            {
               index = offset + 8;

               printf("%6.6x :", offset);

               while (offset < index)
               {
                  sample = memory_read(offset++);
                  printf(" %2.2x%2.2x%2.2x", sample.t1, sample.t2, sample.t3);
               }

               fgets(request, 48, stdin);
               if (request[0] == '.') break;
            }
         }
         else
         {
            absolute = (long) (base_index << 12) + offset;

            for (;;)
            {
               index = absolute + 8;
               printf("%6.6x:%6.6x :", base_index, offset);
               offset += 8;

               while (absolute < index)
               {
                  if (absolute > (WORDS_IN_MEMORY - 1)) break;
                  sample = memory.array[absolute++];
                  printf(" %2.2x%2.2x%2.2x", sample.t1, sample.t2, sample.t3);
               }

               fgets(request, 48, stdin);
               if (request[0] == '.') break;
            }
         }

         break;

      case 'd':

	/*****************************************************
		for viewing peripheral device arrays
	*****************************************************/

         absolute = 0;		/* the device or the offset */
         offset = 0;
         base_tag = 0;

         xx = sscanf(request + 1, "%lx:%x", &absolute, &offset);

         if (xx < 2)
         {
            offset = absolute;	/* one or zero values read	*/
            absolute = 0;	/* device comes from base_index	*/

            if (base_tag = offset & 0x00FC0000)
            {
               base_tag >>= 18;
               offset &= 0x0003FFFF;
            }
            else
            {
               base_tag = offset & 0x0003F000;
               base_tag >>= 12;
               offset &= 0x00000FFF;
            }

            if ((base_tag)
            &&  (base_tag < 8)
            &&  (psr & (32768 >> base_tag))) base_tag += 64;

            base_index = base[base_tag];
            if (base_index & 0x00400000) absolute = base_index & 63;
//            printf("[%x %x %x %x]\n", absolute, base_tag, base_index, offset);
         }
         else base_index = 0x400000 | absolute;

         for (;;)
         {
            printf("%6.6x:%6.6x :", base_index, offset);
            index = offset + 8;

            while (offset < index)
            {
               #ifdef GCC
               xx = device_read(absolute, base_index, offset++);
               #else
               xx = device_array_read(base_index, offset++);
               #endif

               printf(" %6.6x", xx);
            } 

            fgets(request, 48, stdin);
            if (request[0] == '.') break;
         }

         break;

      case 'b':
         for (xx = 0; xx < 72; xx++)
         {
            if (!(xx & 7)) printf("\n%2.2x:", xx);
            printf(" %6.6x", base[xx]);
         }

         printf("\n\n60:");
         for (xx = 96; xx < 104; xx++) printf(" %6.6x", base[xx]);

         printf("\n\n7c                             ");

         for (xx = 124; xx < 192; xx++)
         {
            if (!(xx & 7)) printf("\n%2.2x:", xx);
            printf(" %6.6x", base[xx]);
         }

         putchar('\n');

         break;

      case 'l':
         xx = sscanf(&request[1], "%d %s", &device_id, path);
         if (xx == 2) load_fs(device_id, path);
         else printf("load fs requires: l device fsimage_path\n");
         break;

      case 'h':
         printf("LF\texecute one instruction and display new state\n");
         printf("+\tdisplay more internal stack registers\n");
         printf("0n\tdisplay registers starting at hex address\n");
         printf("r n\tdisplay registers starting at decimal address\n");
         printf("b\tdisplay relocation / configuration ports\n\n");
         printf("m [[page:]offset]\tdisplay system memory\n");
         printf("\tdefault page[s] in current address space\n\n");
         printf("d [[device:]offset]\tdisplay peripheral memory array[s]\n");
         printf("\tdefault device[s] in current address space\n\n");
         printf("g [[page:]breakpoint]\trun [to breakpoint]\n");
         printf("\tdefault page is current instruction section\n");
         printf("\tto remove breakpoint g0:0\n\n");
         printf(".\texit emulator\n");
         break;

      case '.':
         prompt = 0;
         indication &= -1 ^ LOCKSTEP;
         break;

      default:
         prompt = 0;
         indication &= -1 ^ LOCKSTEP;
         break;
   }

   #if 1
   if (prompt)
   {
      putchar('>');
      fflush(stdout);
   }
   #endif
}

static void load_fs(int device_id, char *path)
{
   int		 xx,
		 banks,
		 index = 0;

   char		*loader = (char *) devices[device_id].s.dev24;

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

      devices[device_id].flags = DEVICE | FSYS24;
      devices[device_id].s.dev24 = (device24 *) loader;
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

static void assign_array(int device_id, char *text)
{
   int		 banks;
   long		 words;

   char		*where;

   sscanf(text + 1, "%d", &banks);
   words = banks << 18;
   where = malloc(words << 2);

   if (where)
   {
      devices[device_id].flags = DEVICE | SYSMEM;
      devices[device_id].s.pages = (system_memory *) where;
      base[128 + device_id] = SYSMEM_FLAG | ((banks << 6) - 1);
      printf("device %d additional %ld words memory array added\n", device_id, words);
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

   instruction_display(apc - 1, 1, flag['l'-'a']);
   if (flag['e'-'a']) printf("[RP %p]", register_set);
   printf("%6.6x %12.12lx\n", psr, (word *) apc - memory.array);
   instruction_display(apc, 6, flag['l'-'a']);

   while (index < (iselect | 24))
   {
      print_register_row(index);
      putchar('\n');
      index += 8;
   }

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
}

static int msw2i(msw w)
{
   return (w.t1 << 16) | (w.t2 << 8) | w.t3;
}

static void print_register_row(int index)
{
   int           xx = 8;

   if (flag['e'-'a']) printf("[%p]", _register);
   printf("%2.2x:", index);
   while ((xx--) && (index < REGISTERS))
   {
      if (index < 256) printf(" %6.6x", _register[index++]);
      else             printf(" %8.8x", _register[index++]);
   }
}


/********************************************
	readout target current address space
********************************************/

#ifndef	GCC
static word memory_read(int ea)
{
   word		 data;

   __asm__
   {
	mov	eax, ea
	push	ecx
	mov	ecx, 0
	call	bus_read
	pop	ecx
	bswap	eax
	mov	data, eax
   }

   return data;
}

static int device_array_read(int index, int offset)
{
   int		 data,
		 descriptor = index,
		 pointer = offset;

   /*****************************************************
	note
	some gcc releases will fall apart
	if you reference function arguments from asm block
	so massage the arguments in dynamic variables
   *****************************************************/

   __asm__
   {
	push	ecx
	mov	ecx, esi
	push	ecx
	xor	ecx, ecx
	mov 	eax, descriptor
	mov	esi, pointer
	call	device_read
	pop	ecx
	mov	esi, ecx
	pop	ecx
	mov	data, eax
   }

   return data;
}
#endif

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
