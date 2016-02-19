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
	{ { DEVICE,           BANKS_IN_DEVICE,   { &memory } } ,
	  { DEVICE,           BANKS_IN_DEVICE,   { NULL    } } ,
	  { DEVICE | DATA16,  BANKS_IN_DEVICE16, { NULL    } } } ;

extern int			 _register[256];
extern int			 base[128];
extern int			 psr;

static int device_read(int device_index, int relocation_base, int offset)
{
   msw			*w24p;
   word16		*w16p;

   int			 v;

   device		*devicep = &devices[device_index];


   if (devicep->flags & DEVICE)
   {

      relocation_base &= -64; 
      offset |= relocation_base << 12;

      if (devicep->flags & DATA16)
      {
         w16p = &devicep->s.dev16->array[offset];

         v = (w16p->left << 8)
           |  w16p->right;
      }
      else
      {
         w24p = &devicep->s.dev24->array[offset];

         v = (w24p->t1 << 16)
           | (w24p->t2 <<  8)
           |  w24p->t3;
      }
   }
   else
   {
      v = 0x00FFFFFF;
   }

   return v;
}


word memory_read(int ea)
{
   static word	outside_executable_space = { 0, II, 1, 5 } ;

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

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index | 64];
   }

   if (relocation_base & 0x00400000)
   {
      if (relocation_base & 63) return outside_executable_space;
   }

   offset |= (relocation_base & 0x003FFFFF) << 12;

   if (offset > WORDS_IN_MEMORY)
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         GUARD_INTERRUPT
      }

      return outside_executable_space;
   }

   return memory.array[offset];
}

int operand_read(int ea, int designator)
{
   unsigned			 index, offset, relocation_base, v;

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

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   if ((relocation_base & 0x00C00000) == 0x00400000)
   {
      if (device_index = relocation_base & 63)
      {
         return device_read(device_index, relocation_base, offset);
      }
   }

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

   switch (designator)
   {
      case 0:
      case 7:
         v = (w24p->t1 << 16)
           | (w24p->t2 <<  8)
           |  w24p->t3;
         break;
      case 1:
         v = w24p->t1;
         v -= (v & 128) << 1;
         break;
      case 2:
         if (psr & HALFWORD)
         {
            v = ((w24p->t2 & 15) << 8) | w24p->t3;
            v -= (v & 2048) << 1;
            break;
         }
         v = w24p->t2;
         v -= (v & 128) << 1;
         break;
      case 3:
         if (psr & HALFWORD)
         {
            v = (w24p->t1 << 4) | ((w24p->t2 & 240) >> 4);
            v -= (v & 2048) << 1;
            break;
         }
         v = w24p->t3;
         v -= (v & 128) << 1;
         break;
      default:
         v = ea;
   }
   return v;
}

void burst_read2(int *list, int ea)
{
   unsigned		 index, relocation_base, offset, device_index;

   word			*w24p;

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

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   if ((relocation_base & 0x00C00000) == 0x00400000)
   {
      if (device_index = relocation_base & 63)
      {
         *list       = device_read(device_index, relocation_base, offset);
         *(list + 1) = device_read(device_index, relocation_base, offset + 1);
         return;
      }
   }

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

   w24p = &memory.array[offset];

   *list        = (w24p->t1 << 16)
                | (w24p->t2 <<  8)
                |  w24p->t3;

   w24p++;

   *(list + 1)  = (w24p->t1 << 16)
                | (w24p->t2 <<  8)
                |  w24p->t3;
}


