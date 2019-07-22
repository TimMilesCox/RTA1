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

extern system_memory		 memory;
#if 1
#else
extern device24			*f_s[];
extern device16			 i_f[];
#endif

device				 devices[64] = 
	{ { DEVICE | SYSMEM,  BANKS_IN_DEVICE,   { &memory } } ,
	  { DEVICE | FSYS24,  BANKS_IN_DEVICE,   { NULL    } } ,
	  { DEVICE | DATA16,  BANKS_IN_DEVICE16, { NULL    } } } ;

extern int			 _register[256];
extern int			 base[128];
extern int			 psr;

static int   mread(word *w24p, int designator);
static void mwrite(word *w24p, int designator, int write_value);


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

int device_read(int device_index, int relocation_base, unsigned offset, int designator)
{
   int			*iarayp;
   msw			*w24p;
   word16		*w16p;

   int			 v;

   device		*devicep = &devices[device_index];
   int			 bank = (relocation_base >> 6) & 65535;
   unsigned long	 absolute = (unsigned long) (bank << 18) | offset;

   if (devicep->flags & DEVICE)
   {
      if (devicep->flags & DATA16)
      {
         #ifndef CHECK_ON_BASE
         if (bank > base[128 + device_index] & 65535)
         {
            v = 0x00FFFFFF;
         }
         else
         #endif
         {
            w16p = devicep->s.dev16->array + absolute;

            v = (w16p->left << 8)
              |  w16p->right;
         }
      }
      else if (devicep->flags & FSYS24)
      {
         #ifndef CHECK_ON_BASE
         if (bank > base[128 + device_index] & 65535)
         {
            v = 0x00FFFFFF;
         }
         else
         #endif
         {
            w24p = devicep->s.dev24->array + absolute;

            v = (w24p->t1 << 16)
              | (w24p->t2 <<  8)
              |  w24p->t3;
         }
      }
      else if (devicep->flags & SYSMEM)
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
            v = 0x00FFFFFF;
         }
         else
         #endif
         {
            v = mread(devicep->s.pages->array + absolute, designator);
//            LOAD24(v, devicep->s.pages->iaray[absolute]);
         }
      }
      else v = 0x00A5A5A5;
   }
   else v = 0x005A5A5A;

   return v;
}


/***************************************************************
   memory_read() is called by instructions which take
   only a single word memory operand and never a register,
   notably the instructions

	execute

   and therefore has different rules from operand_read()

   memory_read() is also callable from emulator debug console

***************************************************************/


