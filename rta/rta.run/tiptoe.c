/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is the instruction-executor of the software-emulation
    of the freeware processor architecture

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

    You can redistribute it and/or modify RTA1
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



#define	ASYNC

#ifdef	X86_MSW

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
//	#include <signal.h>
//	#include <sys/time.h>
#include <errno.h>

#ifdef  ASYNC
#include <process.h>
#endif

#else	X86_MSW

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>

#ifdef	ASYNC
#include <pthread.h>
#endif

#endif	X86_MSW

#include "../engine.rta/emulate.h"
#include "../engine.rta/ii.h"
#include "../engine.fs/device24.h"
#include "idisplay.h"
#include "settings.h"
#include "../include.rta/argue.h"
#include "stepping.h"

int			 indication;

extern int		 iselect;
extern word		*apc;
extern page		*b0p;
extern unsigned int	 psr;
extern unsigned int	_register[];
extern unsigned int	 base[];
extern system_memory	 memory;
extern device		 devices[];

static word		*breakpoint;

static int		 runout;

extern void		 execute(word instruction);
extern word		 memory_read(int ea);
extern void		 netbank();

static void		 statement();
static void		 action(char *request);
static void		 load_fs(char *path);

#ifdef METRIC
unsigned int             delta,
                         metric;

static unsigned long long        delta_base,
                                 total_delta,
                                 total_metric;

static void              accumulate_metric();
#endif

#ifdef ASYNC

static void *async()
{
   char			 request[372];
   char			*_p;

   #ifdef X86_MSW
   #else
   funlockfile(stdin);
   #endif

   if (flag['s'-'a'] == 0)
   {
      putchar(':');
      fflush(stdout);
   }

   for (;;)
   {
      if (flag['s'-'a'])
      {
         putchar('>');
         fflush(stdout);
      }

      request[0] = 0;
      _p = fgets(request, 360, stdin);

      if      (_p) action(_p);
      else if (request[0]) action(request);
      else printf("please key console input again\n");

      if (flag['v'-'a'])
      printf("[%x %p %x %x]\n", flag['s'-'a'], _p, runout, indication);
   }
}

#endif

static int msw2i(msw w)
{
   return (w.t1 << 16) | (w.t2 << 8) | w.t3;
}

static void print_register_row(int index)
{
   #if 1
   int		 xx = 8;

   printf("%2.2x:", index);
   while ((xx--) && (index < REGISTERS))
   {
      if (index < 256) printf(" %6.6x", _register[index++]);
      else             printf(" %8.8x", _register[index++]);
   }
   
   #else
   printf("%2.2x: %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x",
           index,
           _register[index],   _register[index+1],
           _register[index+2], _register[index+3],
           _register[index+4], _register[index+5],
           _register[index+6], _register[index+7]);
   #endif
}

static void statement()
{
   int			 index = iselect;
   int			 index2;

   instruction_display(apc - 1, 1, flag['l'-'a']);
   printf("%6.6x %8.8x\n", psr, apc - memory.array);
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

int main(int argc, char *argv[])
{
   #ifdef DAYCLOCK

   #ifdef X86_MSW
   SYSTEMTIME		 stime;
   FILETIME		 ftime;
   #else
   struct timeval	 time;
   #endif

   int			 instructions = INTERVAL;
   int			 imask;
   #endif


   int			 _x;

   #ifdef DAYCLOCK
   unsigned long long	 time_pointer;
   #endif

   #ifdef LP_TSLICE_HERE
   /* It's not. It's in engine.rta/rta.c:execute() */
   int			 icount;
   #endif

   char			 command[84];

   int			 index = 1;
   int			 offset;
   int			 symbol;
   char			*file;

   int			 f, count, image_size = 0;

   word			*load_address = memory.array;
   word			 data_word = { 0, 0, 0, 0 } ;

   #ifdef METRIC
   struct timeval        time2;
   #endif

   #ifdef ASYNC

   #ifdef X86_MSW
   #else

   pthread_attr_t	 asyncb;
   pthread_t		 asyncid;

   #endif
   #endif


   argue(argc, argv);

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
      base[124] = image_size >> 12;		/*	ROM boundary port	*/
      base[128] = PAGES_IN_MEMORY - 1;		/*	system memory size port	*/

      close(f);
   }
   else
   {
      printf("rom image file required\n");
      return 0;
   }

   netbank();

   if (arguments > 1) load_fs(argument[1]);

   #ifdef ASYNC

   #ifdef X86_MSW

   _x = _beginthread(async, 0, NULL);
   if (_x < 0) printf("async thread start %d %d\n", _x, errno);
   else        printf("async thread ID %x\n", _x);

   #else

   _x = pthread_attr_init(&asyncb);

   if (_x < 0) printf("threadcbinit %d e %d\n", _x, errno);
   else
   {
      _x = pthread_create(&asyncid, &asyncb, &async, NULL);
      if (_x < 0) printf("async thread start %d %d\n", _x, errno);
      else        printf("async thread ID %p\n", asyncid);
   }

   #endif
   #endif

   printf("key %s\n\n", (flag['s'-'a'])
                        ? "g[break:point] to run"
                        : "s to enter single step");

   #ifdef METRIC
   gettimeofday(&time2, NULL);
   delta_base = time2.tv_sec * 1000000 + time2.tv_usec;
   #endif

   for (;;)
   {
      if (runout < 0) break;

      execute(*apc++);

      #ifdef METRIC
      metric++;
      #endif

      #ifdef DAYCLOCK

      instructions--;
      if (!instructions)
      {
         instructions = INTERVAL;

         #if X86_MSW

         GetLocalTime(&stime);
         SystemTimeToFileTime(&stime, &ftime);
         time_pointer = ftime.dwLowDateTime;
         #else

         gettimeofday(&time, NULL);

         #endif

         time_pointer = (unsigned) time.tv_usec / 1000
                      + (unsigned long long) time.tv_sec * 1000;
         _register[DAYCLOCK]   =  time_pointer & 0x00FFFFFF;
         _register[DAYCLOCK_U] = (time_pointer >> 24) & 0x00FFFFFF;

         /************************************************
		atomicity writing the two dayclock words
		is not necessary in this model
		because instructions don't happen
		while this does
         ************************************************/
      }
      #endif

      if (indication & LOCKSTEP) flag['s'-'a'] = 1;

      if (indication & BREAKPOINT)
      {
         if (apc == breakpoint)
         {
            flag['s'-'a'] = 1;
         }
      }

      if (flag['s'-'a'])
      {
         indication |= LOCKSTEP;
         statement();
         putchar('>');
         fflush(stdout);
      }

      if (indication & LOCKSTEP)
      {
         #ifdef METRIC
         accumulate_metric();
         #endif
 
         while (indication & LOCKSTEP) usleep(10000);

         #ifdef METRIC
         gettimeofday(&time2, NULL);
         delta_base = time2.tv_sec * 1000000 + time2.tv_usec;
         #endif
      }

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
            if (!icount) ii(YIELD, LP_TSLICE);
         }
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
   }
   return 0;
}

