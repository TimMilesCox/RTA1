/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file fds24.c is free software.

    fds24.c is a utility program for examining instruction
    code or data files and file system images which have been
    generated for the freeware processor architecture

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

    You can redistribute and/or modify RTA1 and its materials
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

#ifdef DOS
#include <fcntl.h>
#include <sys/types.h>
#define off_t	__int64
#define lseek	_lseeki64
#else
#include <fcntl.h>
#include <unistd.h>
#endif

#include <string.h>

#include "../include.rta/argue.h"

#define	FRAME	12
#define	FRAMES	4

typedef struct { unsigned char	b[FRAME]; } frame;
                 
static unsigned char		 octets[8];
static frame			 data[FRAMES];

static int fieldcmp(char *table, int offset, char *string, int bits);

static char *rline(int x)
{
   static char data[384];

   int y = 0;

   while (y < 383)
   {
      if (read(x, &data[y], 1) != 1) break;
      if (data[y++] == 10) break;
   }
   if (y == 0) return NULL;
   data[y] = 0;
   return data;
}

int main(int argc, char *argv[])
{
   int			 x, y, print, length, symbol, bytes;
   int			 hits;

   unsigned char	*p, *q;

   int			 f = -1,
			 s = -1;

   off_t		 cursor = 0,
			 position;

   off_t		 forward,
			 back;

   unsigned char	 pointers[6];
   unsigned char	 string[48];


   argue(argc, argv);
   
   if (flag['s'-'a'])
   {
      if (arguments > 2)
      {
         #ifdef DOS
         s = open(argument[2], O_RDONLY | O_BINARY);
         #else
         s = open(argument[2], O_RDONLY);
         #endif
      }

      if (s < 0)
      {
         printf("no special strings file found at argument_3 -s\n");
         if (arguments > 2) arguments = 2;

         /*************************************************************
		prevent any failed-open pathname
		from getting used as a search string
         *************************************************************/
      }
   }
   else
   {
      for (x = 2; x < arguments; x++) octets[x-2] = strlen(argument[x]);
   }

   if (arguments > 1)
   {
      p = argument[1];
      symbol = *p;

      if ((symbol > '0' - 1) && (symbol < '9' + 1))
      {
         #ifdef OFF_T32
         if      (symbol == '0') sscanf(p, "%lx", &cursor);
         else                    sscanf(p, "%ld", &cursor);
         #else
         if      (symbol == '0') sscanf(p, "%llx", &cursor);
         else                    sscanf(p, "%lld", &cursor);
         #endif
         cursor *= 3;
      }
      else printf("argument 2 not an address. Initial file position zero\n");
   }

   if (arguments)
   {
      #ifdef DOS
      f = open(argument[0], O_RDONLY | O_BINARY);
      #else
      f = open(argument[0], O_RDONLY);
      #endif
   }
   else printf("\n\tfds24 input_file [start_position] [strings_file -s|octet_string_1] [...octet_string_n] [-Z]\n\n"
               "\t-Z displays the file without prompt\n"
               "\tstart_position is the first data word for display or search\n"
               "\tstrings_file -s contains bit-string search masks assembled by masmx\n"
               "\toctet_string_x... are 8-bit ASCII search strings\n\n");

   if (f < 0) printf("input file at argument 1 not opened\n");
   else
   {
      if (cursor)
      {
         /*******************************************************************
		2nd argument is a read start 24-bit word position
         *******************************************************************/

         position = lseek(f, cursor, SEEK_SET);
         if (flag['v'-'a']) printf("status %d file position %llx\n", x, cursor / 3);
      }

      for(;;)
      {
         /*******************************************************************
		read the file in 4-word * 24-bit blocks
		cache 4 blocks for straddling search strings
         *******************************************************************/

         data[0] = data[1];
         #if (FRAMES==4)
         data[1] = data[2];
         data[2] = data[3];
         #endif
         
         bytes = read(f, data[FRAMES-1].b, FRAME);
         if (bytes == 0) break;


         /*******************************************************************
		print = FRAMES -1 is the default setting for serial view
		the newest-read 4-word block will be displayed

		a search-argument hit causes containing blocks to display
         *******************************************************************/

         print = FRAMES - 1;

         if (arguments > 2)
         {
            /****************************************************************

			search strings either on the command line
			or in data file 3rd argument

			presence of search strings causes search
			instead of serial view

			only the 4-word blocks containing each search hit
			are displayed

            ****************************************************************/

            if (s < 0)
            {
               /************************************************************

			there is no search strings file
			3rd..zth argument are ASCII octet search strings

               ************************************************************/


               for (x = 2; x < arguments; x++)
               {
                  length = octets[x - 2];
                  if (length > FRAME*FRAMES-FRAME+1) length = FRAME*FRAMES-FRAME+1;

                  y = FRAME;
                  p = data[FRAMES-1].b - length + 1;

                  q = argument[x];
               
                  while (y)
                  {
                     if (memcmp(p, q, length) == 0) break;
                     p++;
                     y--;
                  }
                  if (y) break;
               }
 
               if (x < arguments)
               {
                  print = (p - data[0].b) / FRAME;
               }
               else
               {
                  cursor += FRAME;
                  if (bytes < FRAME) break;
                  continue;
               }
            }
            else
            {
               /**********************************************************
			there are search strings on the data file
			handle int s

			they are assembled by masmx and may be
			not octet byte and / or not ASCII-based data code
               **********************************************************/

               lseek(s, (off_t) 0, SEEK_SET);
               hits = 0;

               for (;;)
               {
                  x = read(s, pointers, 6);
                  if (x < 6) break;

                  length = (pointers[3] << 16) | (pointers[4] << 8) | pointers[5];
                  if (!length) break;

                  if (length > FRAME * 8 * (FRAMES - 1) + 1)
                  length     = FRAME * 8 * (FRAMES - 1) + 1;

                  y = (pointers[0] << 16) | (pointers[1] << 8) | pointers[2];
                  forward = y * 3;

                  back = lseek(s, (off_t) 0, SEEK_CUR);
                  lseek(s, forward, SEEK_SET);

                  y = 3 * ((length + 23) / 24);
                  x = read(s, string, y);
                  if (x < y) break;

                  lseek(s, back, SEEK_SET);

                  y = 96;

                  while (y--)
                  {
                     x = fieldcmp(data[0].b, 16 * 24 - length - y, string, length);
                     if (x == 0) break;
                  }              

                  if (x == 0)
                  {
                     if (hits == 0) print = (16 * 24 - length - y) / (24 * 4);
                     hits++;

                     if (flag['v'-'a'])

                     #ifdef OFF_T32
                     printf("\t\t\tmatch @%8.8lx:%x %d decimal bits\n",
                     #else
                     printf("\t\t\tmatch @%12.12llx:%x %d decimal bits\n",
                     #endif
                           (cursor - FRAME * (FRAMES - 1)) / 3 + (FRAMES * FRAME * 8 - length - y) / 24,
                           (16 * 24 - length - y) % 24,
                           length);
                  }
               }

               if (hits == 0)
               {
                  cursor += FRAME;
                  if (bytes < FRAME) break;
                  continue;
               }
            }
         }
         
         /****************************************************************

		one 4-word block per line
		on search hit, display the containing number of blocks
		on serial view display one block

         ***************************************************************/


         while (print < FRAMES)
         {
            if (flag['p'-'a']) printf("%s ", argument[0]);
         
            #ifdef OFF_T32
             printf("%8.8lx:", (cursor - FRAME * (FRAMES - 1 - print))/3);
            #else
            printf("%12.12llx:", (cursor - FRAME * (FRAMES - 1 - print))/3);
            #endif

            for (x = 0; x < FRAME; x++)
            {
               symbol = data[print].b[x];
               if (!(x % 3)) printf(" ");
               if ((print < FRAMES - 1) || (x < bytes)) printf("%2.2x", data[print].b[x]);
               else       printf("  ");
            }
            printf("  \"");
            for (x = 0; x < FRAME; x++)
            {
               if ((print == FRAMES - 1) && (x == bytes)) break;
               symbol = data[print].b[x];
               if ((symbol > 32) && (symbol < 127)) printf("%c", symbol);
               else                                 printf(" ");
            }

            putchar('\"');
            if (print < FRAMES-1) putchar('\n');
            print++;
         }

         cursor += FRAME;

         if (bytes < FRAME) break;

         if (uflag['Z'-'A'])
         {
            printf("\n");
            continue;
         }
         else
         {
             fflush(stdout);
         }

         p = rline(0);
         if (!p) break;
         symbol = *p;

         if (symbol == '.') break;

         if ((symbol > '0' - 1) && (symbol < '9' + 1))
         {
            #ifdef OFF_T32
            if (symbol == '0') sscanf(p, "%lx", &cursor);
            else               sscanf(p, "%ld", &cursor);
            #else
            if (symbol == '0') sscanf(p, "%llx", &cursor);
            else               sscanf(p, "%lld", &cursor);
            #endif
            cursor *= 3;
            lseek(f, cursor, SEEK_SET);
         }

         if (symbol == '-') reflag(p + 1);
      }

      close(f);
   }

   if (s < 0)
   {
   }
   else close(s);

   printf("\n");
   return 0;
}

static int fieldcmp(char *table, int offset, char *string, int bits)
{
   unsigned char	*q = table + (offset >> 3);
   int			 bias = offset & 7;
   int			 octets = bits >> 3;
   int			 final_mask = 255 & (0xFFFFFF00 >> (bits & 7));

   int			 left,
			 right;


   while (octets--)
   {
      left = *q++;
      left <<= 8;

      left |= *q;

      left <<= bias;
      right = *string++;
      right <<= 8;
      right ^= left;
      right &= 0xFF00;
      if (right) return right;
   }

   if (final_mask)
   {
      right = *q;
      right <<= bias;
      right ^= *string;
   }

   right &= final_mask;
   return right;
}

