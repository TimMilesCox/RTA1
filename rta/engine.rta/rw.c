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

int device_read(int device_index, int relocation_base, int offset)
{
   msw			*w24p;
   word16		*w16p;

   int			 v;

   device		*devicep = &devices[device_index];


   if (devicep->flags & DEVICE)
   {
      if (device_index)
      relocation_base &= -64; 
      offset |= relocation_base << 12;

      if (devicep->flags & DATA16)
      {
         w16p = &devicep->s.dev16->array[offset];

         v = (w16p->left << 8)
           |  w16p->right;
      }
      else if (devicep->flags & FSYS24)
      {
         w24p = &devicep->s.dev24->array[offset];

         v = (w24p->t1 << 16)
           | (w24p->t2 <<  8)
           |  w24p->t3;
      }
      else if (devicep->flags & SYSMEM)
      {
         if (offset < WORDS_IN_MEMORY)
         {
            LOAD24(v, memory.iaray[offset]);
         }
         else v = 0x00FFFFFF;
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


word memory_read(int ea)
{
   static word	 outside_executable_space = { 0, II, 0, (1 << 6) | 31 } ;
   static word	 nop                      = { 0, LK, 0, 128 + 1 } ;

   unsigned	 relocation_base, offset;
   int		 index = ea & 0x00FC0000;

   if (index)
   {
      offset = ea & 0x0003FFFF;
      index >>= 18;
   }
   else
   {
      index  = ea & 0x0003F000;
      offset = ea & 0x00000FFF;
      index >>= 12;
   }

   if ((index) && ((index & 0x38) == 0))
   {
      if ((32768 >> index) & psr) index |= 64;
   }

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index | 64];
   }

   if (relocation_base & 0x00400000)
   {
      if (relocation_base & 63)
      {
         if (psr & 0x00800000) return nop;
         return outside_executable_space;
      }
   }
   #ifdef WPROTECT
   else
   {
      if (offset & 0x0003F000)
      {
         if (psr & 0x00800000) return nop;
         return outside_executable_space;
      }
   }
   #endif

   offset |= (relocation_base & 0x003FFFFF) << 12;

   if (offset > WORDS_IN_MEMORY-1)
   {
      if (psr & 0x00800000) return nop;
      return outside_executable_space;
   }

   return memory.array[offset];
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

word *memory_hold(int ea)
{
   unsigned      relocation_base, offset;
   unsigned	 alternate = 0;
   int           index = ea & 0x00FC0000;

   if (index)
   {
      offset = ea & 0x0003FFFF;
      index >>= 18;
   }
   else
   {
      index  = ea & 0x0003F000;
      offset = ea & 0x00000FFF;
      index >>= 12;
   }

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

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index | 64];
   }

   if (relocation_base & 0x00400000)
   {
      if (relocation_base & 63)
      {
         /********************************************
            currently nothing outside executable space
            can be locked for update
            but this may change for memories similar
            to system memory in other device arrays
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

         return NULL;
      }
   }
   #endif

   offset |= (relocation_base & 0x003FFFFF) << 12;

   if (offset > WORDS_IN_MEMORY-1)
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT;
      }

      return NULL;
   }

   return memory.array + offset;
}

#endif		/*	ABSOTS	*/

unsigned int operand_read(int ea, int designator)
{
   unsigned		 index, offset, relocation_base;
   int			 v;

   word			*w24p;
   int			 device_index;

   if (designator ==  I) return ea;
   if (designator == XI) return ea;

   if (ea < 256) return _register[ea];

   if (index = ea & 0x00FC0000)
   {
      index >>= 18;
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = (ea & 0x0003F000) >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WPROTECT

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

         return device_read(device_index, relocation_base, offset);
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

   offset |= (relocation_base & 0x003FFFFF) << 12;

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

   w24p = &memory.array[offset];
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

void burst_read2(int *list, int ea)
{
   unsigned		 index, relocation_base, offset, device_index;

   int			*w24p;

   if (ea < 256)
   {
      *list       = _register[ea];
      *(list + 1) = _register[ea + 1];
      return;
   }
 
   if (index = ea & 0x00FC0000)
   {
      index >>= 18;
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = (ea & 0x0003F000) >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WPROTECT

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

         *list       = device_read(device_index, relocation_base, offset);
         *(list + 1) = device_read(device_index, relocation_base, offset + 1);
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
   }
   #endif

   offset |= (relocation_base & 0x003FFFFF) << 12;

   if (offset > (WORDS_IN_MEMORY-2))
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT
      }

      return;
   }

   w24p = &memory.iaray[offset];
   LOAD24(*list, *w24p);
   LOAD24(*(list + 1), *(w24p + 1));
}


void burst_read4(int *list, int ea)
{
   unsigned		 device_index, index, relocation_base, offset;

   int			*w24p;

   if (ea < 256)
   {
      *list       = _register[ea];
      *(list + 1) = _register[ea + 1];
      *(list + 2) = _register[ea + 2];
      *(list + 3) = _register[ea + 3];
      return;
   }

   if (index = ea & 0x00FC0000)
   {
      index >>= 18;
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = (ea & 0x0003F000) >> 12;
      offset = ea & 0x00000FFF;
   }

   relocation_base = base[index];

   #ifdef WPROTECT 

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

         *list       = device_read(device_index, relocation_base, offset);
         *(list + 1) = device_read(device_index, relocation_base, offset + 1);
         *(list + 2) = device_read(device_index, relocation_base, offset + 2);
         *(list + 3) = device_read(device_index, relocation_base, offset + 3);
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
   }
   #endif

   offset |= (relocation_base & 0x003FFFFF) << 12;

   if (offset > (WORDS_IN_MEMORY-4))
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT
      }

      return;
   }

   w24p = &memory.iaray[offset];
   LOAD24(*list, *w24p);
   LOAD24(*(list + 1), *(w24p + 1));
   LOAD24(*(list + 2), *(w24p + 2));
   LOAD24(*(list + 3), *(w24p + 3));
}