/*************************************************

        carries out sync or async tty commands

*************************************************/


static void action(char request[])
{
   word			 sample;
   word			*departure;

   int			 xx,
			 index,
                         base_tag,
                         base_index,
                         offset,
                         absolute;

   int			 symbol = request[0];

   char			 path[360];

   #ifdef METRIC
   double                average,
                         quantum;

   if (symbol == 'i')
   {
      average = total_metric;
      quantum = total_delta;
      average /= quantum;
      printf("instructions %lld usecs %lld approximate MIPS %f\n",
              total_metric, total_delta, average);

      putchar(':');
      fflush(stdout);
      return;
   }
   #endif

   if (flag['s'-'a'] == 0)
   {
      if ((symbol ^ 's') && (symbol ^ '.'))
      {
         printf("key s for single step\n");
         return;
      }
   }

   switch(symbol)
   {
      case 's':
         flag['s'-'a'] = 1;
         break;

      case 'g':
         xx = sscanf(request + 1, "%x:%x", &index, &offset);

         if (xx == 1) breakpoint = &b0p->w[index];
         if (xx == 2) breakpoint = &memory.p4k[index].w[offset];

         if (xx) indication |= BREAKPOINT;
         flag['s'-'a'] = 0;
         indication &= -1 ^ LOCKSTEP;
         putchar(':');
         fflush(stdout);
         break;

      case '+':
         index = sp;
         index += 23;
      case '0':
      case 'r':
         if (symbol ^ '+') index = iselect | 24;

         if      (symbol == '0') sscanf(request,     "%x", &index);
         else if (symbol == 'r') sscanf(request + 1, "%d", &index);

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
            absolute = (base_index << 12) + offset;

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

         xx = sscanf(request +1, "%x:%x", &absolute, &offset);

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
               xx = device_read(absolute, base_index, offset++);
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

         printf("\n7c                             ");

         for (xx = 124; xx < 192; xx++)
         {
            if (!(xx & 7)) printf("\n%2.2x:", xx);
            printf(" %6.6x", base[xx]);
         }

         putchar('\n');

         break;

      case 'l':
         xx = sscanf(&request[1], "%s", path);
         if (xx == 1) load_fs(path);
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
         runout = -1;
         indication &= -1 ^ LOCKSTEP;
         break;

      case '?':
         printf("Stepping %s\n", STEPPING);
         break;

      default:
         indication &= -1 ^ LOCKSTEP;
         break;
   }
}

static void load_fs(char *path)
{
   int		 xx,
		 banks,
		 index = 0;

   char		*loader = (char *) devices[1].s.dev24;

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

      devices[1].s.dev24 = (device24 *) loader;
      base[129] = 0x00E00000 | banks;

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
      printf("%d granules loaded\n", index);
   }
}

#ifdef METRIC
static void accumulate_metric()
{
   /*******************************************************
        this happens at leading edge of emulation sleep
        requested by I/O chilldown indication
   *******************************************************/

   struct timeval        time3;
   unsigned long long    trailing_edge;

   gettimeofday(&time3, NULL);
   trailing_edge = time3.tv_sec * 1000000 + time3.tv_usec;

   delta = trailing_edge -  delta_base;
   total_metric += metric;
   metric = 0;
   total_delta += delta;
}
#endif

