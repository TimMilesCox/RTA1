/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is part of the memory-and-device-subsection
    of the software-emulation of the freeware processor architecture

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
#include "emulate.h"
#include "rw.h"
#include "ii.h"

#ifdef	LINUX
#ifndef	GCC
#include "../tgen.x64/_mnames.h"
#endif
#endif

extern system_memory		 memory;
#if 1
#else
extern device24			*f_s[];
extern device16			 i_f[];
#endif

device				 devices[64] = { { memory.p4k } } ;
//	{  NULL     } ,
//	{  NULL    }  } ;

extern int			 _register[256];
extern int			 base[128];
extern int			 psr;

static int   mread(word *w24p, int designator);
static void mwrite(word *w24p, int designator, int write_value);


/*****************************************************************************

        CHECK_ON_BASE is a switch used on RTA1 emulators
        which posits there is never an accidental value
	in an address space window base[] port
        which could cause an emulator crash
        because sabr and outA|B instructions prevent basing unavailable pages

                outA|B with internal GUARD interrupt, latent value AUTHORITY

                sabr by substituting invalid device page C00001
		which causes the GUARD interrupt on subsequent reference
		done like that because applications call ISRs
		to base peripheral array pages either rightly or wrongly

        and RTA1 target software image kernel routines default all windows
        to invalid device page C00001. That is one basis of memory privacy
        and accident protection. Applications also do not write all windows
	of the address space

        the range checks on each reference avoided by CHECK_ON_BASE are
        left visible in unswitched emulator source

        each reference still needs to check address offset 4096+
        where page is 4K of device zero = executable space

*****************************************************************************/



#ifdef	WINDOW_READ_RULE

/**************************************************************

	72 rules for referencing 64 windows

	1..7 may individually be substituted by 65..71
	per PSR flags see$1..7

	0, 2, 65..71 are writeable from interrupt code
	not application code

	65..71 rule flags are not used yet
	because their rule is always write-protect

	they are left here for any future fancying-up

	these rules are for address windows
	whatever storage block is addressed in them

	this table models wired-in characteristics

**************************************************************/

static unsigned char	 window_rule[72] = {	128, 64, 128 | 32, 16, 8, 4, 2, 1,
                                                      0, 0, 0, 0,    0, 0, 0, 0,
			0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,
			0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,
			0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,
			128, 128, 128, 128, 128,  128, 128, 128 } ;

#endif

/******************************************************************************
	gcc device_read() is in all emulators because debug console() calls it
	machine-code written emulator core has its own non-exported device_read
	but policy is to avoid calling assembler from gcc with arguments
	because a change in tools can mean a change in calling sequence
******************************************************************************/

/********************************************************
        device_read returns a data value unconditionally
        not a status ever
        caller [ operand_read burst_read2 burst_read4 ]
        proves the address before calling here
*******************************************************/


int device_read(int device_index, int relocation_base, unsigned offset, int designator)
{
    msw			*w24p;
    word16		*w16p;

    bank16		*bank16p;
    fsbank		*fsbankp;
    page			*pagep;

    int			 v;
    int			 device_type = base[128 + device_index] & 0x00C00000;
    device		*devicep = &devices[device_index];
   

    relocation_base &= 0x003FFFC0;


   if (device_type == DATA16_FLAG)
   {
      #ifndef CHECK_ON_BASE
      if (bank > base[128 + device_index] & 65535)
      {
         v = -1;
      }
      else
      #endif
      {
         relocation_base >>= 6;

         bank16p = devicep->dev16 + relocation_base;
         w16p = bank16p->w + offset;


         v = (w16p->left << 8)
           |  w16p->right;
      }
   }

   else if (device_type == FSYS24_FLAG)
   {
      #ifndef CHECK_ON_BASE
      if (bank > base[128 + device_index] & 65535)
      {
         v = -2;
      }
      else
      #endif
      {
         relocation_base >>= 6;

         fsbankp = devicep->dev24 + relocation_base;
         w24p = fsbankp->w + offset;

         v = (w24p->t1 << 16)
           | (w24p->t2 <<  8)
           |  w24p->t3;
      }
   }
   else if (device_type ==SYSMEM_FLAG)
   {
      /**************************************************
            system memory can also be accessed here
            so devices with system memory characteristics
            are configured like executable space
            with # 4K pages
      **************************************************/

      #ifndef CHECK_ON_BASE
      if (bank > (base[128 + device_index] >> 6) & 65535)
      {
         v = -3;
      }
      else
      #endif
      {
         pagep = devicep->pages + relocation_base;
         v = mread(pagep->w + offset, designator);
      }
   }
   else v = -4;

   return v;
}

