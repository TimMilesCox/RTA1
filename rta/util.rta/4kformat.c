/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file 4kformat.c is free software.

    4kformat.c is a utility program for constructing file system
    images for the freeware processor architecture

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
#include <string.h>
#include <errno.h>
#define off_t	__int64
#define lseek	_lseeki64
#else
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#endif

#include "../engine.rta/emulate.h"
#include "../include.rta/argue.h"

#define MYGETS
#define	LF	'\n'
#define	CR	'\r'
#define	HT	'\t'
#define	ZERO	0

#define UPLINKS	83
#define DATA 120

#define	PAGE		4096
#define	GRANULE		64
#define	DIRECTORY_BLOCK	128
#define	LEEWAY		GRANULE
#define	PAGES_IN_DEVICE	1
#define DIRECTORY_WORDS 4
#define FILE_WORDS      8

#define	GRANULES	PAGES_IN_DEVICE * (PAGE/GRANULE)




/********************************************************

	for file systems embedded in sequential
	4k-pages of an executable space ROM image

	in case there are not any filestore devices
	and some readonly files and documents are needed

	similar to the device array file systems
	formated with fsformat, except the bank
	and maximum extent is 4096 words
	instead fo 262144 words

	a page-control record type 'P' is found at the
	start of each directory block

	its forward pointer is nonzero if there is
	another block. It has an insert pointer to its
	next writable word and a count of remaining
	available write locations

	a link record type 'L' (typedef struct forward)
	points to the entry for a file or a directory with
	a pointer to the target directory block and a word
	offset

	the link "." in each directory points to its
	directory entry in the owning directory
	(one above ".")

	the link ".." points to the directory entry of
	the owner in the directory owning the owner
	(two above ".")

	structure extent1 describes directory space. The
	first extent1 descriptor in a chain is within the
	directory entry record which names the directory

	structure extent2 describes file space. The first
	extent2 descriptor in a chain is within the file
	entry record which names the file

	extent2 records contain more information than
	extent1 records because an extent1 record only
	identifies a directory block and its size in 
	granules. page_control record in the target
	directory block describes it internally and
	identifies the next directory block in chain

	extent2 records identify a file extent and its
	size in granules. Three more words identify
	the directory block and word offset of the
	next file extent descriptor record

	record size words in  bits 7..0 of the record
	format word does not include the record format
	word itself

********************************************************/

#define	EXTENT1_WORDS	3
#define	EXTENT2_WORDS	6
#define	CONTROL_WORDS	4
#define	VOLUME1_WORDS	3

#define	VOLUME_LABEL_OFFSET 15

typedef struct { msw                rfw,
                            write_point,
                              remainder;
		 dmsw granule_next_page;  } page_control;

typedef struct { msw		    rfw;
		 msw		 offset;
		 dmsw		granule;
                 msw		name[1];  } forward;

typedef struct { msw                rfw,
                               granules;
                 dmsw           granule;  } extent1;

typedef struct { msw		    rfw,
			       granules;
		 dmsw		granule,
		 		   next;
		 msw	    next_offset;  } extent2;

/********************************************************

	directory blocks contain directory descriptors
	record type 'D' and file descriptors record type
	'F'

	directory descriptors are an extent1 descriptor
	and a name

	file descriptors are an extent2 descriptor
	and a name

	a directory block at volume start contains
	the volume label record type 'V' in addition
	to top-level directory and file entries

	the volume label shows the number of banks in the
	device, and points to next unassigned filestore
	granule

	directory blocks are by default 4 granules of
	64 words. Directory blocks do not straddle bank
	boundaries of 4096 words

********************************************************/


typedef struct { extent1             ex;
                 msw    name[DIRECTORY_BLOCK
                - CONTROL_WORDS   - 1
                - 5 * 2
                - EXTENT1_WORDS   - 1] ; } file_tree;

typedef struct { extent1	     ex;
                 msw    name[DIRECTORY_BLOCK
                - CONTROL_WORDS   - 1
                - 5 * 2
                - VOLUME1_WORDS   - 1] ; } volume;



typedef struct { page_control     space;
                 forward         label1,
                                 label2;
        union  { volume    v;
                 file_tree d; }  label3; } tree;


