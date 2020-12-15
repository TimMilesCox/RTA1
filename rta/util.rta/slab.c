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
#ifdef DOS
#include <sys/types.h>
#include <fcntl.h>
#define	off_t	__int64
#define	lseek	_lseeki64
#else
#include <unistd.h>
#include <sys/fcntl.h>
#endif

#include "../include.rta/argue.h"

#if 0
#ifdef	DOS
#define	OFF_T32
#endif
#endif

#define	BYTES_TARGETW	3

typedef struct { unsigned char		  parity,
				t[BYTES_TARGETW];	} word;

int main(int argc, char *argv[])
{
   word			 data = { 0, { 0, 0, 0}  } ;

   unsigned int		 words,
                         low_address = 0,
                         high_address = 1024,
                         rom_words = 1024;

   int			 x, y;
   int			 i, o;
   int			 symbol;

   unsigned int		 sum, checksum, carry;

   off_t		 location,
			 position,
			 new_location,
			 next_location = 0,
                         high_location = 0;

   unsigned char	 header[12];
   unsigned char	*p;


   argue(argc, argv);

   if (flag['h'-'a'])
   {
      printf("\nslab [-options] input output[.rom ] [ target-rom-words ] [ lower-address-limit ]\n\n");

      printf("default ROM size is 1024 RTA1 words\n");
      printf("target-rom-words is leading-zero for hex else decimal\n\n");

      printf("slab ifile ofile -g          #  64 RTA1 words\n");
      printf("slab ifile ofile -p          #  4096 RTA1 words\n");
      printf("slab ifile ofile -b          #  262144 RTA1 words\n");
      printf("slab ifile ofile -m          #  1048576 RTA1 words\n\n");

      printf("slab ifile ofile number      #  number of RTA1 words\n");
      printf("slab ifile ofile number -g   #  number * 64 RTA1 words\n");
      printf("slab ifile ofile number -k   #  number * 1024 RTA1 words\n");
      printf("slab ifile ofile number -p   #  number * 4096 RTA1 words\n");
      printf("slab ifile ofile number -b   #  number * 262144 RTA1 words\n");
      printf("slab ifile ofile number -M   #  number * 1048576 RTA1 words\n");
      printf("slab ifile ofile number -G   #  number * 1073741824 RTA1 words\n");

      printf("options -v verbose -w very verbose\n\n");
      return 0;
   }

   if (flag['v' - 'a']) printf("%s %s %s\n",
                               argv[0], argument[0], argument[1]);

   if (arguments > 1)
   {
      #ifdef DOS
      i = open(argument[0], O_RDONLY | O_BINARY, 0644);
      #else
      i = open(argument[0], O_RDONLY, 0644);
      #endif

      if (i < 0)
      {
         printf("load strings file not available to read\n");
      }
      else
      {
         #ifdef DOS
         o = open(argument[1], O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0644);
         #else
         o = open(argument[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
         #endif

         if (o < 0)
         {
            printf("image file cannot be written\n");
         }
         else
         {
             if (flag['g'-'a']) rom_words = 64;
             if (flag['p'-'a']) rom_words = 4096;
             if (flag['b'-'a']) rom_words = 262144;
            if (uflag['M'-'A']) rom_words = 1048576;
            if (uflag['M'-'A']) rom_words = 1073741824;

            if (arguments > 2)
            {
               p = argument[2];
               if (*p == '0') sscanf(p, "%x", &rom_words);
               else           sscanf(p, "%d", &rom_words);

               if       (flag['g'-'a']) rom_words <<=  6;
               else if  (flag['k'-'a']) rom_words <<= 10;
               else if  (flag['p'-'a']) rom_words <<= 12;
               else if  (flag['b'-'a']) rom_words <<= 18;
               else if (uflag['M'-'A']) rom_words <<= 20;
               else if (uflag['G'-'A']) rom_words <<= 30;
            }

            high_address = rom_words;

            if (arguments > 3)
            {
               if       (*argument[3] == '0') sscanf(argument[3], "%x", &low_address);
               else if ((*argument[3]  > '0') &&  (*argument[3]  < '9' + 1))
                                              sscanf(argument[3], "%d", &low_address);
               else printf("low address argument 4 must be a number leading zero for hex else decimal\n");

               high_address += low_address;
               printf("space %x..%x\n", low_address, high_address);
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

               #ifdef OFF_T32
               printf("%ld bytes written\n", location);
               #else
               printf("%lld bytes written\n", location);
               #endif
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
                     {		
                        #ifdef OFF_T32
                        printf("writing file hex byte position %lx "
                                    " target hex word position %lx"
                                                " decimal words %d\n",
                                             location * BYTES_TARGETW,
                                                             location,
                                                                words);
                        #else
                        printf("writing file hex byte position %llx "
                                    " target hex word position %llx"
                                                " decimal words %d\n",
                                             location * BYTES_TARGETW,
                                                             location,
                                                                words);
                        #endif
                     }

                     if (words & 0x00800000)
                     {
                        printf("fixup tuple %6.6x\n", words);

                        if (uflag['S'-'A'])
                        {
                           printf("skipped: output is for examination only\n"
                                  "input is for link load\n");
                           continue;
                        }

                        else printf("option -S to generate unfixed rom image "
                                    "for examination only\n");
                     }

                     next_location = location + words - 1;

                     if (next_location < high_address)
                     {
                     }
                     else
                     {
                        #ifdef OFF_T32
                        printf("load string target words"
                                  " @ hex %6.6lx..%6.6lx"
                               " beyond prepared space\n",
                                                 location,
                                            next_location);
                        #else 
                        printf("load string target words"
                                " @ hex %6.6llx..%6.6llx"
                               " beyond prepared space\n",
                                                 location,
                                            next_location);
                        #endif
                        break;
                     }

                     if (next_location > high_location)
                     {
                        high_location = next_location;
                     }

                     position = location - low_address;
                     new_location = lseek(o, position * BYTES_TARGETW, SEEK_SET);

                     if (new_location < 0)
                     {
                        #ifdef OFF_T32
                        printf("%s write hex byte position %ld out of range\n",
                                                                   argument[1],
                                                     location * BYTES_TARGETW);
                        #else
                        printf("%s write hex byte position %lld out of range\n",
                                                                    argument[1],
                                                      location * BYTES_TARGETW);
                        #endif
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

                              while ((carry = sum >> 24))
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
                           #ifdef OFF_T32
                           printf("%s checksum failed in string @ %lx ["
                                                       "%6.6lx:%6.6lx]\n",
                                                              argument[0],
                                                                 location,
                                                            checksum, sum);
                           #else
                           printf("%s checksum failed in string @ %llx ["
                                                         "%6.6x:%6.6x]\n",
                                                              argument[0],
                                                                 location,
                                                            checksum, sum);
                           #endif
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
               #ifdef OFF_T32
               printf("%s\n"
                      "latest write to target word @ %6.6lx hex\n",
                                                       argument[1],
                                                    next_location);

               printf("highest write to target word @ %6.6lx hex\n",
                                                     high_location);
               #else
               printf("%s\n"
                      "latest write to target word @ %6.6llx hex\n",
                                                        argument[1],
                                                     next_location);

               printf("highest write to target word @ %6.6llx hex\n",
                                                      high_location);
               #endif
            }

            close (o);
         }

         close(i);
      }
   }

   return 0;
}