word memory_read(unsigned ea)
{
   static word	 outside_executable_space = { 0, II, 0, (1 << 6) | 31 } ;
   static word	 nop                      = { 0, LK, 0, 128 + 1 } ;

   device	*devicep;

   unsigned	 relocation_base;
   unsigned long absolute;
   int		 index = ea >> 18;
   int		 device_index;
   int		 tag;

   if (index)
   {
      absolute = ea & 0x0003FFFF;
   }
   else
   {
      index  = ea >> 12;
      absolute = ea & 0x00000FFF;
   }

   #ifdef WINDOW_READ_RULE

   WINDOW_READ_RULE(tag, index);
   relocation_base = base[index];

   #else
   if ((index) && ((index & 0x38) == 0))
   {
      if ((32768 >> index) & psr) index |= 64;
   }
   #endif

   relocation_base = base[index];

   #if 0
   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index | 64];
   }
   #endif

   if (relocation_base & 0x00400000)
   {
      if (relocation_base & 63)
      {
         #ifdef WPROTECT

         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            if (psr & 0x00800000)
            {
            }
            else
            {
               GUARD_INTERRUPT
            }

            return;
         }

         #endif
         devicep = devices + device_index;
         if (devicep->flags == DEVICE | SYSMEM)
         {
            relocation_base >>= 6;
            relocation_base &= 65535;

            /************************************************************
                devices with system memory characteristic
                are configured as executable space with # 4K pages
            ************************************************************/

            #ifndef CHECK_ON_BASE
            if (relocation_base > (base[128 + device_index] >> 6) & 65535)
            {
               if (psr & 0x00800000)
               {
                  return nop;
               }
               else
               {
                  return outside_executable_space;
               }
            }
            else
            #endif
            {
               absolute |= relocation_base << 18;
               return devicep->s.pages->array[absolute];
            }
         }

         if (psr & 0x00800000) return nop;
         return outside_executable_space;
      }
   }
   #ifdef WPROTECT
   else
   {
      if (absolute & 0x0003F000)
      {
         if (psr & 0x00800000) return nop;
         return outside_executable_space;
      }
   }
   #endif

   absolute |= (unsigned long) (relocation_base & 0x003FFFFF) << 12;

   return memory.array[absolute];
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
   unsigned long absolute;
   device	*devicep;
   int		 device_index;

   if (index)
   {
      absolute = ea & 0x0003FFFF;
   }
   else
   {
      index  = ea >> 12;
      absolute = ea & 0x00000FFF;
   }

   #ifdef WINDOW_WRITE_RULE

   WINDOW_WRITE_RULE(alternate, index)

   if (alternate & 128)
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT
         return NULL;
      }
   }

   #else

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

   #if 0
   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index | 64];
   }
   #endif

   if (relocation_base & 0x00400000)
   {
      if (device_index = relocation_base & 63)
      {
         #ifdef WPROTECT

         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            if (psr & 0x00800000)
            {
            }
            else
            {
               GUARD_INTERRUPT
            }

            return;
         }

         #endif
         devicep = devices + device_index;
         if (devicep->flags == DEVICE | SYSMEM)
         {
            relocation_base >>= 6;
            relocation_base &= 65535;

            /************************************************************
		devices with system memory characteristic
                are configured as executable space with # 4K pages
            ************************************************************/

            #ifndef CHECK_ON_BASE
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
            {
               absolute |= (unsigned long) relocation_base << 18;
               return devicep->s.pages->array + absolute;
            }
         }

         /********************************************
            device arrays similar to system memory
            may also be held for update
            other device arrays can't
         *********************************************/

         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_INTERRUPT
         }

	 return NULL;
      }
   }
   #ifdef WPROTECT
   else
   {
      if (absolute & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095
            in 4096-word blocks
         ********************************************/

         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_INTERRUPT
         }

         return NULL;
      }
   }
   #endif

   absolute |= (relocation_base & 0x003FFFFF) << 12;

   return memory.array + absolute;
}

#endif		/*	ABSOTS	*/


#define	OPERAND_READ1(EA) operand_read(EA, 7)

unsigned int operand_read(unsigned ea, int designator)
{
   unsigned		 index, relocation_base;
   int			 v;
   unsigned		 offset;

   unsigned long	 absolute;

   word			*w24p;
   int			 device_index;

   if (designator ==  I) return ea;
   if (designator == XI) return ea;

   if (ea < 256) return _register[ea];

   if (index = ea >> 18)
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_READ_RULE

   WINDOW_READ_RULE(v, index);
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
      if (device_index = relocation_base & 63)
      {
         #ifdef WPROTECT
         
         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            if (psr & 0x00800000)
            {
            }
            else
            {
               GUARD_INTERRUPT
            }

            return;
         }

         #endif

         return device_read(device_index, relocation_base, offset, designator);
      }
   }
   #ifdef WPROTECT
   else
   {
      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_INTERRUPT
         }

         return;
      }
   }
   #endif

   absolute = (unsigned long) ((relocation_base & 0x003FFFFF) << 12) | offset;

   #ifndef CHECK_ON_BASE
   if (offset > (WORDS_IN_MEMORY-1))
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT
      }
      return 0x00FFFAAA;
   }
   #endif

   return mread(memory.array + absolute, designator);
}

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