/********************************************************
	mread returns a data value unconditionally
	not a status ever
	caller [ operand_read [device_read] ]
	proves the address before calling here
*******************************************************/

static int mread(word *w24p, int designator)
{
   int		 v;

   L24SL(v, *((int *) w24p));

   /******************************************
	c for each platform
	needs to do algebraic right shifts
	true of gcc and cl
   ******************************************/

   switch (designator)
   {
      case 0:
      case 7:
         break;				/*	word	*/
      case 1:
         v >>= 16;			/*	word.t1	*/
         break;
      case 2:
         if (psr & HALFWORD)
         {
            v >>= 12;			/*	word.h1	*/
            break;
         }

         v = (v << 8) >> 16;		/*	word.t2	*/
         break;
      case 3:
         if (psr & HALFWORD)
         {
            v = (v << 12) >> 12;	/*	word.h2	*/
            break;
         }

         v = (v << 16) >> 16;		/*	word.t3	*/
         break;
      default:
         break;
   }

   /**********************************
	leave 8 zeros at position 31
	signs from position 24
   **********************************/

   return (unsigned) v >> 8;
}

#ifdef  GCC

/***********************************************************************
	following are in emulators entirely written in gcc
***********************************************************************/


/***************************************************************
	memory_read() is called by instructions which take
	only a single word memory operand and never a register,
	notably the instructions

		execute
		repeat_execute

	and therefore has different rules from operand_read()

***************************************************************/


word memory_read(unsigned ea)
{
   static word	 outside_executable_space = { 0, II, 0, (1 << 6) | 31 } ;
//   static word	 nop			  = { 0, SK, 0, 128 + 1       } ;

   device	*devicep;
   int		 device_type;

   unsigned	 relocation_base;
   int		 index = ea >> 18;
   unsigned	 offset;
   int		 device_index;
   int		 tag;

   page		*pagep;
   word		*wordp;

   if (index)
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index  = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_READ_RULE

   if (psr & 0x7F00)
   {
      /******************************************
        it should be slightly quicker
        to test and skip if not needed
      ******************************************/

      WINDOW_READ_RULE(tag, index);
   }

   relocation_base = base[index];

   #else
   if ((index) && ((index & 0x38) == 0))
   {
      if ((32768 >> index) & psr) index |= 64;
   }

   relocation_base = base[index];
   #endif

   if (relocation_base & 0x00400000)
   {
      /**************************************************
        anywhere except a 4K-word page of executable space
      **************************************************/
      
      device_index = relocation_base & 63;

      if (device_index)
      {
         #ifdef WPROTECT

         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            #if 0
            not needed because only execute calls this
            and outside_executable_space is a constant
            of the instruction ii GUARD$

            {
               GUARD_INTERRUPT
            }
            #endif

            return outside_executable_space;
         }

         #endif

         devicep = devices + device_index;
         device_type = base[128 + device_index] & 0x00C00000;

         printf("[%x:%x]\n", device_index, device_type);
         if (device_type == SYSMEM_FLAG)
         {
            relocation_base &= 0x003FFFC0;

            /************************************************************
                devices with system memory characteristic
                are configured like executable space with # 4K pages
            ************************************************************/

            #ifndef CHECK_ON_BASE
            if (relocation_base < (base[128 + device_index] & 0x003FFFFF))
            {
            }
            else
            {
               #if 0
               if (psr & 0x00800000)
               {
                  return nop;
               }
               else
               #endif
               {
                  return outside_executable_space;
               }
            }
            else
            #endif
            {
               pagep = (page *) devicep->pages + relocation_base;
               wordp = pagep->w + offset;
               return *wordp;
            }
         }

         // if (psr & 0x00800000) return nop;
         return outside_executable_space;
      }

      /***********************************************************
		a 256K-word page of executable space
		skip 4K-check and read @ offset bits 18
      ***********************************************************/
   }
   #ifdef WPROTECT
   else
   {
      /**********************************************
		a 4K-word page of executable space
      **********************************************/

      if (offset & 0x0003F000)
      {
         // if (psr & 0x00800000) return nop;
         return outside_executable_space;
      }
   }
   #endif

   relocation_base &= 0x003FFFFF;

   pagep = memory.p4k + relocation_base;
   wordp = pagep->w + offset;

//   printf("[%p %p %p %p]\n", &memory, &memory.p4k[PAGES_IN_MEMORY], pagep, wordp);

   return *wordp;
}

#ifdef ABSOTS
               
/************************************************
   operand_read() and operand_write()
   don't look at the designator argument
   before classing EA in register range
   so switch ABSO uses * abso = memory_hold(ea)
   to get a pointer for TS instruction
   and that is one less call to operand_etc()

   of course TS target is never in range 0..255
************************************************/

