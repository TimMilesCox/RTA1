/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file fsformat.c is free software.

    fsformat.c is a utility program for constructing file system
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
#include <stdlib.h>
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

#define	PAGE (unsigned)	262144
#define	GRANULE		64
#define	DIRECTORY_BLOCK	1024
#define	LEEWAY		GRANULE

#define	GRANULES	BANKS_IN_DEVICE * (PAGE/GRANULE)


/********************************************************

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
#define VOLUME1_WORDS	3
#define DIRECTORY_WORDS 4
#define FILE_WORDS      8
#define	LINK_WORDS	3

#define	VOLUME_LABEL_OFFSET 15

typedef struct { msw		    rfw,
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

	directory blocks are by default 16 granules of
	64 words. Directory blocks do not straddle bank
	boundaries of 262144 words

********************************************************/


typedef struct { extent1             ex;
                 msw	name[DIRECTORY_BLOCK
		- CONTROL_WORDS   - 1
		- 5 * 2
		- EXTENT1_WORDS   - 1] ; } file_tree;

typedef struct { extent1	     ex;
                 msw    name[DIRECTORY_BLOCK
                - CONTROL_WORDS   - 1
                - 5 * 2
                - VOLUME1_WORDS   - 1] ; } volume;



typedef struct { page_control	  space;
		 forward	 label1,
				 label2;
	union  { volume	   v;
		 file_tree d; }	 label3; } tree;



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
                           { 0       } ,
                         { { '.' } } } ,


			 { { 'L', 0, 4 } ,
                           { 0 } ,
                           { 0, 0, 0, 0, 0, 0 } ,
                         { { '.', '.' } } } } ;


static msw eopage = { 'E', 0, 0 } ;

#if DIRECTORY_BLOCK > GRANULE
static msw            free_extent[DIRECTORY_BLOCK - GRANULE] = { { 'P', 0, 4 },  { 0, 0, 0 },  { 0, 0, 0 },  { 0, 0, 0 },  { 0, 0, 0 } } ;
#endif

static int		 f;

static unsigned		 pointer1 = CONTROL_WORDS + 1 + 2 * 5 + VOLUME1_WORDS + 1 + 2;
static unsigned		 remainder1 = DIRECTORY_BLOCK - CONTROL_WORDS - 1 - 2 * 5 - VOLUME1_WORDS - 1 - 2;
static unsigned long long gpointer = DIRECTORY_BLOCK / GRANULE;


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

static void lstore(long long value, unsigned char *to)
{
   int           index = 5;

   to[index] = value;
   while (index--) to[index] = (value = value >> 8);
}


