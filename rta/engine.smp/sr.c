/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is part of the emulated-instruction-execution-subsection
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



#include "emulate.h"
#include "smp.h"
#include "rw.h"
#include "sr.h"
#include "operand.h"

#if 0
extern int		 psr;
extern int		 iselect;
extern int		 _register[];
#endif

/***********************************************************************

	emulate the WRITE STAGING REGISTER instruction

	bits are added to a byte or bit string from
	accumulator register A, at absolute address 4 or 132

	the +- number of bits is the sign extended immediate operand

	+ for left-to-right progression, data shifted left from MS end of A

        Right to left working may be later implemented using, for example,
	a negative bit count in the sign-extended operand.

	This is not in this initial emulator, because the -operand
	instruction encoding of a right-to-left working is not decided.

	the primitive block lshift2w(int distance, int data[]) shifts left for
	+count

	it may later be decided to have rshift2w() for a -count

	data is shifted into register WDATA at absolute address 23 or 151
	count of data bits is in WDATAC at absolute address 22 or 150

        WDATAC has a count of 0 indicating 24 free positions when no
	unwritten data is in WDATA

	WSR instruction increments WDATAC

	when WDATAC reaches a count of 24 during WSR instruction
	a data word is written and WDATAC is restarted

	For software using these registers and this instruction, it is
	intuitively clear that WDATAC contains the positive number of bits
	in WDATA awaiting storage, zero = no bits awaiting storage write.

	Software then knows whether to complete a series of string store
	operations by masking and writing a final part-word to storage.

	The function here calculates new bits added + WDATAC - 24
	and triggers storage write on reaching zero. This is meant to
	reflect possible simple hardware implementation. The add result
	crosses a carry point on reaching or on exceeding zero. This
	instruction never updates carry in PSR, but the micro-carry
	on reaching zero from minus range triggers the storage write to p++;

	Storage write takes place when WDATA is full, not on a subsequent
	request to add more data.


************************************************************************/


static void lshift2w(int distance, int data[])
{
   data[0] <<= distance;
   data[0]  |= (data[1] & 0x00FFFFFF) >> (24-distance);
   data[0]  &= 0x00FFFFFF;

   data[1] <<= distance;
   data[1]  &= 0x00FFFFFF;
}

void wsr(int ea, smp *xcore)
{
   int		*register_set = xcore->register_set;
   int		 temp[2]   = { wdata, a } ;

   int		 request   = ea,
                 wspace    = (psr & BSTREAM16W) ? -16 : -24,
		 available = (wdatac & 0x00FFFFFF) + wspace,
		 remainder = available + request;

//   if (psr & BSTREAM16W) remainder += 8;

   if (remainder < 0)
   {
   }
   else
   {
      request -= remainder;
      if (available) lshift2w(request, temp);
      OPERAND_WRITE(temp[0], p++, 0);
      temp[0] = 0;

      p &= 0x00FFFFFF;
      request = remainder;
      available = wspace;
   }

   if (request) lshift2w(request, temp);
   available += request;

   a = temp[1] & 0x00FFFFFF;
   wdata = temp[0] & 0x00FFFFFF;

   wdatac = (available - wspace) & 0x00FFFFFF;
}

void rsr(int ea, smp *xcore)
{
   int		*register_set = xcore->register_set;
   int		 temp[2]   = { a, rdata } ;

   int		 request   = ea,
		 available = rdatac,
		 remaining = available - request;

   if (psr & BSTREAMZR) temp[0] = 0;

   if (remaining < 0)
   {
      if (available) lshift2w(available, temp);
      OPERAND(temp[1], q++, 0)
      remaining = 24;

      if (psr & BSTREAM16R)
      {
         temp[1] <<= 8;
         temp[1] &= 0x00FFFF00;
	 remaining = 16;
      }
      
      q &= 0x00FFFFFF;
      request -= available;
      remaining -= request;
   }

   if (request)
   {
      lshift2w(request, temp);
   }

   a = temp[0] & 0x00FFFFFF;
   rdata = temp[1] & 0x00FFFFFF;
   rdatac = remaining & 0x00FFFFFF;
}