static void device_write(int v, int device_index, int relocation_base, int offset)
{
   msw			*w24p;
   word16		*w16p;

   device               *devicep = &devices[device_index];


   if (devicep->flags & DEVICE)
   {
      relocation_base &= 0x003FFFC0;
      offset |= relocation_base << 12;
   
      if (devicep->flags & DATA16)
      {
         if (relocation_base < (BANKS_IN_DEVICE16 << 6))
         {
            w16p = &devicep->s.dev16->array[offset];

            w16p->right = v;
            w16p->left  = v >> 8;
         }
      }
      else if (devicep->flags & FSYS24)
      {
         w24p = &devicep->s.dev24->array[offset];

         w24p->t3 = v;
         w24p->t2 = v >> 8;
         w24p->t1 = v >> 16;
      }
      else if (devicep->flags & SYSMEM)
      {
         if (offset < WORDS_IN_MEMORY)
         {
            ORDER32(memory.iaray[offset], v);
         }
      }
   }
}


void operand_write(int v, int ea, int designator)
{
   unsigned		 index, offset, relocation_base, device_index;
   unsigned		 alternate = 0;

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

            GUARD_INTERRUPT
            return;
         }
      }

      _register[ea] = v;
      return;
   }

   if (index = ea & 0x00FC0000)
   {
      index >>= 18;
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = (ea & 0x0003F000) >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WPROTECT

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

         device_write(v, device_index, relocation_base, offset);
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
   }
   #endif

   offset |= (relocation_base & 0x003FFFFF) << 12;

   if (offset > (WORDS_IN_MEMORY-1))
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT
      }

      return;
   }

   w24p = &memory.array[offset];

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