void burst_read2(int *list, unsigned ea)
{
   unsigned		 index, relocation_base, device_index;
   unsigned		 offset;
   unsigned long	 absolute;
   int			 tag;

   int			*w24p;

   if (ea < 256)
   {
      *list       = _register[ea];
      *(list + 1) = _register[ea + 1];
      return;
   }
 
   if (index = ea >> 18)
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_READ_RULE

   WINDOW_READ_RULE(tag, index);
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
      #ifdef BANK_EDGE_GUARD
      if (offset == 0x0003FFFF)
      {
         if (base[index + 1] ^ (relocation_base + 64))
         {
            #ifdef WAYPATH
            printf("[a]\n");
            #endif
      
            *list++ = OPERAND_READ1(ea);
            *list   = OPERAND_READ1(ea + 1);
            return;
         }
      }
      #endif

      if (device_index = relocation_base & 63)
      {
         #ifdef WPROTECT

         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            if (psr & 0x00800000)
            {
            }
            else
            {
               GUARD_INTERRUPT
            }

            return;
         }

         #endif

         *list       = device_read(device_index, relocation_base, offset, 7);
         *(list + 1) = device_read(device_index, relocation_base, offset + 1, 7);
         return;
      }
   }
   #ifdef WPROTECT
   else
   {
      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_INTERRUPT
         }

         return;
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
      
               *list++ = OPERAND_READ1(ea);
               *list   = OPERAND_READ1(ea + 1);
               return;
            }
         }
      }
      #endif
   }
   #endif

   absolute = (unsigned long) ((relocation_base & 0x003FFFFF) << 12) | offset;

   w24p = &memory.iaray[absolute];
   LOAD24(*list, *w24p);
   LOAD24(*(list + 1), *(w24p + 1));
}

#ifdef	BANK_EDGE_GUARD
static void straddle_read4(unsigned offset, int *list, unsigned ea)
{
   if ((offset & 3) == 2)
   {
      #ifdef WAYPATH
      printf("[c]\n");
      #endif
      
      burst_read2(list, ea);
      burst_read2(list + 2, ea + 2);
      return;
   }

   #ifdef WAYPATH
   printf("[d]\n");
   #endif
      
   *list++ = OPERAND_READ1(ea);
   *list++ = OPERAND_READ1(ea + 1);
   *list++ = OPERAND_READ1(ea + 2);
   *list   = OPERAND_READ1(ea + 3);
}
#endif

void burst_read4(int *list, unsigned ea)
{
   unsigned		 device_index, index, relocation_base;
   unsigned		 offset;
   unsigned long	 absolute;
   int			 tag;

   int			*w24p;

   if (ea < 256)
   {
      *list       = _register[ea];
      *(list + 1) = _register[ea + 1];
      *(list + 2) = _register[ea + 2];
      *(list + 3) = _register[ea + 3];
      return;
   }

   if (index = ea >> 18)
   {
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = ea >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WINDOW_READ_RULE

   WINDOW_READ_RULE(tag, index);
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
      #ifdef BANK_EDGE_GUARD
      if (offset > 0x0003FFFC)
      {
         if (base[index + 1] ^ (relocation_base + 64))
         {
            #ifdef WAYPATH
            printf("[e]\n");
            #endif
      
            straddle_read4(offset, list, ea);
            return;
         }
      }
      #endif

      if (device_index = relocation_base & 63)
      {
         #ifdef WPROTECT

         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory read address nowhere is a non-device
               don't read from nowhere whoever you are
            *********************************************/

            if (psr & 0x00800000)
            {
            }
            else
            {
               GUARD_INTERRUPT
            }

            return;
         }

         #endif

         *list       = device_read(device_index, relocation_base, offset, 7);
         *(list + 1) = device_read(device_index, relocation_base, offset + 1, 7);
         *(list + 2) = device_read(device_index, relocation_base, offset + 2, 7);
         *(list + 3) = device_read(device_index, relocation_base, offset + 3, 7);
         return;
      }
   }
   #ifdef WPROTECT
   else
   {
      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_INTERRUPT
         }

         return;
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
      
               straddle_read4(offset, list, ea);
               return;
            }
         }
      }
      #endif
   }
   #endif

   absolute = (unsigned long) ((relocation_base & 0x003FFFFF) << 12) | offset;

   w24p = &memory.iaray[absolute];
   LOAD24(*list, *w24p);
   LOAD24(*(list + 1), *(w24p + 1));
   LOAD24(*(list + 2), *(w24p + 2));
   LOAD24(*(list + 3), *(w24p + 3));
}