/*******************************************************

	file descriptor contains the first
	extent descriptor of the file

*******************************************************/


typedef struct { extent2	     ex;
		 dmsw		 quanta;
		 msw	        name[1]; } file;



static tree label1  =  { { { 'P', 0, CONTROL_WORDS } } ,

                         { { 'L', 0, 4 } ,
                           { 0, 0, VOLUME_LABEL_OFFSET } ,
                           { 0, 0, 0, 0, 0, 0 } ,
                         { { '.' } } } ,

                         { { 'L', 0, 4 } ,
                           { 0 } ,
                           { 255, 255, 255, 255, 255, 255 } ,
                         { { '.', '.' } } } ,

                         { { { { 'V', 0, VOLUME1_WORDS + 2 } ,
                             { 0 } ,
                             { 0, 0, 0, 0, 0, DIRECTORY_BLOCK / GRANULE } } ,
                           { { 'F', 'S', '0' } , { '0', '0', '1' } } } } } ;



static tree label2  =  { { { 'P', 0, CONTROL_WORDS } } ,

			 { { 'L', 0, 4 }  ,
                           { 0 } ,
                           { 0     } ,
                         { { '.' } } } ,


			 { { 'L', 0, 4 } ,
                           { 0 } ,
                           { 0, 0, 0, 0, 0, 0 } ,
                         { { '.', '.' } } } } ;

static msw eopage = { 'E', 0, 0 } ;


static int		 f;

static unsigned          pointer1 = CONTROL_WORDS + 1 + 2 * 5 + VOLUME1_WORDS + 1 + 2;
static unsigned          remainder1 = DIRECTORY_BLOCK - CONTROL_WORDS - 1 - 2 * 5 - VOLUME1_WORDS - 1 - 2;
static unsigned long long gpointer = DIRECTORY_BLOCK / GRANULE;
static unsigned long long fs_offset;


#ifdef MYGETS
static char *mygets(char *to, int limit)
{
   char	*data = to;
   int			 bytes = limit;
   int			 symbol = 0;

   while (bytes--)
   {
      symbol = getchar();
      while (symbol == CR) symbol = getchar();
      if (symbol ==  LF) break;
      if (symbol == EOF) break;
      if (symbol ==  HT) symbol = SP;
      *data++ = symbol;
   }

   *data = ZERO;
   
   if (symbol == LF) return to;
   bytes++;
   if (bytes ^ limit) return to;
   return NULL;
}
#endif

static int copy(unsigned char *to, char *from)
{
   int			 distance = 0;

   while ((*to++ = *from++)) distance++;
   return (distance + 2) / 3;
}

static int outputw(int f, char *data, int words)
{
   char image[8];

   int		 bytes = 0,
		 written;

   int		 across = 8;

   if (flag['x'-'a'])
   {
      while (words--)
      {
         sprintf(image, "%2.2X", *data++);
         sprintf(image + 2, "%2.2X", *data++);
         sprintf(image + 4, "%2.2X", *data++);
         image[6] = ' ';
	 across--;

         if (across == 0)
         {
            image[6] = '\n';
            across = 8;
         }

         if (words == 0) image[6] = '\n';

         written = write(f, image, 7);

         if (written < 0)
         {
            printf("write error %d\n", errno);
            return written;
         }

         bytes += written;
      }

      return bytes;
   }

   return write(f, data, words * 3);
}

static void output_label(int f, char *name, long long position)
{
   char	 image[272];
   int			 bytes = sprintf(image, "\n+%s:$20:%12.12llX\n",name,position);
   int			 written = write(f, image, bytes);

   if (written < 0) printf("label write error %d\n", errno);
}