word *memory_hold(unsigned ea)
{
   unsigned      relocation_base;
   unsigned	 alternate = 0;
   int           index = ea >> 18;
   unsigned	 offset;
   device	*devicep;
   int		 device_type;
   page		*pagep;
   int		 device_index;

   if (index)
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index  = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_WRITE_RULE

   WINDOW_WRITE_RULE(alternate, index)

   if (alternate & 128)
   {
      if (psr & 0x00800000)
      {
         /********************************************
		ISRs may write all spaces which exist
         ********************************************/
      }
      else
      {
         GUARD_INTERRUPT
         return NULL;
      }
   }

   #else
   #error YOU ARE NOT HERE
   if (index & 0x38)
   {
   }
   else
   {
      if ((index == 0)
      ||  (alternate = ((32768 >> index) & psr))
      ||  (index == 2))
      {
         if (psr & 0x00800000)
         {
            /************************************
               interrupt code may lock
               anywhere in executable space
               for update
            ************************************/
         }
         else
         {
            GUARD_INTERRUPT
            return NULL;
         }
      }

      if (alternate) index |= 64;
   }

   #endif

   relocation_base = base[index];

   if (relocation_base & 0x00400000)
   {
      /**************************************************
	anywhere except a 4K-word page of executable space
      **************************************************/

      if ((device_index = relocation_base & 63))
      {
         #ifdef WPROTECT

         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            GUARD_INTERRUPT

            return NULL;
         }

         #endif
         devicep = devices + device_index;
         device_type = base[128 + device_index] & 0x00C00000;

         if (device_type == SYSMEM_FLAG)
         {
            relocation_base &= 0x003FFFC0;

            /************************************************************
		devices with system memory characteristic
                are configured as executable space with # 4K pages
            ************************************************************/

            #ifndef CHECK_ON_BASE
            #error YOU ARE NOT HERE EITHER
            if (relocation_base > (base[128 + device_index] >> 6) & 65535)
            {
               if (psr & 0x00800000)
               {
               }
               else
               {
                  GUARD_INTERRUPT
               }
            }
            else
            #endif
            pagep = devicep->pages + relocation_base;
            return pagep->w + offset;
         }

         /********************************************
            device arrays similar to system memory
            may also be held for update
            other device arrays can't
         *********************************************/

         GUARD_INTERRUPT

	 return NULL;
      }

      /***********************************************************
                a 256K-word page of executable space
                skip 4K-check and lock @ offset bits 18
      ***********************************************************/
   }
   #ifdef WPROTECT
   else
   {
      /**********************************************
                a 4K-word page of executable space
      **********************************************/

      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095
            in 4096-word blocks
         ********************************************/

         GUARD_INTERRUPT

         return NULL;
      }
   }
   #endif

   relocation_base &= 0x003FFFFF;

   if (relocation_base < base[124])
   {
      /******************************************************
         no writes inside the fixed system image
         defined by the RAM boundary port
      ******************************************************/

      GUARD_AUTHORITY
      return NULL;
   }

   pagep = memory.p4k + relocation_base;
   return pagep->w + offset;
}

#endif		/*	ABSOTS	*/


#define	OPERAND_READ1(EA) operand_read(EA, 7)

int operand_read(unsigned ea, int designator)
{
   unsigned		 index, relocation_base;
   int			 v;
   unsigned		 offset;

   page			*pagep;
   word			*w24p;

   int			 device_index;

   if (designator ==  I) return ea;
   if (designator == XI) return ea;

   if (ea < 256) return _register[ea];

   if ((index = ea >> 18))
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_READ_RULE

   if (psr & 0x7F00)
   {
      /******************************************
        it should be slightly quicker
        to test and skip if not needed
      ******************************************/

      WINDOW_READ_RULE(v, index);
   }

   relocation_base = base[index];

   #elif defined(WPROTECT)

   /********************************************************
      this is not intended as a logic change
      but aims to be more efficient
      by shifting a flag bit and then ORing psr onto it
      and only fetching relocation_base once
   ********************************************************/

   if (index & 0x38)
   {
   }
   else if (index)
   {
      if ((32768 >> index) & psr) index |= 64;
   }

   relocation_base = base[index];

   #else

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if (psr & (32768 >> index)) relocation_base = base[index + 64];
   }

   #endif

   if (relocation_base & 0x00400000)
   {
      /**************************************************
        anywhere except a 4K-word page of executable space
      **************************************************/
      
      if ((device_index = relocation_base & 63))
      {
         #ifdef WPROTECT
         
         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            GUARD_INTERRUPT

            return -LP_DEVICE;
         }

         #endif

         return device_read(device_index, relocation_base, offset, designator);
      }

      /***********************************************************
                a 256K-word page of executable space
                skip 4K-check and read @ offset bits 18
      ***********************************************************/
   }
   #ifdef WPROTECT
   else
   {
      /**********************************************
                a 4K-word page of executable space
      **********************************************/

      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         GUARD_INTERRUPT

         return -LP_ADDRESS;
      }
   }
   #endif

   #ifndef CHECK_ON_BASE
   if (offset > (WORDS_IN_MEMORY-1))
   {
      #if 0
      if (psr & 0x00800000)
      {
      }
      else
      #endif
      {
         GUARD_INTERRUPT
      }
      return -LP_ADDRESS;
   }
   #endif

   relocation_base &=0x003FFFFF;
   pagep = memory.p4k + relocation_base;
   return mread(pagep->w + offset, designator);
}

