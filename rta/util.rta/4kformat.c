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
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <errno.h>
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
#define	DIRECTORY_BLOCK	1024
#define	LEEWAY		GRANULE
#define	PAGES_IN_DEVICE	1

#define	GRANULES	PAGES_IN_DEVICE * (PAGE/GRANULE)




/********************************************************

	a forward record is found at the end of a
	block of a volume or tree list if that
	has needed to be extended

********************************************************/

#define	EXTENT1_WORDS	3
#define	EXTENT2_WORDS	6
#define	CONTROL_WORDS	4

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


	A Tree at the start of the 
	Volume contains the first free extent block
	in the volume and it's initialised to the
	whole device 

	volume tree has one extra word of granule
	free count in front of the name, because
	the total may be more than 16M granules = 1GW

	The extra count word allows 256 teragranules
	although some file access structures only
	reach 256 terawords

	write_point and remainder are about where
	more information can go in this directory
	block

	this initial free extent gets smaller as files are
	assigned and gets extent descriptors chained onto
	it as files are freed

	only the free extent pieces can be larger than
	PAGE words or 4096 granules,
	and nothing assigned straddles a bank boundary

	the extent block pointer in the tree increments
	as its leading edge is sliced. The block pointer
	in an extent in use stays how it is

	how successful this will be managing extents
	assignable from one to 4096 small granules
	in size remains to be seen

	It isn't wished to assign files of a couple of
	text lines 1K words at a time, or to have a
	class of small files or subfiles either

	the forward pointer of an extent descriptor
	is 48 bits because it can be anywhere on the
	device in another block of the containing list
	and it is a granule address. Six bits of the
	next word are a word offset

	the granule address is also two words because
	theoretically a device can have 256 MegaGranules

	directory blocks are assigned at 1024 words
	per scoop, but on any granule.octetoundary. If that
	brings them over a bank boundary they get an
	exceptional small directory block. If they get
	64 words it should be some use, but if it isn't
	any use it will just contain a forward record
	-you must always leave 3+ words for the forward
	record at the end of any directory block

	the plan is different for any files where 
	table-like random accesses all intended


********************************************************/


typedef struct { extent1             ex;
                 msw        name[360-4]; } file_tree;

typedef struct { page_control	  space;
		 forward	 label1,
				 label2;
		 file_tree	 label3;
		 msw
      dspace[DIRECTORY_BLOCK-3-5-5-360];  } tree;


/*******************************************************

	file also contains an extent which is the first
	extent of the file

*******************************************************/


typedef struct { extent2	     ex;
		 dmsw		 quanta;
		 msw	        name[1]; } file;





static tree label1  =  { { { 'P', 0, CONTROL_WORDS } } ,

			 { { 'L', 0, 4 } ,
                           { 0, 0, 10 } ,
                           { 0, 0, 0, 0, 0, 0 } ,
                         { { '.' } } } ,

			 { { 'L', 0, 4 } ,
                           { 0 } ,
                           { 255, 255, 255, 255, 255, 255 } ,
                         { { '.', '.' } } } ,

			 { { { 'V', 0, EXTENT1_WORDS + 2 + 1 } ,
                             { 0 } ,
                             { 0, 0, 0, 0, 0, 16 } } ,
                           { { 0, 0, 0 } ,
                             { 'F', 'S', '0' } , { '0', '0', '1' } } } } ;


static tree label2  =  { { { 'P', 0, CONTROL_WORDS } } ,

			 { { 'L', 0, 4 }  ,
                           { 0 } ,
                           { 0     } ,
                         { { '.' } } } ,


			 { { 'L', 0, 4 } ,
                           { 0 } ,
                           { 0, 0, 0, 0, 0, 0 } ,
                         { { '.', '.' } } } } ;



static int		 f;

static unsigned		 pointer1 = 5 + 5 + 5 + 7;
static unsigned		 remainder1 = 1024 - 5 - 5 - 5 - 7;
static unsigned long long gpointer = 16;