static int interpret(tree *actual, unsigned *displacement, long long dstart_granule, forward *up1)
{
   static dmsw		 restart_offset = { 0, 0, 0, 0, 0, 16 } ;
   static dmsw		 restart_link   = { 0, 0, 0, 0, 0, 0  } ;
   static msw		 start_zero        =          { 0, 0, 0  } ;

   static msw		 extrahead            = { 'X', 0, EXTENT2_WORDS  } ;

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

   int			 slot = PAGE / GRANULE - (gpointer & 4095);
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

   int			 byword;
   msw			 bypass = { 128, 0, 0 } ;
   msw			*indexp = (msw *) &labelv;

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

      gpointer = DIRECTORY_BLOCK / GRANULE;
      label1.label3.v.ex.granule = restart_offset;

      pointer1 += CONTROL_WORDS + 1 + 2 * 5 + 1;

      indexp = ((msw *) &label1) + DIRECTORY_BLOCK - 1;
      *indexp = eopage;      
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
         printf("%d - %s does not go\n"
                "use fxformat\n",
                DIRECTORY_BLOCK - *displacement, argument);

         exit(0);
      }

      printf("\t%s/\n", argument);

      #if DIRECTORY_BLOCK > GRANULE
      if (slot < (DIRECTORY_BLOCK / GRANULE))
      {
         printf("directory %s position aligned %d granules\n", argument, slot);
         byword = slot * 64 - 6;     
         bypass.t3 = byword;
         bypass.t2 = byword >> 8;     
         free_extent[5] = bypass;
         write(f, (char *) &free_extent, slot * GRANULE * 3);
         gpointer += slot;
      }
      #endif

      next->ex.rfw.t1 = 'D';
      next->ex.rfw.t3 = EXTENT1_WORDS 
                     + copy(&next->name[0].t1, argument);


      vpointer = 5 + 5 + 5;

      slab = *displacement;
      *displacement = slab + new->ex.rfw.t3 + 1;

      dstart_position = lseek(f, (off_t) 0, SEEK_CUR);

      status = write(f, &label2, DIRECTORY_BLOCK * 3);

      labelv = label2;

      labelv.label1.offset.t3 = slab;
      labelv.label1.offset.t2 = slab >> 8;
      labelv.label1.offset.t1 = slab >> 16;

      lstore(dstart_granule, labelv.label1.granule.octet);

      labelv.label2.offset = up1->offset;
      labelv.label2.granule = up1->granule;

      lstore(gpointer, next->ex.granule.octet);

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

      /***********************************************
        last word of directory block reserved
                for eopage record 450000
        which may be eclipsed by bypass record
        with no following words   800000
      ***********************************************/

      labelv.space.write_point.t3 = vpointer;
      labelv.space.write_point.t2 = vpointer >>  8;
      labelv.space.write_point.t1 = vpointer >> 16;

      labelv.space.remainder.t3 = vremainder;
      labelv.space.remainder.t2 = vremainder >>  8;
      labelv.space.remainder.t1 = vremainder >> 16;

      byword = vremainder;

      /************************************************
        eopage 450000 is written on last word
        which is also last possible position of
        bypass record header 800000 OR free words - 1
      ************************************************/

      indexp = ((msw *) &labelv) + DIRECTORY_BLOCK - 1;
      *indexp = eopage;

      if (byword < 0)
      {
         /*********************************************
                this should be impossible
         *********************************************/
      }
      else
      {
         /*********************************************
                write_point is
                bypass header word 800000 OR free - 1
         *********************************************/

         bypass.t3 = byword;
         bypass.t2 = byword >> 8;

         indexp = ((msw *) &labelv) + vpointer;
         *indexp = bypass;
      }

      lseek(f, (off_t) dstart_position, SEEK_SET);
      write(f, &labelv, DIRECTORY_BLOCK * 3);
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
            printf("%d - %s does not go\n"
                   "use fxformat\n",
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

                  lstore(p64, new->quanta.octet);

                  p64 += GRANULE * sizeof(msw) - 1;
                  p64 /= GRANULE * sizeof(msw);

                  slot = PAGE/GRANULE-(gpointer & (PAGE/GRANULE-1));

                  if (slot > p64) slot = p64;

                  lstore(gpointer, new->ex.granule.octet);

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

                  if (flag['v'-'a']) printf("slot1 %d banks %ld last granules %ld total %lld\n",
                                             slot,
                                             p32,
                                             slab, p64);

                  new->ex.next_offset = start_zero;
                  new->ex.next = restart_link;

                  if ((p32) || (slab))
                  {
                     bremainder = DIRECTORY_BLOCK - *displacement - 7;
                     if (flag['v'-'a']) printf("[%d:%u]\n", bremainder, *displacement);

                     if (bremainder < 0)
                     { 
                        printf("%d - %s - extents does not go\n"
                               "use fxformat\n",
                               DIRECTORY_BLOCK - *displacement, argument);
                        exit(0);
                     }

                     printf("%s extent %d granules %12.12llX\n", argument, slot, gpointer - slot);
                     vpointer = *displacement;
                     *displacement = vpointer + 7;

                     voffset = vpointer & 1023;
                     new->ex.next_offset.t3 = voffset;
                     new->ex.next_offset.t2 = voffset >> 8;
                     new->ex.next_offset.t1 = 0;

                     vpointer >>= 10;
                     apointer = dstart_granule + vpointer;

                     lstore(apointer, new->ex.next.octet);
                  }

                  while (p32--)
                  {
                     printf("%llx:%x->", apointer, voffset);
                     printf("%s extent %d granules %llx\n", argument, PAGE/GRANULE, gpointer);
                     extra->rfw = extrahead;
                 
                     extra->granules.t1 = (PAGE/GRANULE) >> 16;
                     extra->granules.t2 = (PAGE/GRANULE) >>  8;
                     extra->granules.t3 = (PAGE/GRANULE) & 255;

                     lstore(gpointer, extra->granule.octet);

                     extra->next = restart_link;
		     extra->next_offset = start_zero;

                     if ((p32) || (slab))
                     {
                        bremainder = DIRECTORY_BLOCK - *displacement - 7;
                        if (flag['v'-'a']) printf("[%d:%u]\n", bremainder, *displacement);

                        if (bremainder < 0)
                        {
                           printf("%d - %s - extents does not go\n"
                                  "use fxformat\n",
                                 DIRECTORY_BLOCK - *displacement, argument);
                           exit(0);
                        }

                        vpointer = *displacement;
                        *displacement = vpointer + 7;

                        voffset = vpointer & 1023;
                        extra->next_offset.t3 = voffset;
                        extra->next_offset.t2 = voffset >> 8;
                        vpointer >>= 10;
                        apointer = dstart_granule + vpointer;

                        lstore(apointer, extra->next.octet);
                     }

                     gpointer += PAGE/GRANULE;
                     extra++;
                  }

                  if (slab)
                  {
                     printf("%llx:%x->", apointer, voffset);
                     printf("%s extend %ld granules %12.12llX\n", argument, slab, gpointer);
                     extra->rfw = extrahead;

                     extra->granules.t1 = slab >> 16;
                     extra->granules.t2 = slab >>  8;
                     extra->granules.t3 = slab;

                     lstore(gpointer, extra->granule.octet);

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
                         
                      status = write(f, fbuffer, GRANULE * sizeof(msw));
                  }

                  close(f2);
               }
            }
         }
      }
   }

   return 1;
}