int burst_read2(int *list, unsigned ea)
{
   unsigned		 index, relocation_base, device_index;
   unsigned		 offset;

   int			 tag;

   page			*pagep;
   int			*w24p;
   int			 temp;

   if (ea < 256)
   {
      *list       = _register[ea];
      *(list + 1) = _register[ea + 1];
      return 0;
   }
 
   if ((index = ea >> 18))
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_READ_RULE

   if (psr & 0x7F00)
   {
      /******************************************
        it should be slightly quicker
        to test and skip if not needed
      ******************************************/

      WINDOW_READ_RULE(tag, index);
   }

   relocation_base = base[index];

   #elif defined(WPROTECT)

   /********************************************************
      this is not intended as a logic change 
      but aims to be more efficient
      by shifting a flag bit and then ORing psr onto it
      and only fetching relocation_base once
   ********************************************************/

   if (index & 0x38)
   {
   }
   else if (index)
   {
      if ((32768 >> index) & psr) index |= 64;
   }

   relocation_base = base[index];

   #else

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   #endif

   if (relocation_base & 0x00400000)
   {
      /**************************************************
        anywhere except a 4K-word page of executable space
      **************************************************/
      
      #ifdef BANK_EDGE_GUARD
      if (offset == 0x0003FFFF)
      {
         if (base[index + 1] ^ (relocation_base + 64))
         {
            #ifdef WAYPATH
            printf("[a]\n");
            #endif
      
            if ((temp = OPERAND_READ1(ea)) < 0) return temp;
            *list++ = temp;
            if ((temp = OPERAND_READ1(ea + 1)) < 0) return temp;;
            *list = temp;
            return 0;
         }
      }
      #endif

      if ((device_index = relocation_base & 63))
      {
         #ifdef WPROTECT

         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            GUARD_INTERRUPT

            return -LP_DEVICE;
         }

         #endif

         if ((temp = device_read(device_index, relocation_base, offset, 7)) < 0) return temp;
         *list = temp;
         if ((temp = device_read(device_index, relocation_base, offset + 1, 7)) < 0) return temp;
         *(list + 1) = temp;
         return 0;
      }

      /***********************************************************
                a 256K-word page of executable space
                skip 4K-check and read @ offset bits 18
      ***********************************************************/
   }
   #ifdef WPROTECT
   else
   {
      /**********************************************
                a 4K-word page of executable space
      **********************************************/

      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         GUARD_INTERRUPT

         return -LP_ADDRESS;
      }
      #ifdef BANK_EDGE_GUARD
      else
      {
         if (offset == 0x00000FFF)
         {
            if (base[index + 1] ^ (relocation_base + 1))
            {
               #ifdef WAYPATH
               printf("[b]\n");
               #endif
      
               if ((temp = OPERAND_READ1(ea)) < 0) return temp;
               *list++ = temp;
               if ((temp = OPERAND_READ1(ea + 1)) < 0) return temp;
               *list = temp;
               return 0;
            }
         }
      }
      #endif
   }
   #endif

   relocation_base &= 0x003FFFFF;
   pagep = memory.p4k + relocation_base;
   w24p = pagep->i + offset;
   LOAD24(*list, *w24p);
   LOAD24(*(list + 1), *(w24p + 1));
   return 0;
}

#ifdef	BANK_EDGE_GUARD
static int straddle_read4(unsigned offset, int *list, unsigned ea)
{
   int		 sense;

   if ((sense = burst_read2(list, ea)) < 0) return sense;
   if ((sense = burst_read2(list + 2, ea + 2)) < 0) return sense;
   return 0;
}
#endif