static void device_write(int v, int device_index, int relocation_base, unsigned offset, int designator)
{
   int			*iarayp;
   msw			*w24p;
   word16		*w16p;

   device               *devicep = &devices[device_index];
   int			 bank = (relocation_base >> 6) & 65535;
   unsigned long	 absolute = (unsigned long) (bank << 18) | offset;


   if (devicep->flags & DEVICE)
   {
      if (devicep->flags & DATA16)
      {
         #ifndef CHECK_ON_BASE
         if (bank > base[128 + device_index] & 65535)
         {
         }
         else
         #endif
         {
            w16p = devicep->s.dev16->array + absolute;

            w16p->right = v;
            w16p->left  = v >> 8;
         }
      }
      else if (devicep->flags & FSYS24)
      {
         #ifndef CHECK_ON_BASE
         if (bank > base[128 + device_index] & 65535)
         {
         }
         else
         #endif
         {
            w24p = devicep->s.dev24->array + absolute;

            w24p->t3 = v;
            w24p->t2 = v >> 8;
            w24p->t1 = v >> 16;
         }
      }
      else if (devicep->flags & SYSMEM)
      {
         /**************************************************
            devices of sysmem character are configured
            as executable space with # 4K pages because
            executable space can also be accessed here
         **************************************************/

         #ifndef CHECK_ON_BASE
         if (bank > (base[128 + device_index] >> 6) & 65535)
         {
         }
         else
         #endif
         {
            mwrite(devicep->s.pages->array + absolute, designator, v);
//            ORDER32(*iarayp, v);
         }
      }
   }
}

#define OPERAND_WRITE1(DATA, EA) operand_write(DATA, EA, 7)

void operand_write(int v, unsigned ea, int designator)
{
   unsigned		 index, relocation_base, device_index;
   unsigned		 alternate = 0;
   unsigned		 offset;

   unsigned long	 absolute;

   word			*w24p;

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
            return;
         }
      }

      _register[ea] = v;
      return;
   }

   if (index = ea >> 18)
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
         return;
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
            return;
         }
      }

      if (alternate) index |= 64;
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
      if (device_index = relocation_base & 63)
      {
         #ifdef WPROTECT
         
         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory write target nowhere is a non-device
               don't write to nowhere whoever you are
            *********************************************/

            if (psr & 0x00800000)
            {
            }
            else
            {
               GUARD_IIX(4)
            }

            return;
         }

         #endif

         device_write(v, device_index, relocation_base, offset, designator);
         return;
      }
   }
   #ifdef WPROTECT
   else
   {
      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_IIX(5)
         }

         return;
      }
   }
   #endif

   absolute = (unsigned long) ((relocation_base & 0x003FFFFF) << 12) | offset;

   #ifndef CHECK_ON_BASE
   if (offset > (WORDS_IN_MEMORY-1))
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_IIX(6)
      }

      return;
   }
   #endif

   mwrite(memory.array + absolute, designator, v);
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