static int interpret(tree *actual, unsigned *displacement, long long dstart_granule, forward *up1)
{
   static dmsw		 restart_offset = { 0, 0, 0, 0, 0, DIRECTORY_BLOCK / GRANULE } ;
   static dmsw		 restart_link   = { 0, 0, 0, 0, 0, 0  } ;
   static msw		 start_zero        =          { 0, 0, 0  } ;

   static msw		 extrahead            = { 'X', 0, EXTENT2_WORDS  } ;

   #if DIRECTORY_BLOCK > GRANULE
   static msw            free_extent[DIRECTORY_BLOCK - GRANULE] = { { 'f', 'r', 'e' },  { 'e' } } ;
   #endif

   char			 data[DATA + 4];
   char			*rp;

   char			 command[12];
   char			 argument[52];
   char			 path[360];

   unsigned long	 vpointer = 5 + 5 + 5;
   unsigned long	 vremainder;

   long			 p32 = 3 * (*displacement) + (long) actual;
   file_tree		*next = (file_tree *) p32;
   file			*new  = (file *) p32;
   extent2		*extra;

   tree			 labelv;

   int			 slot = 64 - (gpointer & 63);
   int			 voffset;
   int			 status;
   int			 bremainder;

   long                  slab;

   off_t		 p64;

   int			 f2;

   char			 fbuffer[GRANULE * sizeof(msw)];

   long long		 apointer;

   off_t                 dstart_position,
			 zero_position;

   int                   byword;
   msw                   bypass = { 128, 0, 0 } ;
   msw                  *indexp = (msw *) &labelv;

   #ifdef MYGETS
   rp = mygets(data, DATA);
   #else
   rp = fgets(data, DATA, stdin);
   #endif

   if (rp == NULL) return 0;
   if (*rp == '.') return 0;

   sscanf(rp, "%11s %48s", command, argument);

   if      (strcmp(command, "volume") == 0)
   {
      label1.label3.v.ex.rfw.t3 = pointer1
                                = VOLUME1_WORDS
			        + copy(&label1.label3.v.name[0].t1, argument);

      gpointer = DIRECTORY_BLOCK / GRANULE + fs_offset;
      label1.label3.v.ex.granule = restart_offset;

      pointer1 += CONTROL_WORDS + 1 + 2 * 5 + 1;
   }

   else if (strcmp(command, "tree/")   == 0)
   {
      /************************************************
        placeholder directory entry with no files
        and no space to write file entries
      ************************************************/

      next->ex.rfw.t1 = 'D';
      next->ex.rfw.t3 = EXTENT1_WORDS
                     + copy(&next->name[0].t1, argument);

      next->ex.granules = start_zero;
      next->ex.granule = restart_link;

      *displacement += new->ex.rfw.t3 + 1;
   }

   else if (strcmp(command, "tree")   == 0)
   {
      /************************************************
                        directory
      ************************************************/

      bremainder = DIRECTORY_BLOCK - *displacement - DIRECTORY_WORDS - (strlen(argument) + 2) / 3;
      if (flag['v'-'a']) printf("[%d:%u]\n", bremainder, *displacement);

      if (bremainder < 0)
      {
         printf("%d - %s does not go\n",
                DIRECTORY_BLOCK - *displacement, argument);

         exit(0);
      }

      if (flag['x'-'a'] & flag['y'-'a'])
      output_label(f, argument, dstart_granule * 64 + *displacement);

      next->ex.rfw.t1 = 'D';
      next->ex.rfw.t3 = EXTENT1_WORDS
                     + copy(&next->name[0].t1, argument);

      vpointer = 5 + 5 + 5;

      slab = *displacement;
      *displacement = slab + new->ex.rfw.t3 + 1;

      #if DIRECTORY_BLOCK > GRANULE
      if (slot < (DIRECTORY_BLOCK / GRANULE))
      {
         printf("directory %s position aligned %d granules\n", argument, slot);
         outputw(f, (char *) &free_extent, slot * GRANULE);
         gpointer += slot;
      }
      #endif

      dstart_position = lseek(f, (off_t) 0, SEEK_CUR);
      status = outputw(f, (char *) &label2, DIRECTORY_BLOCK);

      labelv = label2;

      labelv.label1.offset.t3 = slab;
      labelv.label1.offset.t2 = slab >> 8;
      labelv.label1.offset.t1 = slab >> 16;

      labelv.label1.granule.octet[5] = dstart_granule;
      labelv.label1.granule.octet[4] = dstart_granule >>  8;
      labelv.label1.granule.octet[3] = dstart_granule >> 16;
      labelv.label1.granule.octet[2] = dstart_granule >> 24;
      labelv.label1.granule.octet[1] = dstart_granule >> 32;
      labelv.label1.granule.octet[0] = dstart_granule >> 40;

      labelv.label2.offset = up1->offset;
      labelv.label2.granule = up1->granule;

      next->ex.granule.octet[5] = gpointer;
      next->ex.granule.octet[4] = gpointer >>  8;
      next->ex.granule.octet[3] = gpointer >> 16;
      next->ex.granule.octet[2] = gpointer >> 24;
      next->ex.granule.octet[1] = gpointer >> 32;
      next->ex.granule.octet[0] = gpointer >> 40;
      apointer = gpointer;
      gpointer += DIRECTORY_BLOCK / GRANULE;

      for (;;)
      {
         if (interpret(&labelv, (unsigned *) &vpointer, apointer, &labelv.label1) == 0) break;
      }

      slot = PAGE/GRANULE - (gpointer & (PAGE/GRANULE-1));
      slot *= GRANULE;
      if (slot > DIRECTORY_BLOCK) slot = DIRECTORY_BLOCK;

      next->ex.granules.t3 = DIRECTORY_BLOCK >> 6;
      next->ex.granules.t2 = DIRECTORY_BLOCK >> 14;
      next->ex.granules.t1 = DIRECTORY_BLOCK >> 22;

      vremainder = DIRECTORY_BLOCK - 1 - vpointer;

      labelv.space.write_point.t3 = vpointer;
      labelv.space.write_point.t2 = vpointer >>  8;
      labelv.space.write_point.t1 = vpointer >> 16;

      labelv.space.remainder.t3 = vremainder;
      labelv.space.remainder.t2 = vremainder >>  8;
      labelv.space.remainder.t1 = vremainder >> 16;

      byword = vremainder - 1;

      if (byword < 0)
      {
      }
      else
      {
         /*********************************************
                bypass record is an indication of free
                space at the end of a directory page

                Mostly for viewing because file system
                managers use write_point algebraically.
                write_point also points at this spot

         *********************************************/

         bypass.t3 = byword;
         bypass.t2 = byword >> 8;

         indexp += vpointer;
         *indexp = bypass;
      }

      indexp = ((msw *) &labelv) + DIRECTORY_BLOCK - 1;
      *indexp = eopage;

      lseek(f, (off_t) dstart_position, SEEK_SET);
      outputw(f, (char *) &labelv, DIRECTORY_BLOCK);
      lseek(f, (off_t) 0, SEEK_END);
   }
   else if (strcmp(command, "file")   == 0)
   {
      status = sscanf(data, "%s %s %s", command, argument, path);
      if (status < 2) printf("file name [input_path]\n");
      else
      {
         bremainder = DIRECTORY_BLOCK - *displacement - FILE_WORDS - (strlen(argument) + 2) / 3;
         if (flag['v'-'a']) printf("[%d:%u]\n", bremainder, *displacement);

         if (bremainder < 0)
         {
            printf("%d - %s does not go\n",
                   DIRECTORY_BLOCK - *displacement, argument);
            exit(0);
         }

         if (status < 3) strcpy(path, argument);
         #ifdef DOS
         f2 = open(path, O_RDONLY | O_BINARY, 0444);
         #else
         f2 = open(path,     O_RDONLY, 0444);
         #endif

         if (f2 < 0) printf("input file E %d\n", errno);
         else
         {
            if (flag['x'-'a']) output_label(f, argument, dstart_granule * 64
							+ *displacement);
            p64 = lseek(f2, (off_t) 0, SEEK_END);

            if (p64 < 0) printf("end E %d\n", errno);
            else
            {
               zero_position = lseek(f2, (off_t) 0, SEEK_SET);

               if (zero_position < 0) printf("start E %d\n", errno);
               else
               {
                  new->ex.rfw.t1 = 'F';
                  new->ex.rfw.t2 = 0;
                  new->ex.rfw.t3 = EXTENT2_WORDS
                                 + 2
                                 + copy(&new->name[0].t1, argument);

                  *displacement += new->ex.rfw.t3 + 1;

                  new->quanta.octet[5] = p64;
                  new->quanta.octet[4] = p64 >>  8;
                  new->quanta.octet[3] = p64 >> 16;
                  new->quanta.octet[2] = p64 >> 24;
                  new->quanta.octet[1] = p64 >> 32;
                  new->quanta.octet[0] = p64 >> 40;

                  p64 += GRANULE * sizeof(msw) - 1;
                  p64 /= GRANULE * sizeof(msw);

                  slot = PAGE/GRANULE-(gpointer & (PAGE/GRANULE-1));

                  if (slot > p64) slot = p64;

                  new->ex.granule.octet[5] = gpointer;
                  new->ex.granule.octet[4] = gpointer >>  8;
                  new->ex.granule.octet[3] = gpointer >> 16;
                  new->ex.granule.octet[2] = gpointer >> 24;
                  new->ex.granule.octet[1] = gpointer >> 32;
                  new->ex.granule.octet[0] = gpointer >> 40;

                  new->ex.granules.t3 = slot;
                  new->ex.granules.t2 = slot >>  8;
                  new->ex.granules.t1 = slot >> 16;

                  slab = p64 - slot;
                  p32 = slab / (PAGE/GRANULE);
                  slab %= PAGE/GRANULE;

                  gpointer += slot;

                  vpointer = (unsigned long) new;
                  vpointer += (new->ex.rfw.t3 + 1) * 3;
                  extra = (extent2 *) vpointer;

                  printf("%llx slot1 %d banks %ld last granules %ld total %lld\n", gpointer,
                                                                                   slot,
                                                                                   p32,
                                                                                   slab,
                                                                                   p64);

                  new->ex.next_offset = start_zero;
                  new->ex.next = restart_link;

                  if ((p32) || (slab))
                  {
                     bremainder = DIRECTORY_BLOCK - *displacement - 7;
                     if (flag['v'-'a']) printf("[%d:%u]\n", bremainder, *displacement);

                     if (bremainder < 0)
                     {
                        printf("%d - %s - extents does not go\n",
                               DIRECTORY_BLOCK - *displacement, argument);
                        exit(0);
                     }

                     vpointer = *displacement;
                     *displacement = vpointer + 7;

                     voffset = vpointer & 1023;

                     if (voffset > DIRECTORY_BLOCK - 7)
                     {
                        printf("directory extension not yet feasible\n");
                        return 0;
                     }

                     new->ex.next_offset.t3 = voffset;
                     new->ex.next_offset.t2 = voffset >> 8;
                     vpointer >>= 10;
                     apointer = dstart_granule + vpointer;

                     new->ex.next.octet[5] = apointer;
                     new->ex.next.octet[4] = apointer >>  8;
                     new->ex.next.octet[3] = apointer >> 16;
                     new->ex.next.octet[2] = apointer >> 24;
                     new->ex.next.octet[1] = apointer >> 32;
                     new->ex.next.octet[0] = apointer >> 40;
                  }

                  while (p32--)
                  {
                     printf("%llx %s extent %d granules\n", gpointer, argument, PAGE/GRANULE);
                     extra->rfw = extrahead;
                 
                     extra->granules.t1 = (PAGE/GRANULE) >> 16;
                     extra->granules.t2 = (PAGE/GRANULE) >>  8;
                     extra->granules.t3 =  PAGE/GRANULE;

                     extra->granule.octet[5] = gpointer;
                     extra->granule.octet[4] = gpointer >>  8;
                     extra->granule.octet[3] = gpointer >> 16;
                     extra->granule.octet[2] = gpointer >> 24;
                     extra->granule.octet[1] = gpointer >> 32;
                     extra->granule.octet[0] = gpointer >> 40;

                     extra->next = restart_link;
		     extra->next_offset = start_zero;

                     if ((p32) || (slab))
                     {
                        bremainder = DIRECTORY_BLOCK - *displacement - 7;
                        if (flag['v'-'a']) printf("[%d:%u]\n", bremainder, *displacement);

                        if (bremainder < 0)
                        {
                           printf("%d - %s - extents does not go\n",
                                 DIRECTORY_BLOCK - *displacement, argument);
                           exit(0);
                        }

                        vpointer = *displacement;
                        *displacement = vpointer + 7;

                        voffset = vpointer & 1023;

                        if (voffset > DIRECTORY_BLOCK - 7)
                        {
                           printf("directory extension not yet feasible\n");
                           return 0;
                        }

                        extra->next_offset.t3 = voffset;
                        extra->next_offset.t2 = voffset >> 8;
                        vpointer >>= 10;
                        apointer = dstart_granule + vpointer;

                        extra->next.octet[5] = apointer;
                        extra->next.octet[4] = apointer >>  8;
                        extra->next.octet[3] = apointer >> 16;
                        extra->next.octet[2] = apointer >> 24;
                        extra->next.octet[1] = apointer >> 32;
                        extra->next.octet[0] = apointer >> 40;
                     }

                     gpointer += PAGE/GRANULE;
                     extra++;
                  }

                  if (slab)
                  {
                     printf("%llx %s extend %ld granules\n", gpointer, argument, slab);
                     extra->rfw = extrahead;

                     extra->granules.t1 = slab >> 16;
                     extra->granules.t2 = slab >>  8;
                     extra->granules.t3 = slab;

                     extra->granule.octet[5] = gpointer;
                     extra->granule.octet[4] = gpointer >>  8;
                     extra->granule.octet[3] = gpointer >> 16;
                     extra->granule.octet[2] = gpointer >> 24;
                     extra->granule.octet[1] = gpointer >> 32;
                     extra->granule.octet[0] = gpointer >> 40;

                     extra->next = restart_link;
                     gpointer += slab;
                     extra++;
                  }


                  next = (file_tree *) extra;
                  new  = (file *) extra;

                  printf("%s loading %lld granules\n", argument, p64);

                  while (p64--)
                  {
                      status = read(f2, fbuffer, GRANULE * sizeof(msw));

                      if (status < 0)
                      {
                         printf("read E %d\n", errno);
                         break;
                      }

                      if (status < (GRANULE * sizeof(msw)))
                      {
                         if (status) memset(fbuffer + status, 0, GRANULE*sizeof(msw) - status);
                         else break;
                      }
                         
                      status = outputw(f, fbuffer, GRANULE);
                  }

                  close(f2);
               }
            }
         }
      }
   }

   return 1;
}

