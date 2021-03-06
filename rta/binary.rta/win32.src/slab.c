/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file slab.c is free software.

    slab.c is a utility program for constructing target instruction
    code for the freeware processor architecture

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
#ifdef MSW
#include <fcntl.h>
#include <sys/types.h>
#else
#include <sys/fcntl.h>
#include <unistd.h>
#endif

#include "../include.rta/argue.h"

#define	BYTES_TARGETW	3

typedef struct { unsigned char		  parity,
				t[BYTES_TARGETW];	} word;

int main(int argc, char *argv[])
{
   word			 data = { 0, { 0, 0, 0}  } ;

   int			 words,
                         rom_words = 1024;

   int			 x, y;
   int			 i, o;
   int			 symbol;

   unsigned long	 sum, checksum, carry;

   off_t		 location,
			 next_location = 0,
                         high_location = 0;

   unsigned char	 header[12];
   unsigned char	*p;


   argue(argc, argv);

   if (flag['h'-'a'])
   {
      printf("\nslab [-options] input[.bxl] output[.rom] [target-rom-words]\n\n");

      printf("default ROM size is 1024 RTA1 words\n");
      printf("target-rom-words is leading-zero for hex else decimal\n\n");

      printf("slab ifile ofile -p          #  4096 RTA1 words\n");
      printf("slab ifile ofile -b          #  262144 RTA1 words\n");
      printf("slab ifile ofile -m          #  1048576 RTA1 words\n\n");

      printf("slab ifile ofile number      #  number of RTA1 words\n");
      printf("slab ifile ofile number -k   #  number * 1024 RTA1 words\n");
      printf("slab ifile ofile number -p   #  number * 4096 RTA1 words\n");
      printf("slab ifile ofile number -b   #  number * 262144 RTA1 words\n");
      printf("slab ifile ofile number -m   #  number * 1048576 RTA1 words\n");

      printf("options -v verbose -w very verbose\n\n");
      return 0;
   }

   if (flag['v' - 'a']) printf("%s %s %s\n",
                               argv[0], argument[0], argument[1]);

   if (arguments > 1)
   {
      i = open(argument[0], O_RDONLY | O_BINARY, 0644);

      if (i < 0)
      {
         printf("rbx file not available to read\n");
      }
      else
      {
         o = open(argument[1], O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0644);

         if (o < 0)
         {
            printf("image file cannot be written\n");
         }
         else
         {
            if (flag['p'-'a']) rom_words = 4096;
            if (flag['b'-'a']) rom_words = 262144;
            if (flag['m'-'a']) rom_words = 1048576;

            if (arguments > 2)
            {
               p = argument[2];
               if (*p == '0') sscanf(p, "%x", &rom_words);
               else           sscanf(p, "%d", &rom_words);

               if      (flag['k'-'a']) rom_words <<= 10;
               else if (flag['p'-'a']) rom_words <<= 12;
               else if (flag['b'-'a']) rom_words <<= 18;
               else if (flag['m'-'a']) rom_words <<= 20;
            }

            if (flag['v'-'a']) printf("preparing %d target words\n", rom_words);

            for (x = 0; x < rom_words; x++)
            {
               y = write(o, (void *) data.t, BYTES_TARGETW);

               if (y != BYTES_TARGETW)
               {
                  printf("preparation abandoned\n");
                  break;
               }
            }

            if (flag['v'-'a'])
            {
               location = lseek(o, (off_t) 0, SEEK_CUR);
               printf("%ld bytes written\n", location);
            }

            if (y == BYTES_TARGETW)
            {
               for (;;)
               {
                  x = read(i, header, 12);

                  if (x == 12)
                  {
                     words    = (header[0] << 16)
                              | (header[1] <<  8)
                              |  header[2];

                     checksum = (header[3] << 16)
                              | (header[4] <<  8)
                              |  header[5];

                     location = ((long long) header[6] << 40)
                              | ((long long) header[7] << 32)
                              | (header[8] << 24)
                              | (header[9] << 16)
                              | (header[10]<<  8)
                              |  header[11];


                     if (flag['w'-'a'])
                     printf("writing file hex byte position %lx "
                                 " target hex word position %lx"
                                             " decimal words %d\n",
                                          location * BYTES_TARGETW,
                                                          location,
                                                             words);

                     next_location = location + words - 1;

                     if (next_location < rom_words)
                     {
                     }
                     else
                     {
                        printf("load string target words"
                                " @ hex %6.6lx..%6.6lx"
                               " beyond prepared space\n",
                                                 location,
                                            next_location);
                        break;
                     }

                     if (next_location > high_location)
                     {
                        high_location = next_location;
                     }

                     x = lseek(o, location * BYTES_TARGETW, SEEK_SET);

                     if (x < 0)
                     {
                        printf("%s write hex byte position %ld failed\n",
                                                              argument[1],
                                                 location * BYTES_TARGETW);
                        break;
                     }
                     else
                     {
                        sum = words
                            +  (location        & 0x00FFFFFF)
                            + ((location >> 24) & 0x00FFFFFF);

                        for (x = 0; x < words; x++)
                        {
                           y = read(i, data.t, BYTES_TARGETW);

                           if (y == BYTES_TARGETW)
                           {
                              y = write(o, (void *) data.t, BYTES_TARGETW);

                              if (y != BYTES_TARGETW)
                              {
                                 printf("word not written %s\n", argument[1]);
                                 break;
                              }

                              sum += (data.t[0] << 16)
                                   | (data.t[1] <<  8)
                                   |  data.t[2];

                              while (carry = sum >> 24)
                              {
                                 sum &= 0x00FFFFFF;
                                 sum += carry;
                              }
                           }
                           else
                           {
                              printf("%s 24 bits not read\n", argument[0]);
                              break;
                           }
                        }

                        if (y < 0)
                        {
                           printf("abandon\n");
                           break;
                        }

                        sum ^= 0x00FFFFFF;

                        if (sum ^ checksum)
                        {
                           printf("%s checksum failed in string @ %lx ["
                                                       "%6.6lx:%6.6lx]\n",
                                                              argument[0],
                                                                 location,
                                                            checksum, sum);
                           break;
                        }
                     }
                  }
                  else
                  {
                     if (x)
                     {
                        printf("unexpected termination in %s\n", argument[0]);
                     }

                     break;
                  }

               }
            }

            if (flag['v'-'a'])
            {
               printf("%s\n"
                      "latest write to target word @ %6.6lx hex\n",
                                                        argument[1],
                                                     next_location);

               printf("highest write to target word @ %6.6lx hex\n",
                                                      high_location);
            }

            close (o);
         }

         close(i);
      }
   }

   return 0;
}