int burst_read4(int *list, unsigned ea)
{
   unsigned		 device_index, index, relocation_base;
   unsigned		 offset;

   int			 tag;

   page			*pagep;
   int			*w24p;

   int			 temp;

   if (ea < 256)
   {
      *list       = _register[ea];
      *(list + 1) = _register[ea + 1];
      *(list + 2) = _register[ea + 2];
      *(list + 3) = _register[ea + 3];
      return 0;
   }

   if ((index = ea >> 18))
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_READ_RULE

   if (psr & 0x7F00)
   {
      /******************************************
        it should be slightly quicker
        to test and skip if not needed
      ******************************************/

      WINDOW_READ_RULE(tag, index);
   }

   relocation_base = base[index];

   #elif defined(WPROTECT)

   /********************************************************
      this is not intended as a logic change
      but aims to be more efficient
      by shifting a flag bit and then ORing psr onto it
      and only fetching relocation_base once
   ********************************************************/
   
   if (index & 0x38)
   {
   }
   else if (index)
   {  
      if ((32768 >> index) & psr) index |= 64;
   }
         
   relocation_base = base[index];

   #else

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   #endif


   if (relocation_base & 0x00400000)
   {
      /**************************************************
        anywhere except a 4K-word page of executable space
      **************************************************/
      
      #ifdef BANK_EDGE_GUARD
      if (offset > 0x0003FFFC)
      {
         if (base[index + 1] ^ (relocation_base + 64))
         {
            #ifdef WAYPATH
            printf("[e]\n");
            #endif
      
            straddle_read4(offset, list, ea);
            return 0;
         }
      }
      #endif

      if ((device_index = relocation_base & 63))
      {
         #ifdef WPROTECT

         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            GUARD_INTERRUPT

            return -LP_DEVICE;
         }

         #endif

         if ((temp = device_read(device_index, relocation_base, offset, 7)) < 0) return temp;
         *list = temp;
         if ((temp = device_read(device_index, relocation_base, offset + 1, 7)) < 0) return temp;
         *(list + 1) = temp;
         if ((temp = device_read(device_index, relocation_base, offset + 2, 7)) < 0) return temp;
         *(list + 2) = temp;
         if ((temp = device_read(device_index, relocation_base, offset + 3, 7)) < 0) return temp;
         *(list + 3) = temp;
         return 0;
      }

      /***********************************************************
                a 256K-word page of executable space
                skip 4K-check and read @ offset bits 18
      ***********************************************************/
   }
   #ifdef WPROTECT
   else
   {
      /**********************************************
                a 4K-word page of executable space
      **********************************************/

      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         GUARD_INTERRUPT
         return -LP_ADDRESS;
      }
      #ifdef BANK_EDGE_GUARD
      else
      {
         if (offset > 0x00000FFC)
         {
            if (base[index + 1] ^ (relocation_base + 1))
            {
               #ifdef WAYPATH
               printf("[f]\n");
               #endif
      
               return straddle_read4(offset, list, ea);
            }
         }
      }
      #endif
   }
   #endif

   relocation_base &= 0x003FFFFF;
   pagep = memory.p4k + relocation_base;
   w24p = pagep->i + offset;
   LOAD24(*list, *w24p);
   LOAD24(*(list + 1), *(w24p + 1));
   LOAD24(*(list + 2), *(w24p + 2));
   LOAD24(*(list + 3), *(w24p + 3));
   return 0;
}


static void device_write(int v, int device_index, int relocation_base, unsigned offset, int designator)
{
   msw			*w24p;
   word16		*w16p;

   page			*pagep;
   fsbank		*fsbankp;
   bank16		*bank16p;

   device               *devicep = &devices[device_index];
   int			 device_type = base[128 + device_index] & 0x00C00000;


   relocation_base &= 0x003FFFC0;

   if (device_type == DATA16_FLAG)
   {
       #ifndef CHECK_ON_BASE
       if (bank > base[128 + device_index] & 65535)
       {
       }
       else
       #endif
       {
          relocation_base >>= 6;

          bank16p = devicep->dev16 + relocation_base;
          w16p = bank16p->w + offset;

          w16p->right = v;
          w16p->left  = v >> 8;
       }
    }
    else if (device_type == FSYS24_FLAG)
    {
       #ifndef CHECK_ON_BASE
       if (bank > base[128 + device_index] & 65535)
       {
       }
       else
       #endif
       {
          relocation_base >>= 6;

          fsbankp = devicep->dev24 + relocation_base;            
          w24p = fsbankp->w + offset;

          w24p->t3 = v;
          w24p->t2 = v >> 8;
          w24p->t1 = v >> 16;
       }
    }
    else if (device_type == SYSMEM_FLAG)
    {
        /**************************************************
            devices of system memory character are configured
            like executable space with # 4K pages because
            because that's how executable space is
            in 256K blocks

            a page pointer is supplied and gets pointed to
            a 256K bank edge
        **************************************************/

        #ifndef CHECK_ON_BASE
        if (bank > (base[128 + device_index] >> 6) & 65535)
        {
        }
        else
        #endif
        {
           pagep = devicep->pages + relocation_base;
           mwrite(pagep->w + offset, designator, v);
        }
    }
}

