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



#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <sys/time.h>

#include "../engine.rta/emulate.h"
#include "../engine.rta/ii.h"
#include "../engine.fs/device24.h"

#define	ASYNC

#ifdef	ASYNC
#include <pthread.h>
#endif

#include <errno.h>

#include "settings.h"

#define	ARGUMENTS	3

extern int		 iselect;
extern word		*apc;
extern page		*b0p;
extern unsigned int	 psr;
extern unsigned int	_register[256];
extern unsigned int	 base[];
extern system_memory	 memory;
extern device		 devices[];

static word		*breakpoint;
char			 flag[26];
unsigned short		 arguments;
char			*argument[ARGUMENTS];

static int		 mscan;

extern void		 execute(word instruction);
extern word		 memory_read(int ea);
extern void		 netbank();

static void		 statement();
static void		 action(char *request);
static void		 load_fs(char *path);

#ifdef SIGALERT
static void signet(int sig);

static struct sigaction  si = {      (* signet),
                                   1 << SIGUSR1,
                                     SA_SIGINFO } ;
static void signet(int sig)
{
   base[NET_ATTENTION_COUNT]++;
}
#endif


#ifdef DAYCLOCK
static int		 start_time;
#endif


#ifdef ASYNC

static void *async()
{
   char			 request[360];

   for (;;)
   {
      if (flag['s'-'a'])
      {
         usleep(10000);
         continue;
      }

      putchar(':');
      fgets(request, 62, stdin);

      action(request);
   }
}

#endif

static int msw2i(msw w)
{
   printf("[%2.2x%2.2x%2.2x]\n", w.t1, w.t2, w.t3);
   return (w.t1 << 16) | (w.t2 << 8) | w.t3;
}

static void print_register_row(int index)
{
   printf("%2.2x: %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x\n",
           index,
           _register[index],   _register[index+1],
           _register[index+2], _register[index+3],
           _register[index+4], _register[index+5],
           _register[index+6], _register[index+7]);
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
      #if 1
      print_register_row(index);
      #else
      printf("%2.2x: %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x\n",
              index,
              _register[index],   _register[index+1],
              _register[index+2], _register[index+3],
              _register[index+4], _register[index+5],
              _register[index+6], _register[index+7]);
      #endif
      index += 8;
   }

   index = sp;
   printf("[%6.6x]->", index);

   index2 = 7;

   while (index2--)
   {
      printf(" %6.6x", _register[index++]);
   }

   printf("\n   ");
   index2 = 8;

   while (index2--)
   {
      printf(" %6.6x", _register[index++]);
   }
   putchar('\n');
}