static void help_out()
{
   printf("\n\n\t4kformat fs[.rom] [granules [-T|G|M|b|p|K]] < specifications [-hvwz]\n"
          "\t\tor\n"
          "\t4kformat -x fs[.txo] [root_directory_offset_granule] < specifications [-hvw]\n\n");

   printf("\trom file system output may opt a size in\n"
          "\t64-word granules | Kwords | 4096-word pages | 262144-word banks | Megawords | Gigawords | Terawords\n\n");

   printf("\ttxo file system output for code inclusion opts a 48-bit root directory offset granule\n"
          "\tto point beyond code addresses. 48-bit file and directory offsets are exported\n\n");

   printf("\t-z\tzero-fill to next 4K page\n");
   printf("\t-v\tverbose\n");
   printf("\t-w\textra verbose\n");
   printf("\t-h\tprint this help\n\n");

   printf("\teach specification starts in the 1st column of a line. Example :\n\n");
   printf("volume volume_name\n");
   printf("tree directory_1\n");
   printf("tree directory_2\n");
   printf("file file_a from_path_1a\n");
   printf("file file_b from_path_1b\n");
   printf(".\n");
   printf("file file_z from_path_2z\n");
   printf(".\n");
   printf(".\n\n");

   printf("\twhere a filename is given with no load path the file must be in the working directory\n"
          "\tcommand . changes to containing directory\n"
          "\tvolume is root directory and . from volume concludes operation\n\n");  
}