#define OPERAND_WRITE1(DATA, EA) operand_write(DATA, EA, 7)

int operand_write(int v, unsigned ea, int designator)
{
   unsigned		 index, relocation_base, device_index;
   unsigned		 alternate = 0;
   unsigned		 offset;


   word			*w24p;
   page			*pagep;

   if (ea < 256)
   {
      if (psr & 0x00800000)
      {
         /**********************************************
            interrupt code may write all registers
         **********************************************/
      }
      else
      {
         if (ea & 128)
         {
            /*******************************************
               application code may not
               write registers at 128+
            *******************************************/

            GUARD_IIX(1)
            return -LP_AUTHORITY;
         }
      }

      _register[ea] = v;
      return 0;
   }

   if ((index = ea >> 18))
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = ea >> 12;
      offset = ea & 0x00000FFF;
   }


   #ifdef WINDOW_WRITE_RULE

   WINDOW_WRITE_RULE(alternate, index);

   if (alternate & 128)
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_IIX(2)
         return -LP_AUTHORITY;
      }
   }

   relocation_base = base[index];

   #elif defined(WPROTECT)
   #error YOU ARE NEVER HERE
   if (index & 0x38)
   {
   }
   else
   {
      /***************************************************
	only interrupt code may write

		B0 instruction window
		B2 vector window
		B65 thread control block
		B66..71 kernel data

	the compiler must conform to known expectations
	about resolution order of compound conditions
	and implementors in platform assembler must program:

	first:	if B0 jump to test for interrupt code

        second:	produce alternate as unconditional side-effect
		and if alternate jump to test for interrupt code

	third:	if B2 jump to test for interrupt code
      ***************************************************/

      if ((index == 0)
      ||  (alternate = ((32768 >> index) & psr))
      ||  (index == 2))
      {
         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_IIX(3)
            return -LP_AUTHORITY;
         }
      }

      if (alternate) index |= 64;
   }

   relocation_base = base[index];

   #else
   #error NEVER
   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   #endif

   if (relocation_base & 0x00400000)
   {
      /**************************************************
        anywhere except a 4K-word page of executable space
      **************************************************/
      
      if ((device_index = relocation_base & 63))
      {
         #ifdef WPROTECT
         
         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory write target nowhere is a non-device
               don't write to nowhere whoever you are
            *********************************************/

            GUARD_IIX(4)
            return -LP_DEVICE;
         }

         #endif

         device_write(v, device_index, relocation_base, offset, designator);
         return 0;
      }

      /***********************************************************
                a 256K-word page of executable space
                skip 4K-check and write @ offset bits 18
      ***********************************************************/
   }
   #ifdef WPROTECT
   else
   {
      /**********************************************
                a 4K-word page of executable space
      **********************************************/

      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         GUARD_IIX(5)
         return -LP_ADDRESS;
      }
   }
   #endif

   #ifndef CHECK_ON_BASE
   if (offset > (WORDS_IN_MEMORY-1))
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_IIX(6);
         return -LP_ADDRESS;
      }

      return -LP_ADDRESS;
   }
   #endif

   relocation_base &= 0x003FFFFF;

   if (relocation_base < base[124])
   {
      /******************************************************
         no writes inside the fixed system image
         defined by the RAM boundary port
      ******************************************************/

      GUARD_AUTHORITY
      return -LP_AUTHORITY;
   }

   pagep = memory.p4k + relocation_base;
   mwrite(pagep->w + offset, designator, v);
   return 0;
}

static void mwrite(word *w24p, int designator, int v)
{
   switch (designator)
   {
      case 0:
      case 7:
	 ORDER32(*((int *) w24p), v);
         break;
      case 1:
         w24p->t1 = v;
         break;
      case 2:
         if (psr & HALFWORD)
         {
            w24p->t2 = (w24p->t2 & 15)
                     | (v      << 4);
            w24p->t1 = (v      >> 4);
         }
         else w24p->t2 = v;
         break;
      case 3:
         if (psr & HALFWORD)
         {
            w24p->t3 = v;
            w24p->t2 =  (w24p->t2  & 240)
                     | ((v >> 8)   &  15);
         }
         else w24p->t3 = v;
         break;
   }
}

