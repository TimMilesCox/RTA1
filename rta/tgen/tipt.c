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

#define	TIME_UPDATE	1
#define	LOCKSTEP	2
#define	BREAKPOINT	4
#define	CHILLDOWN	8

#define	chilldown	base[103]

#define ROM_PAGE	&memory.p4k[0].w[0]

static word		*breakpoint;

int			 indication;

#if 0
int			 trace, trace1, trace2, trace3;
#endif

#ifndef	X86_MSW
static struct pollfd	 attention = { 0, POLLIN } ;
#endif

static long long	 start_time;
static long long	 u;
#ifndef	X86_MSW
static struct timeval	 time1;
#endif

#if	0

int			 iselect = 128;
word			*apc = ROM_PAGE;
extern page		*b0p;
extern unsigned int	 b0_name;
extern unsigned int	 psr;
extern unsigned int	_register[];
unsigned int		*register_set = _register+128;
extern unsigned int	 base[];
extern device		 devices[];

#else

system_memory            memory;
int              	 iselect = 128;
word		    	*apc = ROM_PAGE;
page            	*b0p  = memory.p4k;
unsigned int		 b0_name;
unsigned int    	 psr = 0x00800000;
unsigned int    	 _register[256+24];
 
unsigned int		*register_set = _register+128;
// unsigned int     	 base[192];
// system_memory    	 memory;
// device           	 devices[64];
#include "ioports.c"
#include "devices.c"
device			*pdevice = devices;

#endif

static word		 memory_read(int ea);
extern void		 bus_read();
//	extern int               bus_read(int device, int pointer);
extern void              netbank();
extern void		 execute(int instruction);

static void		*emulate();
static void		 statement();
static void		 action(char request[]);
static void		 load_fs(char *path);
static int		 msw2i(msw w);
static void		 print_register_row(int index);

int main(int argc, char *argv[])
{
   int			 _x;
   char			 text[72];
   char			*_p;

   #ifdef X86_MSW
   start_time = GetTickCount64();
   #else
   pthread_attr_t        asyncb;
   pthread_t             asyncid;

   gettimeofday(&time1, NULL);
   start_time = time1.tv_sec * 1000 + time1.tv_usec / 1000;
   #endif

   int                   f, count, image_size = 0;

   word                 *load_address = memory.array;
   word                  data_word = { 0, 0, 0, 0 } ;

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
      base[124] = image_size >> 12;             /*      ROM boundary port       */
      base[128] = PAGES_IN_MEMORY - 1;          /*      system memory size port */

      close(f);
   }
   else
   {
      printf("rom image file required\n");
      return 0;
   }

   netbank();

   if (arguments > 1) load_fs(argument[1]);

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

   for (;;)
   {
      #ifdef X86_MSW
      _x = _kbhit(1);
      #else
      _x = poll(&attention, 1, 1000);

      if (_x < 0)
      {
         printf("problem with input request %d. exit emulator\n", errno);
         break;
      }
      else
      #endif

      if (_x)
      {
         flag['s'-'a'] = 1;

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
      gettimeofday(&time1, NULL);
      u = time1.tv_sec * 1000 + time1.tv_usec / 1000;
      #endif

      indication |= TIME_UPDATE;
   }

   return 0;
}

void *emulate()	/* thread start */
{
   unsigned int		 transitionl,
			 transitionu;

   printf("emulation start\n");

   for (;;)
   {
      if (flag['s'-'a']) indication |= LOCKSTEP;
      if (flag['e'-'a']) printf("[%x %p %x %p %x]\n",
                                 indication, register_set, psr, apc,
                                 apc - memory.array);
      #ifdef X86_MSW
      __masm {
        engage:
		push	eax
		push	ebx
		push	ecx
		push	edx
		push	ebp
		push	esi
		push	edi

                mov     ebp, dword ptr [register_set]
                mov     edx, dword ptr [apc]

        next:   mov     eax, dword ptr [edx]
                add     edx, 4
                bswap   eax
                call    execute
                test    word ptr [indication], TIME_UPDATE|LOCKSTEP|BREAKPOINT
                jz      next

		mov	dword ptr [apc], edx
		mov	dword ptr [register_set], ebp

		pop	edi
		pop	esi
		pop	ebp
		pop	edx
		pop	ecx
		pop	ebx
		pop	eax
      }
      #else
      __asm__
      { 
	engage:
;		OSX PIC
;		reference no extern variables in asm block
;		compiler injects instructions which use an index register
;		it was ebx but could change any compiler release
;		if things go wrong gcc -S tipt.c

		push	ebp
		mov	ebp, esi
		push	ebp

;		compiler does not allow pop esi
;		so don't push it

		push	edi
		push	edx
		push	ecx
		push	ebx
		push	eax

;		compiler does not allow pop esi
;		so don't push it

		mov	ebp, register_set
		mov	edx, apc
	next:	mov	eax, [edx]
		add	edx, 4
		bswap	eax
		call	execute
		test	indication, CHILLDOWN|TIME_UPDATE|LOCKSTEP|BREAKPOINT
		jz	next
		mov	register_set, ebp
		mov	apc, edx

		pop	eax
		pop	ebx
		pop	ecx
		pop	edx
		pop	edi

;		compiler does not allow pop esi
		pop	ebp
		mov	esi, ebp
		pop	ebp
      }
      #endif

      #if 0
      if (flag['e'-'a'])
      printf("[0:%8.8x 1:%8.8x 2:%8.8x 3:%8.8x]\n",
              trace, trace1, trace2, trace3);
      #endif

      if (indication & CHILLDOWN)
      {
         indication &= -1 ^ CHILLDOWN;
         usleep(base[103]);
      }

      if (indication & TIME_UPDATE)
      {
         transitionl = u & 0x00FFFFFF;
         transitionu = (u >> 24) & 0x00FFFFFF;

         _register[DAYCLOCK] = u & 0x00FFFFFF;
         _register[DAYCLOCK_U] = (u >> 24) & 0x00FFFFFF;
         indication &= -1 ^ TIME_UPDATE;
         if (indication == 0) continue;
      }

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

      #if 1
      putchar('>');
      fflush(stdout);
      #endif

      while (indication & LOCKSTEP) usleep(10000);
   } 
}

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
   int			 prompt = 0;

   char			 path[360];


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
         xx = sscanf(request + 1, "%x:%x", &index, &offset);

         if (xx == 1) breakpoint = &b0p->w[index];
         if (xx == 2) breakpoint = &memory.p4k[index].w[offset];

         flag['s'-'a'] = 0;
         indication &= -1 ^ LOCKSTEP;
         if (xx) indication |= BREAKPOINT;

         if (breakpoint)
         {
            if (flag['v'-'a']) printf("[>%x,%x]",
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

         if      (symbol == '0') sscanf(request,     "%x", &index);
         else if (symbol == 'r') sscanf(request + 1, "%d", &index);

         while (index < 280)
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

      case 'z':
         sscanf(request + 1, "%lld", &start_time);
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

static void statement()
{
   int                   index = iselect;
   int                   index2;

   instruction_display(apc - 1, 1, flag['l'-'a']);
   if (flag['e'-'a']) printf("[RP %p]", register_set);
   printf("%6.6x %8.8x\n", psr, (word *) apc - memory.array);
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
   while ((xx--) && (index < 280)) printf(" %6.6x", _register[index++]);
}


/********************************************
	readout target current address space
********************************************/

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