int main(int argc, char *argv[])
{
   #ifdef DAYCLOCK
   struct timeval	 time;
   int			 instructions = INTERVAL;
   int			 imask;
   #endif

   int			 time_pointer;

   char			 command[48];

   int			 index = 1;
   int			 offset;
   int			 symbol;
   char			*file;

   int			 f, count, image_size = 0;

   word			*load_address = memory.array;
   word			 data_word;

   #ifdef ASYNC

   pthread_attr_t	 asyncb;
   pthread_t		 asyncid;

   #endif

   #ifdef DAYCLOCK

   gettimeofday(&time, NULL);
   start_time = time.tv_sec * 1000 + time.tv_usec / 1000;

   #endif


   while (index < argc)
   {
      file = argv[index];

      if (*file == '-')
      {
         file++;

         while (symbol = *file++)
         {
            if ((symbol > 0x40) && (symbol < 0x5B)) symbol |= 32;
            if ((symbol > 0x60) && (symbol < 0x7B)) flag[symbol - 'a'] = 1;
         }
      }
      else
      {
         if (arguments < ARGUMENTS) argument[arguments++] = file;
      }

      index++;
   }

   if (arguments)
   {
      f = open(argument[0], O_RDONLY, 0444);

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

   #ifdef SIGALERT

   /**********************************************************

	if you turn SIGALERT on
	you must also turn it on in ../netifx/netbank.c

        and in the load image which the emulated machine
	executes in ../t77.4/icapsule.msm

	and in the interface simulator process ../netifx/netifx.c

   **********************************************************/

   x = sigaction(SIGUSR1, &si, NULL);
   printf("[RSIG %d %p %p %x %x]\n",
            x, signet, si.sa_handler, si.sa_mask, si.sa_flags);
   #endif

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
      if (mscan)
      {
         usleep(1000000);
         continue;
      }

      execute(*apc++);

      #ifdef DAYCLOCK

      instructions--;
      if (!instructions)
      {
         instructions = INTERVAL;
         gettimeofday(&time, NULL);
         time_pointer = time.tv_sec * 1000 + time.tv_usec / 1000;
         time_pointer -= start_time;
         _register[DAYCLOCK_U] = time_pointer >> 24;
         _register[DAYCLOCK]   = time_pointer & 0x00FFFFFF;
      }

      #endif


      if (apc == breakpoint)
      {
         flag['s'-'a'] = 1;
      }
      else
      {
         if (flag['s'-'a'] == 0)
         {
            if (psr & 0x00800000)
            {
            }
            else
            {
               time_pointer = _register[REALTIME_CLOCK];
               if (time_pointer & 0x00800000)
               {
                  time_pointer++;
                  time_pointer &= 0xFFFFFF;
                  _register[REALTIME_CLOCK] = time_pointer;
                  if (!time_pointer) ii(YIELD);
               }
            }

            #ifdef TIMER
            instructions--;
            if (!instructions)
            {
               instructions = INTERVAL;
               gettimeofday(&time, NULL);
               time_pointer = time.tv_sec * 1000000 + time.tv_usec;
               time_pointer /= TIME_GRANULE;
               time_pointer &= 0x00FFFFFF;
               imask = (psr >> 16) & 127;

               if (time_pointer |= base[TIMER_IO_PORT]) ii(TIMER);
               {
                  base[TIMER_IO_PORT] = time_pointer;
                  if (imask < TIMER_IMASK) ii(TIMER);
               }
            }
            #endif

            continue;
         }
      }

      statement();
      putchar('>');
      fflush(stdout);

      fgets(command, 48, stdin);

      switch (command[0]) 
      {
         case 'g':
            symbol = sscanf(&command[1], "%x:%x", &index, &offset);

            if (symbol)
            {
               if (symbol == 1) breakpoint = &b0p->w[index];
               if (symbol == 2) breakpoint = &memory.p4k[index].w[offset];
            }

            flag['s'-'a'] = 0;
            break;

         case '.':
            return 0;

         default:
            action(command);
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

   int			 x,
			 index,
                         base_index,
                         offset,
                         absolute;

   char			 path[360];


   switch(request[0])
   {
      case 's':
         flag['s'-'a'] = 1;
         break;

      case 'r':

         index = iselect | 24;
         sscanf(&request[1], "%d", &index);

         while (index < 256)
         {
            #if 1
            print_register_row(index);
            #else
            printf("%2.2x: %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x %6.6x\n",
                    index,
                    _register[index],   _register[index+1],
                    _register[index+2], _register[index+3],
                    _register[index+4], _register[index+5],
                    _register[index+6], _register[index+7]);
            #endif

            index += 8;
            fgets(request, 48, stdin);
            if (request[0] == '.') break;
         }

         break;

      case 'm':

         x = sscanf(&request[1], "%x:%x", &base_index, &offset);
         mscan = 1;

         if (x < 2)
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

         mscan = 0;
         break;

      case 'b':
         for (x = 0; x < 72; x++)
         {
            if (!(x & 7)) printf("\n%2.2x:", x);
            printf(" %6.6x", base[x]);
         }

         printf("\n7c                             ");

         for (x = 124; x < 192; x++)
         {
            if (!(x & 7)) printf("\n%2.2x:", x);
            printf(" %6.6x", base[x]);
         }

         putchar('\n');

         break;

      case 'z':
         sscanf(request + 1, "%d", &start_time);
         break;

      case 'l':
         x = sscanf(&request[1], "%s", path);

         if (x == 1) load_fs(path);

         break;
   }
}

static void load_fs(char *path)
{
   int		 x,
		 banks,
		 index = 0;

   char		*loader;
   int		 f = open(path, O_RDONLY, 0444);

   vpage	 page;

   if (f < 0) printf("file system image %s error %d\n", path, errno);
   else
   {
      printf("loading file system image %s\n", path);
      x = read(f, (void *) &page, sizeof(page));

      /**************************************************
	more exactly volume root directory initial page
      **************************************************/

      if (x ^ sizeof(page))
      {
         printf("volume header read unsuccessful\n");
         return;
      }

      x = 3 * (page.label.ex.rfw.t3 - VOLUME1_WORDS);
      banks = msw2i(page.label.ex.granules);

      printf("%.*s %d storage banks\n", x, &page.label.name[0].t1, banks);

      #if 0
      loader = malloc(banks * sizeof(fsbank));
      #else
      loader = (char *) calloc(banks, sizeof(fsbank));
      #endif

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
         x = read(f, loader, 192);
         if (x < 0) break;
         if (!x) break;
         index++;
         loader += 192;
      }

      close(f);
      printf("%d granules loaded\n", index);
   }
}