int burst_write2(int *list, unsigned ea)
{
   unsigned		 device_index, index, relocation_base;
   unsigned		 alternate = 0;
   unsigned		 offset;

   page			*pagep;
   int			*w24p;

   int			 sense;

   int			 v = *list,
			 w = *(list + 1);

   if (ea < 256)
   {
      if (psr & 0x00800000)
      {
         /**********************************************
            interrupt code may write all registers
            including a few spares at 256+
         **********************************************/
      }
      else
      {
         if (ea > 126)
         {
            /*******************************************
               application code may not
               write registers at 128+
            *******************************************/
            
            GUARD_IIX(8)
            return -LP_AUTHORITY;
         }
      }

      _register[ea]     = v;
      _register[ea + 1] = w;
      return 0;
   }

   if ((index = ea >> 18))
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_WRITE_RULE
   WINDOW_WRITE_RULE(alternate, index);

   if (alternate & 128)
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_IIX(9)
         return -LP_AUTHORITY;
      }
   }

   relocation_base = base[index];

   #elif defined(WPROTECT)

   if (index & 0x38)
   {
   }
   else
   {
      /***************************************************
        only interrupt code may write

                B0 instruction window
                B2 vector window
                B65 thread control block
                B66..71 kernel data

        the compiler must conform to known expectations
        about resolution order of compound conditions
        and implementors in platform assembler must program:

        first:  if B0 jump to test for interrupt code

        second: produce alternate as unconditional side-effect
                and if alternate jump to test for interrupt code

        third:  if B2 jump to test for interrupt code
      ***************************************************/

      if ((index == 0)
      ||  (alternate = ((32768 >> index) & psr))
      ||  (index == 2))
      {
         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_IIX(10)
            return -LP_AUTHORITY;
         }
      }

      if (alternate) index |= 64;
   }

   relocation_base = base[index];

   #else
   #error NEVER NEVER NEVER
   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   #endif

   if (relocation_base & 0x00400000)
   {
      /**************************************************
        anywhere except a 4K-word page of executable space
      **************************************************/
      
      #ifdef BANK_EDGE_GUARD
      if (offset == 0x0003FFFF)
      {
         if (base[index + 1] ^ (relocation_base + 64))
         {
            /****************************************************
               1st word of 2 is last of block
               next block is not in next window
               resolve 2 addresses if possible. Blow up if not
            ****************************************************/

            #ifdef WAYPATH
            printf("[g]\n");
            #endif

            if ((sense = OPERAND_WRITE1(v, ea)) < 0) return sense;
            if ((sense = OPERAND_WRITE1(w, ea + 1)) < 0) return sense;
            return 0;
         }
      }
      #endif

      if ((device_index = relocation_base & 63))
      {
         #ifdef WPROTECT
         
         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory write target nowhere is a non-device
               don't write to nowhere whoever you are
            *********************************************/

            GUARD_IIX(11)
            return -LP_DEVICE;
         }

         #endif

         device_write(v, device_index, relocation_base, offset, 7);
         device_write(w, device_index, relocation_base, offset + 1, 7);
         return 0;

         /********************************************************
		device capacity is already checked
         ********************************************************/
      }

      /***********************************************************
                a 256K-word page of executable space
                skip 4K-check and write @ offset bits 18
      ***********************************************************/
   }
   #ifdef WPROTECT
   else
   {
      /**********************************************
                a 4K-word page of executable space
      **********************************************/

      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         GUARD_IIX(12)
         return -LP_ADDRESS;
      }
      
      #ifdef BANK_EDGE_GUARD
      else
      {
         if (offset == 0x00000FFF)
         {
            if (base[index + 1] ^ (relocation_base + 1))
            {
               /***********************************************
                  2-word write to last word in block
                  and next block not in next window
                  resolve 2 addresses if you can. Or blow up
               ***********************************************/

               #ifdef WAYPATH
               printf("[h]\n");
               #endif

               if ((sense = OPERAND_WRITE1(v, ea) < 0)) return sense;
               if ((sense = OPERAND_WRITE1(w, ea + 1)) < 0) return sense;
               return 0;
            }
         }
      }
      #endif
   }
   #endif


   relocation_base &= 0x003FFFFF;

   if (relocation_base < base[124])
   {
      /******************************************************
         no writes inside the fixed system image
         defined by the RAM boundary port
      ******************************************************/

      GUARD_AUTHORITY
      return -LP_AUTHORITY;
   }

   pagep = memory.p4k + relocation_base;
   w24p = pagep->i + offset;
   ORDER32(*w24p, v);
   ORDER32(*(w24p + 1), w);
   return 0;
}

#ifdef BANK_EDGE_GUARD
static int straddle_write4(unsigned offset, int *list, unsigned ea)
{
   int		 sense;

   if ((sense = burst_write2(list, ea)) < 0) return sense;
   if ((sense = burst_write2(list + 2, ea + 2)) < 0) return sense;
   return 0;
}
#endif