#ifdef MYGETS
static unsigned char *mygets(unsigned char *to, int limit)
{
   unsigned char	*data = to;
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

static int copy(char *to, char *from)
{
   int			 distance = 0;

   while (*to++ = *from++) distance++;
   return (distance + 2) / 3;
}

static int outputw(int f, unsigned char *data, int words)
{
   unsigned char image[8];

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

static void output_label(int f, unsigned char *name, long long position)
{
   unsigned char	 image[272];
   int			 bytes = sprintf(image, "\n+%s:$20:%6.6llX\n",name,position);
   int			 written = write(f, image, bytes);

   if (written < 0) printf("label write error %d\n", errno);
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
   char			 argument[48];
   char			 path[360];

   unsigned long	 vpointer = 5 + 5 + 5;
   unsigned long	 vremainder;

   long			 p32 = 3 * (*displacement) + (long) actual;
   file_tree		*next = (file_tree *) p32;
   file			*new  = (file *) p32;
   extent2		*extra;

   tree			 labelv;

   int			 slot = 4096 - (gpointer & 4095);
   int			 status;

   long                  slab;

   unsigned long long	 p64;

   int			 f2;

   char			 fbuffer[GRANULE * sizeof(msw)];

   long long		 apointer;
   off_t                 dstart_position;

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

   sscanf(rp, "%s %s", command, argument);

   if      (strcmp(command, "volume") == 0)
   {
      label1.label3.ex.rfw.t3 = pointer1
                              = EXTENT1_WORDS
			      + 1	/* volume has extra ms granule count */
			      + copy(&label1.label3.name[1].t1, argument);

      gpointer = 16;
      label1.label3.ex.granule = restart_offset;

      pointer1 += 5 + 5 + 5 + 1;
   }

   else if (strcmp(command, "tree")   == 0)
   {
      next->ex.rfw.t1 = 'D';
      next->ex.rfw.t3 = EXTENT1_WORDS
                     + copy(&next->name[0].t1, argument);

      vpointer = 5 + 5 + 5;

      slab = *displacement;
      *displacement = slab + new->ex.rfw.t3 + 1;

      dstart_position = lseek(f, (off_t) 0, SEEK_CUR);
      status = outputw(f, &label2, DIRECTORY_BLOCK);

      labelv = label2;
      labelv.label1.offset.t3 = slab;

      #if 1
      labelv.label1.granule.octet[5] = dstart_granule;
      labelv.label1.granule.octet[4] = dstart_granule >>  8;
      labelv.label1.granule.octet[3] = dstart_granule >> 16;
      labelv.label1.granule.octet[2] = dstart_granule >> 24;
      labelv.label1.granule.octet[1] = dstart_granule >> 32;
      labelv.label1.granule.octet[0] = dstart_granule >> 40;
      #endif

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
         if (interpret(&labelv, &vpointer, apointer, &labelv.label1) == 0) break;
      }

      #if 1
      #else
      labelv.label1.ex.granule = next->label1.ex.granule;
      #endif

      slot = PAGE/GRANULE - (gpointer & (PAGE/GRANULE-1));
      slot *= GRANULE;
      if (slot > DIRECTORY_BLOCK) slot = DIRECTORY_BLOCK;

      next->ex.granules.t3 = slot >> 6;
      next->ex.granules.t2 = slot >> 14;
      next->ex.granules.t1 = slot >> 22;

      #if 0
      labelv.label1.ex.granules = next->label1.ex.granules;
      #endif

      vremainder = slot - vpointer;

      #if 1
      labelv.space.write_point.t3 = vpointer;
      labelv.space.write_point.t2 = vpointer >>  8;
      labelv.space.write_point.t1 = vpointer >> 16;

      labelv.space.remainder.t3 = vremainder;
      labelv.space.remainder.t2 = vremainder >>  8;
      labelv.space.remainder.t1 = vremainder >> 16;
      #else
      next->write_point.t3 = vpointer;
      next->write_point.t2 = vpointer >>  8;
      next->write_point.t1 = vpointer >> 16;

      next->remainder.t3 = vremainder;
      next->remainder.t2 = vremainder >>  8;
      next->remainder.t1 = vremainder >> 16;
      #endif

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

      #if 0
      labelv.label1.write_point = next->label1.write_point;
      labelv.label1.remainder   = next->label1.remainder;
      #endif

      lseek(f, (off_t) dstart_position, SEEK_SET);
      outputw(f, &labelv, DIRECTORY_BLOCK);
      lseek(f, (off_t) 0, SEEK_END);
   }
   else if (strcmp(command, "file")   == 0)
   {
      status = sscanf(data, "%s %s %s", command, argument, path);
      if (status < 2) printf("file name [input_path]\n");
      else
      {
         if (status < 3) strcpy(path, argument);
         f2 = open(path,     O_RDONLY, 0444);

         if (f2 < 0) printf("input file E %d\n", errno);
         else
         {
            if (flag['x'-'a']) output_label(f, argument, dstart_granule * 64
							+ *displacement);
            p64 = lseek(f2, (off_t) 0, SEEK_END);
            if ((long long) p64 < 0) printf("end E %d\n", errno);
            else
            {
               status = lseek(f2, (off_t) 0, SEEK_SET);

               if (status < 0) printf("start E %d\n", errno);
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

                  printf("slot1 %d banks %ld last granules %ld total %lld\n", slot,
                                                                              p32,
                                                                              slab,
                                                                              p64);

                  new->ex.next_offset = start_zero;
                  new->ex.next = restart_link;

                  if ((p32) || (slab))
                  {
                     vpointer = *displacement;
                     *displacement = vpointer + 7;

                     new->ex.next_offset.t3 = vpointer & 63;
                     vpointer >>= 6;
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
                     printf("%s extent %d granules\n", argument, PAGE/GRANULE);
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
                        vpointer = *displacement;
                        *displacement = vpointer + 7;

                        new->ex.next_offset.t3 = vpointer & 63;
                        vpointer >>= 6;
                        apointer = dstart_granule + vpointer;

                        new->ex.next.octet[5] = apointer;
                        new->ex.next.octet[4] = apointer >>  8;
                        new->ex.next.octet[3] = apointer >> 16;
                        new->ex.next.octet[2] = apointer >> 24;
                        new->ex.next.octet[1] = apointer >> 32;
                        new->ex.next.octet[0] = apointer >> 40;
                     }

                     gpointer += PAGE/GRANULE;
                     extra++;
                  }

                  if (slab)
                  {
                     printf("%s extend %ld granules\n", argument, slab);
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


/*******************************************************

	the job to do here is scan the specification
	text, measure and read the test files requested,
	and write the emulated file system image

*******************************************************/



int main(int argc, char *argv[])
{
   int			 status,
			 symbol;

   long long		 net_granules = GRANULES;
   unsigned char	*uptr;

   int                   byword;
   msw                   bypass = { 128, 0, 0 } ;
   msw                  *indexp = (msw *) &label1;


   argue(argc, argv);

   if (arguments)
   {
      #ifdef DOS
      f = open(argument[0], O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0777);
      #else
      f = open(argument[0], O_RDWR | O_CREAT | O_TRUNC, 0777);
      #endif

      if (f < 0) printf("file at argument 1 cannot be written %d\n", errno);
      else
      {
         status = outputw(f, &label1, 1024);
         if (status < 0) printf("write error %d\n", errno);

         for (;;)
         {
            if (interpret(&label1, &pointer1, 0, &label1.label1) == 0) break;
         }
      }

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
            else if (uflag['K'-'A']) net_granules <<=  4;
         }
      }

      net_granules -= gpointer;

      printf("%lld granules written, %lld free\n", gpointer, net_granules);

      label1.label3.ex.granules.t1 = net_granules >> 16;
      label1.label3.ex.granules.t2 = net_granules >>  8;
      label1.label3.ex.granules.t3 = net_granules;

      label1.label3.name[0].t1 = net_granules >> 40;
      label1.label3.name[0].t2 = net_granules >> 32;
      label1.label3.name[0].t3 = net_granules >> 24;

      remainder1 = 1024 - pointer1;

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

      label1.space.write_point.t1 = pointer1 >> 16;
      label1.space.write_point.t2 = pointer1 >>  8;
      label1.space.write_point.t3 = pointer1;

      label1.space.remainder.t1 = remainder1 >> 16;
      label1.space.remainder.t2 = remainder1 >>  8;
      label1.space.remainder.t3 = remainder1;

      label1.label3.ex.granule.octet[5] = gpointer;
      label1.label3.ex.granule.octet[4] = gpointer >>  8;
      label1.label3.ex.granule.octet[3] = gpointer >> 16;
      label1.label3.ex.granule.octet[2] = gpointer >> 24;
      label1.label3.ex.granule.octet[1] = gpointer >> 32;
      label1.label3.ex.granule.octet[0] = gpointer >> 40;

      lseek(f, (off_t) 0, SEEK_SET);
      status = outputw(f, &label1, 1024);
      if (status < 0) printf("write error %d\n", errno);      

      close (f);
   }
   else printf("an output filename for the file system image is required\n");

   return 0;
}