void burst_write2(int *list, unsigned ea)
{
   unsigned		 device_index, index, relocation_base;
   unsigned		 alternate = 0;
   unsigned		 offset;

   unsigned long	 absolute;

   int			*w24p;

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
            return;
         }
      }

      _register[ea]     = v;
      _register[ea + 1] = w;
      return;
   }

   if (index = ea >> 18)
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
         return;
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
            return;
         }
      }

      if (alternate) index |= 64;
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

            OPERAND_WRITE1(v, ea);
            OPERAND_WRITE1(w, ea + 1);
            return;
         }
      }
      #endif

      if (device_index = relocation_base & 63)
      {
         #ifdef WPROTECT
         
         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory write target nowhere is a non-device
               don't write to nowhere whoever you are
            *********************************************/

            if (psr & 0x00800000)
            {
            }
            else
            {
               GUARD_IIX(11)
            }

            return;
         }

         #endif

         device_write(v, device_index, relocation_base, offset, 7);
         device_write(w, device_index, relocation_base, offset + 1, 7);
         return;
      }
   }
   #ifdef WPROTECT
   else
   {
      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_IIX(12)
         }

         return;
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

               OPERAND_WRITE1(v, ea);
               OPERAND_WRITE1(w, ea + 1);
               return;
            }
         }
      }
      #endif
   }
   #endif


   absolute = (unsigned long) ((relocation_base & 0x003FFFFF) << 12) | offset;

   w24p = memory.iaray + absolute;
   ORDER32(*w24p, v);
   ORDER32(*(w24p + 1), w);
}

#ifdef BANK_EDGE_GUARD
static void straddle_write4(unsigned offset, int *list, unsigned ea)
{
   if ((offset & 3) == 2)
   {
      #ifdef WAYPATH
      printf("[i]\n");
      #endif

      burst_write2(list, ea);
      burst_write2(list + 2, ea + 2);
      return;
   }

   #ifdef WAYPATH
   printf("[j]\n");
   #endif

   OPERAND_WRITE1(*list++, ea);
   OPERAND_WRITE1(*list++, ea + 1);
   OPERAND_WRITE1(*list++, ea + 2);
   OPERAND_WRITE1(*list,   ea + 3);
   return;
}
#endif

void burst_write4(int *list, unsigned ea)
{
   unsigned		 device_index, index, relocation_base;
   unsigned		 alternate = 0;
   unsigned 		 offset;

   unsigned long	 absolute;

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
            return;
         }
      }

      _register[ea]     = v;
      _register[ea + 1] = w;
      _register[ea + 2] = _u;
      _register[ea + 3] = _l;
      return;
   }

   if (index = ea >> 18)
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
         return;
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
            GUARD_INTERRUPT
            return;
         }
      }

      if (alternate) index |= 64;
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
      
            straddle_write4(offset, list, ea);
            return;
         }
      }
      #endif

      if (device_index = relocation_base & 63)
      {
         #ifdef WPROTECT
         
         if (relocation_base & 0x00800000)
         {
            /*********************************************
               memory write target nowhere is a non-device
               don't write to nowhere whoever you are
            *********************************************/

            if (psr & 0x00800000)
            {
            }
            else
            {
               GUARD_INTERRUPT
            }

            return;
         }

         #endif

         device_write(v,  device_index, relocation_base, offset, 7);
         device_write(w,  device_index, relocation_base, offset + 1, 7);
         device_write(_u, device_index, relocation_base, offset + 2, 7);
         device_write(_l, device_index, relocation_base, offset + 3, 7);
         return;
      }
   }
   #ifdef WPROTECT
   else
   {
      if (offset & 0x0003F000)
      {
         /********************************************
            you can't do offsets > 4095 
            in 4096-word blocks
         ********************************************/
         
         if (psr & 0x00800000)
         {
         }
         else
         {
            GUARD_INTERRUPT
         }

         return;
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
      
               straddle_write4(offset, list, ea);
               return;
            }
         }
      }
      #endif
   }
   #endif

   absolute = (unsigned long) ((relocation_base & 0x003FFFFF) << 12) | offset;

   w24p = memory.iaray + absolute;
   ORDER32(*w24p, v);
   ORDER32(*(w24p + 1), w);
   ORDER32(*(w24p + 2), _u);
   ORDER32(*(w24p + 3), _l);
}