/*******************************************************

	the job to do here is scan the specification
	text, measure and read the test files requested,
	and write the emulated file system image

*******************************************************/



int main(int argc, char *argv[])
{
   int			 status,
                         symbol;

   long long             net_granules = GRANULES;
   int			 net_banks = BANKS_IN_DEVICE;
   char			*uptr;

   int			 byword;
   msw			 bypass = { 128, 0, 0 } ;
   msw                  *indexp = (msw *) &label1;

   argue(argc, argv);

   if (arguments)
   {
      #if 0
      #ifdef DOS
      f = open(argument[0], O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0777);
      #else
      f = open(argument[0], O_RDWR | O_CREAT | O_TRUNC, 0777);
      #endif

      if (f < 0) printf("file at argument 1 cannot be written %d\n", errno);
      else
      {
         status = write(f, &label1, DIRECTORY_BLOCK * 3);
         if (status < 0) printf("write error %d\n", errno);

         for (;;)
         {
            if (interpret(&label1, &pointer1, 0, &label1.label1) == 0) break;
         }
      }
      #endif

      if (arguments > 1)
      {
         uptr = argument[1];
         symbol = *uptr;

         if ((symbol < '0') || (symbol > '9'))
         {
         }
         else
         {
            if (symbol == '0') sscanf(uptr, "%llx", &net_granules);
            else               sscanf(uptr, "%lld", &net_granules);

            if      (uflag['T'-'A']) net_granules <<= 34;
            else if (uflag['G'-'A']) net_granules <<= 24;
            else if (uflag['M'-'A']) net_granules <<= 14;
            else if  (flag['b'-'a']) net_granules <<= 12;
            else if  (flag['p'-'a']) net_granules <<=  6;
            else if  (flag['k'-'a']) net_granules <<=  4;

            net_banks = (net_granules + 4095) >> 12;
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
         status = write(f, &label1, DIRECTORY_BLOCK * 3);
         if (status < 0) printf("write error %d\n", errno);

         for (;;)
         {
            if (interpret(&label1, &pointer1, 0, &label1.label1) == 0) break;
         }
      }

      label1.label3.v.ex.granules.t3 = net_banks;
      label1.label3.v.ex.granules.t2 = net_banks >>  8;
      label1.label3.v.ex.granules.t1 = net_banks >> 16;

      net_granules -= gpointer;

      printf("%lld granules written, %lld free\n", gpointer, net_granules);


      remainder1 = DIRECTORY_BLOCK - pointer1 - 1;

      /***********************************************
        last word of root directory block is reserved
        for eopage record 450000
        or bypass record header with zero words after
                          800000
        which may eclipse eopage record
      ***********************************************/

      label1.space.write_point.t1 = pointer1 >> 16;
      label1.space.write_point.t2 = pointer1 >>  8;
      label1.space.write_point.t3 = pointer1;

      label1.space.remainder.t1 = remainder1 >> 16;
      label1.space.remainder.t2 = remainder1 >>  8;
      label1.space.remainder.t3 = remainder1;

      /************************************************
        length in bypass record header
        is the same as space.remainder
      ************************************************/

      byword = remainder1;

      if (byword < 0)
      {
         /*********************************************
                this should be impossible
         *********************************************/
      }
      else
      {
         /*********************************************
            write_point is
            bypass record header word 800000 + free - 1
         *********************************************/

         bypass.t3 = byword;
         bypass.t2 = byword >> 8;
         indexp += pointer1;
         *indexp = bypass;
      }
      
      lstore(gpointer, label1.label3.v.ex.granule.octet);

      lseek(f, (off_t) 0, SEEK_SET);
      status = write(f, &label1, DIRECTORY_BLOCK * 3);
      if (status < 0) printf("write error %d\n", errno);      

      close (f);
   }
   else printf("an output filename for the file system image is required\n");

   return 0;
}