void burst_read4(int *list, int ea)
{
   unsigned		 device_index, index, relocation_base, offset;

   word                 *w24p;

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

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   if ((relocation_base & 0x00C00000) == 0x00400000)
   {
      if (device_index = relocation_base & 63)
      {
         *list       = device_read(device_index, relocation_base, offset);
         *(list + 1) = device_read(device_index, relocation_base, offset + 1);
         *(list + 2) = device_read(device_index, relocation_base, offset + 2);
         *(list + 3) = device_read(device_index, relocation_base, offset + 3);
         return;
      }
   }

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

   w24p = &memory.array[offset];

   *list        = (w24p->t1 << 16)
                | (w24p->t2 <<  8)
                |  w24p->t3;

   w24p++;

   *(list + 1)  = (w24p->t1 << 16)
                | (w24p->t2 <<  8)
                |  w24p->t3;
   w24p++;

   *(list + 2)  = (w24p->t1 << 16)
                | (w24p->t2 <<  8)
                |  w24p->t3;
   w24p++;

   *(list + 3)  = (w24p->t1 << 16)
                | (w24p->t2 <<  8)
                |  w24p->t3;
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
      else
      {
         w24p = &devicep->s.dev24->array[offset];

         w24p->t3 = v;
         w24p->t2 = v >> 8;
         w24p->t1 = v >> 16;
      }
   }
}


void operand_write(int v, int ea, int designator)
{
   unsigned		 index, offset, relocation_base, device_index;

   word			*w24p;

   if (ea < 256)
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         if (ea < 128)
         {
         }
         else return;
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

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   if ((relocation_base & 0x00C00000) == 0x00400000)
   {
      if (device_index = relocation_base & 63)
      {
         device_write(v, device_index, relocation_base, offset);
         return;
      }
   }

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
         w24p->t3 = v;
         w24p->t2 = v >> 8;
         w24p->t1 = v >> 16;
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
         else w24p->t1 = v;
         break;
      case 3:
         if (psr & HALFWORD)
         {
            w24p->t3 = v;
            w24p->t2 =  (w24p->t2  & 240)
                   | ((v >> 8) &  15);
         }
         else w24p->t1 = v;
         break;
   }
}

void burst_write2(int *list, int ea)
{
   int			 v = *list;
   int			 w = *(list + 1);

   unsigned		 device_index, index, relocation_base, offset;

   word			*w24p;

   if (ea < 256)
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         if (ea < 127)
         {
         }
         else return;
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

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   if ((relocation_base & 0x00C00000) == 0x00400000)
   {
      if (device_index = relocation_base & 63)
      {
         device_write(v, device_index, relocation_base, offset);
         device_write(w, device_index, relocation_base, offset + 1);
         return;
      }
   }

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

   w24p = &memory.array[offset];

   w24p->t3 = v;
   w24p->t2 = v >> 8;
   w24p->t1 = v >> 16;

   w24p++;

   w24p->t3 = w;
   w24p->t2 = w >> 8;
   w24p->t1 = w >> 16;
}


void burst_write4(int *list, int ea)
{
   int                   v  = *list;
   int                   w  = *(list + 1);
   int			 _u = *(list + 2);
   int                   _l = *(list + 3);

   unsigned		 device_index, index, relocation_base, offset;

   word                 *w24p;

   if (ea < 256)
   {
      if (psr & 0x00800000)
      {
      }
      else
      {
         if (ea < 125)
         {
         }
         else return;
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

   relocation_base = base[index];

   if ((index) && (index < 8))
   {
      if ((psr << index) & 32768) relocation_base = base[index + 64];
   }

   if ((relocation_base & 0x00C00000) == 0x00400000)
   {
      if (device_index = relocation_base & 63)
      {
         device_write(v,  device_index, relocation_base, offset);
         device_write(w,  device_index, relocation_base, offset + 1);
         device_write(_u, device_index, relocation_base, offset + 2);
         device_write(_l, device_index, relocation_base, offset + 3);
         return;
      }
   }

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

   w24p = &memory.array[offset];

   w24p->t3 = v;
   w24p->t2 = v >> 8;
   w24p->t1 = v >> 16;

   w24p++;

   w24p->t3 = w;
   w24p->t2 = w >> 8;
   w24p->t1 = w >> 16;

   w24p++;

   w24p->t3 = _u;
   w24p->t2 = _u >> 8;
   w24p->t1 = _u >> 16;

   w24p++;

   w24p->t3 = _l;
   w24p->t2 = _l >> 8;
   w24p->t1 = _l >> 16;
}

