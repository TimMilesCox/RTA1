#include <stdio.h>

#ifdef	LINUX
#include <malloc.h>
#else
#include <sys/malloc.h>
#endif

#include "../engine.rta/emulate.h"
#include "../include.rta/argue.h"
#include "../rta.run/settings.h"

#ifdef  GCC
#include "../engine.rta/rw.h"
#else

#ifdef  LINUX
#include "../tgen.x64/_mnames.h"
#endif

extern int		 device_read(int device_index, int block, int offset, int designator);
#endif

#ifdef	METRIC
extern long long	 total_metric,
			 total_delta;
#endif

extern void load_fs(int device_id, char *path);
extern void assign_interface_relay(int device_id, char *text);
extern void assign_array(int device_id, char *text);

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

int			 register_pointer = 128;

int print_register_row(int index)
{
   int           xx = 8;

   if (flag['e'-'a']) printf("[%p]", _register);
   printf("%2.2x:", index);
   while ((xx--) && (index < REGISTERS))
   {
      if (index < 256) printf(" %6.6x", _register[index++]);
      else             printf(" %8.8x", _register[index++]);

      if ((index & 127) == 0) break;
   }

   return index;
}

int print7_registers(int index)
{
   int		 xx = 7;

   while ((xx--) && (index < REGISTERS))
   {
      printf(" %6.6x", _register[index++]);
      if ((index & 127) == 0) break;
   }

   return index;
}

