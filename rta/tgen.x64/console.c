#include <stdio.h>

#include "../engine.rta/emulate.h"
#include "../include.rta/argue.h"
#include "../rta.run/settings.h"

#ifdef  GCC
#include "../engine.rta/rw.h"
#else
extern word              memory_read(int ea);
extern int		 device_read(int device_index, int block, int offset, int designator);
#endif

#ifdef	METRIC
extern long long	 total_metric,
			 total_delta;
#endif

extern void load_fs(int device_id, char *path);
extern void assign_interface_relay(int device_id, char *text);

extern int		 iselect;
extern int		 indication;
extern word		*apc;
extern word		*apcz;
extern page		*b0p;
extern unsigned int	 b0_name;
extern unsigned int	 psr;
extern word		*breakpoint;
extern unsigned int	_register[];
extern unsigned int	 base[];
extern device		 devices[];
extern system_memory	 memory;

void print_register_row(int index)
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

void action(char request[])
{
   word			 sample;
   word			*departure;

   unsigned		 xx,
			 index,
			 device_sense,
                         device_index,
                         base_index,
                         offset,
                         guide;

   page			*pagep;

   word			*wordp,
			*limitp;

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
      printf("instructions %lld usecs %lld approximate MIPS %f\n",
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
         xx = sscanf(request + 1, "%x:%x", &index, &offset);

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
         else
         {
            indication &= -1 ^ BREAKPOINT;
            breakpoint = NULL;
         }

         if (breakpoint)
         {
            if (flag['v'-'a']) printf("[>%x,%lx]",
                                       indication, breakpoint - memory.p4k[0].w);
            if (flag['e'-'a']) printf("[@%p:%p]", memory.p4k[0].w, breakpoint);
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
            base_index &= 0x003FFFFF;
            pagep = memory.p4k + base_index;
            wordp = pagep->w + offset;
            limitp = memory.p4k[PAGES_IN_MEMORY].w;

            for (;;)
            {
               if (xx = offset >> 12)
               {
                  base_index += xx;
                  offset &= 4095;
                  index -= xx << 12;
               }

               printf("%6.6x:%6.6x :", base_index, offset);
               index = offset + 8;

               while (offset < index)
               {
                  if (wordp < limitp)
                  {
                  }
                  else break;

                  if (xx = offset >> 12)
                  {
                     base_index += xx;
                     offset &= 4095;
                     index -= xx << 12;
                  }

                  offset++;
                  sample = *wordp++;
                  printf(" %2.2x%2.2x%2.2x", sample.t1, sample.t2, sample.t3);
               }

               fgets(request, 48, stdin);
               if (request[0] == '.') break;

               if (wordp < limitp)
               {
               }
               else break;
            }
         }

         break;

      case 'd':

	/*****************************************************
		for viewing peripheral device arrays
	*****************************************************/

         device_index = 0;		/* the device or the offset */
         offset = 0;
         base_index = 0;

         sscanf(request + 1, "%x:%x:%x", &device_index, &base_index, &offset);

         if (device_index < 0) break;
         if (device_index > 63) break;

         device_sense = base[128 + device_index];
         guide = device_sense & 0x003FFFFF;
         device_sense &= 0x00C00000;

         if (device_sense == 0) break;

         if (device_sense ^ SYSMEM_FLAG)
         {   
             base_index <<= 6;
             guide <<= 6;
         }

         base_index |= 0x00400000;
         guide |= 0x00400000;

         for (;;)
         {
            if (xx = offset >> 18)
            {
               base_index += xx << 6;
               offset &= 262143;
               index -= xx << 18;
            }

            printf("%6.6x:%6.6x :", device_index | base_index, offset);
            index = offset + 8;

            while (offset < index)
            {
               if (xx = offset >> 18)
               {
                  base_index += xx << 6;
                  offset &= 262143;
                  index -= xx << 18;
               }

               if (device_index == 0)
               {
                  if ((offset >> 12) > (guide & 0x003FFFFF))
                  base_index = guide + 64;
               }

               if (base_index > guide) break;               

               xx = device_read(device_index, base_index, offset++, 0);

               printf(" %6.6x", xx);
            }

            if (base_index > guide) break;

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
         xx = sscanf(&request[1], "%d %s", &device_index, path);

         if (xx == 2)
         {
            if ((device_index > 2) && (device_index < 64))
            {
               device_sense = base[128 + device_index];
               device_sense &= 0x00C00000;

               if ((device_sense == SYSMEM_FLAG)
               ||  (device_sense == DATA16_FLAG))
               printf("device %d already assigned as %s\n",
                       device_index,
                      (device_sense == SYSMEM_FLAG) ?
                      "system memory" : "interface array");

               else load_fs(device_index, path);
            }

            else printf("only devices 3..63 are dynamically assignable\n");
         }
         else printf("load fs requires: l device fsimage_path\n");
         break;

      case '#':
         xx = sscanf(&request[1], "%d %s", &device_index, path);

         if (xx == 2)
         {
            if ((device_index > 2) && (device_index < 64))
            {
               device_sense = base[128 + device_index];
               device_sense &= 0x00C00000;

               if (device_sense) printf("device %d already assigned type %x\n",
                                         device_index,
                                         device_sense);

               else assign_interface_relay(device_index, path);
            }
            else printf("only devices 3..63 are dynamically assignable\n");
         }
         else printf("attach interface array requires: # device key\n");

         break;

      case 'h':
         printf("LF\texecute one instruction and display new state\n");
         printf("+\tdisplay more internal stack registers\n");
         printf("0n\tdisplay registers starting at hex address\n");
         printf("r n\tdisplay registers starting at decimal address\n");
         printf("\n\n\t\tenter device- block- iframe- and offset-addresses in hex\n\n");
         printf("b\tdisplay relocation / configuration ports\n\n");
         printf("m [[page:]offset]\tdisplay system memory\n");
         printf("\tdefault page[s] in current address space\n\n");
         printf("d [[device:][block:]offset]\tdisplay peripheral memory array[s]\n");
         printf("g [[frame:]breakpoint]\trun [to breakpoint]\n");
         printf("\tdefault breakpoint frame is current halted instruction frame\n");
         printf("\tto remove breakpoint g0:0\n\n");

         printf("\n\n\t\tcommands l and # enter device address in decimal\n\n");

         printf("l device path\t\tload file system image\n");
         printf("# device hexkey\t\tassign network interface relay array\n");

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