int burst_write4(int *list, unsigned ea)
{
   unsigned		 device_index, index, relocation_base;
   unsigned		 alternate = 0;
   unsigned 		 offset;

   page			*pagep;
   int			*w24p;

   int			 v = *list,
			 w  = *(list + 1),
			 _u = *(list + 2),
			 _l = *(list + 3);			

   if (ea < 256)
   {
      if (psr & 0x00800000)
      {
         /**********************************************
            interrupt code may write all registers
            including a few spares at 256+
         **********************************************/
      }
      else
      {
         if (ea > 124)
         {
            /*******************************************
               application code may not
               write registers at 128+
            *******************************************/
    
            GUARD_IIX(13)
            return -LP_AUTHORITY;
         }
      }

      _register[ea]     = v;
      _register[ea + 1] = w;
      _register[ea + 2] = _u;
      _register[ea + 3] = _l;
      return 0;
   }

   if ((index = ea >> 18))
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_WRITE_RULE
   WINDOW_WRITE_RULE(alternate, index);

   if (alternate & 128)
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT
         return -LP_AUTHORITY;
      }
   }

   relocation_base = base[index];

   #elif defined(WPROTECT)
   #error NOT EVER
   if (index & 0x38)
   {
   }
   else
   {
      /***************************************************
        only interrupt code may write

                B0 instruction window
                B2 vector window
                B65 thread control block
                B66..71 kernel data

        the compiler must conform to known expectations
        about resolution order of compound conditions
        and implementors in platform assembler must program:

        first:  if B0 jump to test for interrupt code

        second: produce alternate as unconditional side-effect
                and if alternate jump to test for interrupt code

        third:  if B2 jump to test for interrupt code
      ***************************************************/

      if ((index == 0)
      ||  (alternate = ((32768 >> index) & psr))
      ||  (index == 2))
      {
         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_INTERRUPT
            return;
         }
      }

      if (alternate) index |= 64;
   }

   relocation_base = base[index];

   #else
   #error AND AGAIN NO
   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   #endif

   if (relocation_base & 0x00400000)
   {
      /**************************************************
        anywhere except a 4K-word page of executable space
      **************************************************/
      
      #ifdef BANK_EDGE_GUARD
      if (offset > 0x0003FFFC)
      {
         if(base[index + 1] ^ (relocation_base + 64))
         {
            /************************************************
               4-word write starting in last 3 words of block
               and next block is not in next window
               resolve addresses in 2 blocks. Or blow up
            ************************************************/

            #ifdef WAYPATH
            printf("[k]\n");
            #endif
      
            return straddle_write4(offset, list, ea);
         }
      }
      #endif

      if ((device_index = relocation_base & 63))
      {
         #ifdef WPROTECT
         
         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory write target nowhere is a non-device
               don't write to nowhere whoever you are
            *********************************************/

            GUARD_INTERRUPT
            return -LP_DEVICE;
         }

         #endif

         device_write(v,  device_index, relocation_base, offset, 7);
         device_write(w,  device_index, relocation_base, offset + 1, 7);
         device_write(_u, device_index, relocation_base, offset + 2, 7);
         device_write(_l, device_index, relocation_base, offset + 3, 7);
         return 0;
      }

      /***********************************************************
                a 256K-word page of executable space
                skip 4K-check and write @ offset bits 18
      ***********************************************************/
   }
   #ifdef WPROTECT
   else
   {
      /**********************************************
                a 4K-word page of executable space
      **********************************************/

      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         GUARD_INTERRUPT
         return -LP_ADDRESS;
      }

      #ifdef BANK_EDGE_GUARD
      else
      {
         if (offset > 0x0000FFC)
         {
            if (base[index + 1] ^ (relocation_base + 1))
            {
               /************************************************
                  4-word write starting in last 3 words of block
                  and next block is not in next window
                  resolve addresses in 2 blocks. Or blow up
               ************************************************/
            
               #ifdef WAYPATH
               printf("[l]\n");
               #endif
      
               return straddle_write4(offset, list, ea);
            }
         }
      }
      #endif
   }
   #endif

   relocation_base &= 0x003FFFFF;

   if (relocation_base < base[124])
   {
      /******************************************************
         no writes inside the fixed system image
         defined by the RAM boundary port
      ******************************************************/

      GUARD_AUTHORITY
      return -LP_AUTHORITY;
   }

   pagep = memory.p4k + relocation_base;
   w24p = pagep->i + offset;
   ORDER32(*w24p, v);
   ORDER32(*(w24p + 1), w);
   ORDER32(*(w24p + 2), _u);
   ORDER32(*(w24p + 3), _l);
   return 0;
}

#endif	/*	GCC	*/