/*******************************************************

	scan the specification text
	and construct the file system image

*******************************************************/



int main(int argc, char *argv[])
{
   static char		 outwrite[64];

   int			 status,
			 symbol;

   long long		 net_granules = GRANULES;
   long long		 offset;

   off_t		 position;
   int			 net_pages = PAGES_IN_DEVICE;
   char	*uptr;

   int                   byword;
   msw                   bypass = { 128, 0, 0 } ;
   msw                  *indexp = (msw *) &label1;


   argue(argc, argv);

   if (flag['h'-'a'])
   {
      help_out();
      return 0;
   }

   if (arguments)
   {
      if (arguments > 1)
      {
         uptr = argument[1];
         symbol = *uptr;

         if ((symbol < '0') || (symbol > '9'))
         {
         }
         else
         {
            if (symbol == '0') sscanf(uptr, "%llx", &offset);
            else               sscanf(uptr, "%lld", &offset);
 
            if (flag['x'-'a'])
            {
               if (uflag['T'-'A'] | uflag['G'-'A'] | uflag['M'-'A'] | flag['b'-'a'] | flag['p'-'a'] | uflag['K'-'A'])
               {  
                  printf("\n\n\t-x opted: 2nd positional argument %s is root directory granule offset\n"
                         "\tto test target size duplicate the operation without -x :\n"
                         "\t\t4kformat fs.rom [%s [-T|G|M|b|p|K]] < specifications\n\n", uptr, uptr);

                  printf("\thelp : 4kformat -h\n\n");
                  return 0;
               }

               offset += 63;
               offset >>= 6;
               gpointer += offset;
               fs_offset = offset;
               printf("fs offset in block %llx\n", offset);
               net_granules -= gpointer;
            }
            else
            {
               net_granules = offset;
               if      (uflag['T'-'A']) net_granules <<= 34;
               else if (uflag['G'-'A']) net_granules <<= 24;
               else if (uflag['M'-'A']) net_granules <<= 14;
               else if  (flag['b'-'a']) net_granules <<= 12;
               else if  (flag['p'-'a']) net_granules <<=  6;
               else if (uflag['K'-'A']) net_granules <<=  4;

               net_pages = (net_granules + 63) >> 6;
            }
         }
      }

      #ifdef DOS
      f = open(argument[0], O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0777);
      #else
      f = open(argument[0], O_RDWR | O_CREAT | O_TRUNC, 0777);
      #endif

      if (f < 0) printf("file at argument 1 cannot be written %d\n", errno);
      else
      {
         status = outputw(f, (char *) &label1, DIRECTORY_BLOCK);
         if (status < 0) printf("write error %d\n", errno);

         for (;;)
         {
            if (interpret(&label1, &pointer1, fs_offset, &label1.label1) == 0) break;
         }
      }

      label1.label3.v.ex.granules.t3 = net_pages;
      label1.label3.v.ex.granules.t2 = net_pages >>  8;
      label1.label3.v.ex.granules.t1 = net_pages >> 16;

      net_granules -= gpointer;

      printf("%lld granules written, %lld free\n", gpointer, net_granules);

      remainder1 = DIRECTORY_BLOCK - pointer1 - 1;

      byword = remainder1 - 1;

      if (byword < 0)
      {
      }
      else
      {
         /*********************************************
                bypass record is an indication of free
                space at the end of a directory page

                Mostly for viewing because file system
                managers use write_point algebraically.
                write_point also points at this spot

         *********************************************/

         bypass.t3 = byword;
         bypass.t2 = byword >> 8;
         indexp += pointer1;
         *indexp = bypass;
      }

      indexp = ((msw *) &label1) + DIRECTORY_BLOCK - 1;
      *indexp = eopage; 

      label1.space.write_point.t1 = pointer1 >> 16;
      label1.space.write_point.t2 = pointer1 >>  8;
      label1.space.write_point.t3 = pointer1;

      label1.space.remainder.t1 = remainder1 >> 16;
      label1.space.remainder.t2 = remainder1 >>  8;
      label1.space.remainder.t3 = remainder1;

      label1.label3.v.ex.granule.octet[5] = gpointer;
      label1.label3.v.ex.granule.octet[4] = gpointer >>  8;
      label1.label3.v.ex.granule.octet[3] = gpointer >> 16;
      label1.label3.v.ex.granule.octet[2] = gpointer >> 24;
      label1.label3.v.ex.granule.octet[1] = gpointer >> 32;
      label1.label3.v.ex.granule.octet[0] = gpointer >> 40;

      lseek(f, (off_t) 0, SEEK_SET);
      status = outputw(f, (char *) &label1, DIRECTORY_BLOCK);
      if (status < 0) printf("write error %d\n", errno);      

      if (flag['z'-'a'])
      {
         if (flag['x'-'a']) printf("page fill option -z not supported with txo option -x\n");
         else
         {
            position = lseek(f, (off_t) 0, SEEK_END);
            status = (4096 * 3) - position % (4096 * 3);
            printf("zero fill %d octets\n", status);
            write(f, outwrite, status & 63);
            status >>= 6;
            while (status--) write(f, outwrite, 64);
         }
      }

      close (f);
   }
   else printf("an output filename for the file system image is required\n");

   return 0;
}