void burst_write2(int *list, int ea)
{
   unsigned		 device_index, index, relocation_base, offset;
   unsigned		 alternate = 0;

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
            
            GUARD_INTERRUPT
            return;
         }
      }

      _register[ea]     = v;
      _register[ea + 1] = w;
      return;
   }

   if (index = ea & 0x00FC0000)
   {
      index >>= 18;
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = (ea & 0x0003F000) >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WPROTECT

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
      if (offset == 0x0003FFFF)
      {
         /****************************************************
            1st word of 2 is last of block
            write words beyond block to YOUR next block if any
         ****************************************************/

         if (base[index + 1] ^ (relocation_base + 64)) return;

         /****************************************************
           if that isn't within the anticipated array limit
           fail to write silently. Or guard interrupt if preferred

           base 64 is static zero
           and should not give a spurious match on base 63 + 1
         ****************************************************/
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

         device_write(v, device_index, relocation_base, offset);
         device_write(w, device_index, relocation_base, offset + 1);
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
      if (offset == 0x00000FFF) 
      {  
         /****************************************************
            1st word of 2 is last of block
            write words beyond block to YOUR next block if any
         ****************************************************/
            
         if (base[index + 1] ^ (relocation_base + 1)) return;
            
         /****************************************************
           if that isn't within the anticipated array limit
           fail to write silently. Or guard interrupt if preferred

           base 64 is static zero
           and should not give a spurious match on base 63 + 1
         ****************************************************/
      }
      #endif
   }
   #endif


   offset |= (relocation_base & 0x003FFFFF) << 12;

   if (offset > (WORDS_IN_MEMORY-2))
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT
      }

      return;
   }

   w24p = memory.iaray + offset;
   ORDER32(*w24p, v);
   ORDER32(*(w24p + 1), w);
}

void burst_write4(int *list, int ea)
{
   unsigned		 device_index, index, relocation_base, offset;
   unsigned		 alternate = 0;

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
    
            GUARD_INTERRUPT
            return;
         }
      }

      _register[ea]     = v;
      _register[ea + 1] = w;
      _register[ea + 2] = _u;
      _register[ea + 3] = _l;
      return;
   }

   if (index = ea & 0x00FC0000)
   {
      index >>= 18;
      offset = ea & 0x0003FFFF;
   }
   else
   {
      index = (ea & 0x0003F000) >> 12;
      offset = ea & 0x00000FFF;
   }

   #ifdef WPROTECT

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
      if ((offset & 0x0003FFFC) ^ 0x0003FFFC)
      {
         /****************************************************
            if offset bits 17..2 are not all 1s
            then you are not at the last 4 words in the block
            so 4 words may be written in a burst
         ****************************************************/
      }
      else if (offset & 3)
      {
         /****************************************************
            1st word of 4 is in last 3 of  block
            write words beyond block to YOUR next block if any
         ****************************************************/

         if (base[index + 1] ^ (relocation_base + 64)) return;

         /****************************************************
           if that isn't in the anticipated array limit
           fail to write silently. Or guard interrupt if preferred

           base 64 is static zero
           and should not give a spurious match on base 63 + 1
         ****************************************************/
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

         device_write(v,  device_index, relocation_base, offset);
         device_write(w,  device_index, relocation_base, offset + 1);
         device_write(_u, device_index, relocation_base, offset + 2);
         device_write(_l, device_index, relocation_base, offset + 3);
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
      if ((offset & 0x00000FFC) ^ 0x00000FFC)
      {
         /****************************************************
            if offset bits 11..2 are not all 1s
            then you are not at the last 4 words in the block
            so 4 words may be written in a burst
         ****************************************************/
      }
      else if (offset & 3)
      {
         /****************************************************
            1st word of 4 is in last 3 of  block
            write words beyond block to YOUR next block if any
         ****************************************************/

         if (base[index + 1] ^ (relocation_base + 1)) return;

         /****************************************************
           if that isn't in the anticipated array limit
           fail to write silently. Or guard interrupt if preferred

           base 64 is static zero
           and should not give a spurious match on base 63 + 1
         ****************************************************/
      }
      #endif
   }
   #endif

   offset |= (relocation_base & 0x003FFFFF) << 12;

   if (offset > (WORDS_IN_MEMORY-4))
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT
      }
      return;
   }

   w24p = &memory.iaray[offset];
   ORDER32(*w24p, v);
   ORDER32(*(w24p + 1), w);
   ORDER32(*(w24p + 2), _u);
   ORDER32(*(w24p + 3), _l);
}