void action(char request[])
{
   word			 sample;
   word			*departure;

   unsigned		 xx,
			 index,
			 datum,
			 device_sense,
                         device_index,
                         base_index,
			 block,
			 pointer,
			 base_tag,
                         limitb,
                         guide;

   unsigned 		 offset;

   page			*pagep;

   word			*wordp,
			*limitp;

   int			 symbol = request[0];
   int			 prompt = 0;
   int			 update;

   char			 path[360];
   char			*text;

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

   #ifdef _XINT
   if (symbol == 'j')
   {
      /************************************************************
	"index" is an 8-bit mask to generate 8 interrupts at once
      ************************************************************/

      sscanf(request + 1, "%x:%x", &index, &datum);

      xx = 8;

      while (xx--)
      {
         if ((index >> xx) & 1) base[80 + xx] = datum;
      }

      indication |= (index << 8) & EXTERNAL_INTERRUPT;

      printf("[%x:%x:%x]\n", index, datum, indication);
      return;
   }

   if (symbol == 'k')
   {
      indication |= INCREMENTER_CARRY;
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
         if (xx == 2)
         {
            if (index | offset) breakpoint = &memory.p4k[index].w[offset];
            else breakpoint = NULL;
         }

         flag['s'-'a'] = 0;
         indication &= -1 ^ LOCKSTEP;

         if (breakpoint)
         {
            indication |= BREAKPOINT;

            #ifdef __X64
            #define FORMAT1 "[>%x,%lx]"
            #else
            #define FORMAT1 "[>%x,%x]"
            #endif

            if (flag['v'-'a']) printf(FORMAT1,
                                      indication, breakpoint - memory.p4k[0].w);
            if (flag['f'-'a']) printf("[@%p:%p]", memory.p4k[0].w, breakpoint);
         }
         else indication &= -1 ^ BREAKPOINT;

         prompt = 0;
         putchar(':');
         fflush(stdout);
         break;

      case '+':
         index = register_pointer;
         register_pointer = sp;
      case '0':
      case 'r':
         if (symbol ^ '+') index = iselect | 24;

         if      (symbol == '0') sscanf(request,     "%x", &index);
         else if (symbol == 'r') sscanf(request + 1, "%d", &index);

         while (index < REGISTERS)
         {
            index = print_register_row(index);
            // index += 8;
            fgets(request, 48, stdin);
            if (request[0] == '.') break;
         }

         break;

      case 'u':
      case 'U':

        datum = symbol;
        text = request + 1;
        index = -1;

        while ((symbol = *text) == ' ') text++;
        if (symbol == '0') sscanf(text, "%x", &index);
	else               sscanf(text, "%d", &index);

        while ((symbol = *text++) ^ ':') ;
        while ((symbol = *text) == ' ') text++;

        if (symbol == '0') prompt = sscanf(text, "%x", &update);
        else               prompt = sscanf(text, "%d", &update);

        if ((prompt < 1) || (index < 0) || (index > 282))
             printf("u 0..282:value in decimal / leading zero hex either field\n");
        else
        {
           /********************************************************************
		unless Big-U override truncate simulated registers to 24 bits
		but leave sub-architectural registers 256..287 platform size
           ********************************************************************/

           if ((index < 256) && (datum ^ 'U')) update &= 0x00FFFFFF;
           _register[index] = update;
        }

        break;

      case 'm':

         xx = sscanf(&request[1], "%x:%x", &base_index, &offset);

         if (xx < 2)
         {
            offset = base_index & 0x00FFFFFF;

            for (;;)
            {
               index = offset + 8;
               printf("%6.6x :", offset);
               guide = 262143;

               if (base_tag = (offset >> 18)) pointer = offset & 0x0003FFFF;
               else
               {
                  base_tag = offset >> 12;
                  pointer = offset & 0x00000FFF;
                  guide = 4095;
               }

               if ((base_tag) && (base_tag < 8))
               {
                  if ((0x8000 >> base_tag) & psr) base_tag |= 64;
               }

               base_index = base[base_tag];
               device_index = 0;

               if (base_index & 0x00400000) device_index = base_index & 63;

               if (device_index == 0)
               {
                  pagep = memory.p4k + (base_index & 0x003FFFFF);
                  wordp = pagep->w + pointer;
               }

               printf(" [%6.6x:%6.6x] :", base_index, pointer);
                  
               while (offset < index)
               {
                  if (device_index)
                  {
                     if (base_index & 0x00800000) printf(" ******");
                     else printf(" %6.6x",
                                 device_read(device_index, base_index, pointer, 0));
                  }
                  else
                  {
                     sample = *wordp++;
                     printf(" %2.2x%2.2x%2.2x", sample.t1, sample.t2, sample.t3);
                  }

                  pointer++;
                  offset++;

                  if (pointer > guide) break;
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

         text = request + 1;
         sscanf(text, "%x", &device_index);
         while ((datum = *text++) && (datum ^ ':')) ;

         if (datum == ':')
         {
            sscanf(text, "%x", &base_index);
            while ((datum = *text++) && (datum ^ ':')) ;
            if (datum == ':') sscanf(text, "%x", &offset);
         }

         if (device_index < 0) break;
         if (device_index > 63) break;

         device_sense = base[128 + device_index];
         limitb = device_sense & 0x003FFFFF;
         device_sense &= 0x00C00000;

         if (device_sense == 0) break;

         base_index <<= 6;

         guide = 262143;

         if (device_sense == SYSMEM_FLAG)
         {
            guide = limitb << 12;
            guide |= 4095;
            guide &= 262143;
         }
         else limitb <<= 6;

         base_index &= 0x003FFFC0;
         limitb &= 0x003FFFC0;
         base_index |= 0x00400000;
         base_index |= device_index;
         limitb |= 0x00400000;
         limitb |= device_index;

         for (;;)
         {
            block = base_index + ((offset >> 18) << 6);
            pointer = offset & 0x0003FFFF;

            printf("%6.6x:%6.6x :", block, pointer);
            index = offset + 8;
            if (block > limitb) break;

            while (offset < index)
            {
               if (pointer > guide) break;               

               xx = device_read(device_index, block, pointer, 0);
               printf(" %6.6x", xx);
               offset++;
               pointer++;
            }


            fgets(request, 48, stdin);
            if (request[0] == '.') break;
         }

         break;

      case 'b':
         #if 1
         printf("\n   ");

         for (xx = 0; xx < 64; xx++)
         {
            if (!(xx & 7)) printf("\n%2.2x:", xx);
            printf(" %6.6x", base[xx]);
         }

         printf("\n   ");
         
         for (      ; xx < 128; xx++)
         {
            if (!(xx & 7)) printf("\n%2.2x:", xx);
            printf(" %6.6x", base[xx]);
         }

         printf("\n   ");
         
         for (      ; xx < 192; xx++)
         {
            if (!(xx & 7)) printf("\n%2.2x:", xx);
            printf(" %6.6x", base[xx]);
         }

         #else
         for (xx = 0; xx < 76; xx++)
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
         #endif

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

      case '/':
         xx = sscanf(request, "/%d/%s", &device_index, path);

         if      (xx < 2)            printf("requires /decimal device number/device info\n");
         else if (device_index < 3)  printf("devices 0..2 are fixed\n");
         else if (device_index > 63) printf("devices 3..63 are dynamically assignable\n");
         else
         {
            if      (path[0] == '-')
            {
               device_sense = base[128 + device_index];
               device_sense &= 0x00C00000;

               switch (device_sense)
               {
                  case SYSMEM_FLAG:
                  case FSYS24_FLAG:
                     free(devices[device_index].pages);
                     devices[device_index].pages = NULL;
                     printf("device %d %x unmounted\n", device_index, base[device_index + 128]);
                     base[device_index + 128] = 0;
                     break;

                  default:
                     printf("only memory and filestore devices "
                            "may be unmounted during operation\n");
               }
            }
            else
            {
               device_sense = base[128 + device_index];
               device_sense &= 0x00C00000;

               if (device_sense) printf("device %d already assigned type %x\n",
                                         device_index,
                                         device_sense);

               else if (path[0] == '+') assign_array(device_index, path);
               else if (path[0] == '#') assign_interface_relay(device_index, path);
               else                     load_fs(device_index, path);
            }
         }

         break;

      case '\\':
         xx = sscanf(request + 1, "%s", path);

         if (xx)
         {
            text = path;

            while (datum = *text++)
            {
               if ((datum > ('a' - 1)) && (datum < ('z' + 1)))  flag[datum - 'a'] ^= 1;
               if ((datum > ('A' - 1)) && (datum < ('Z' + 1))) uflag[datum - 'A'] ^= 1;
            }
         }
         else printf("option letters required");
         break;

      case 'h':
         printf("LF\texecute one instruction and display new state\n");
         printf("+\tdisplay more internal stack registers\n");
         printf("0n\tdisplay registers starting at hex address\n");
         printf("r n\tdisplay registers starting at decimal address\n");
         printf("u register:value\tupdate a register 0..282:value\n"
                "\t\t\tdecimal / leading zero hex either field\n"
		"\t\t\tuppercase U forces oversize write in registers 0..255\n");

         printf("\n\n\t\tenter device- block- iframe- and offset-addresses in hex\n\n");
         printf("b\tdisplay relocation / configuration ports\n\n");
         printf("m [[page:]offset]\tdisplay system memory\n");
         printf("\tdefault page[s] in current address space\n\n");
         printf("d [[device:][block:]offset]\tdisplay peripheral memory array[s]\n");
         printf("g [[frame:]breakpoint]\trun [to breakpoint]\n");
         printf("\tdefault breakpoint frame is current halted instruction frame\n");
         printf("\tto remove breakpoint g0:0\n\n");

         printf("\n\n\t\tcommands l # + enter device address in decimal\n\n");

         printf("/device/ device path\t\tload file system image\n");
         printf("/device/# device hexkey\t\tassign network interface relay array\n");
         printf("/device/+ device banks\t\tassign memory array\n");
         printf("/device/- device banks\t\tunmount memory array or filestore\n\n");

         printf("\\\t\t\tinvert run option\n");
         printf("..\t\t\texit emulator\n");
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

