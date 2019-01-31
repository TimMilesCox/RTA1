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
#include "ii.h"

extern int		 iselect,
			 psr,
			 b0_name,
			 _register[],
			 base[];

extern page		*b0p;
extern word		*apc;
extern system_memory	 memory;

extern int		 indication;
extern char		 flag[];
extern char		 uflag[];

void ii(int ea, int latent_parameter)
{
   int			*temp_sp;

   iselect = 128;
   sp -= 4;

   temp_sp = &_register[sp];

   temp_sp[3] = apc - b0p->w;
   temp_sp[2] = b0_name;
   temp_sp[1] = psr;
   temp_sp[0] = latent_parameter;

   if (psr & 0x00800000)
   {
   }
   else
   {
      /***************************************************
	if not already in an ISR
	save the interrupted application packet
      ***************************************************/

      fp = sp;
   }

   psr |= 0x00800000;

   if (ea == XBASE_U)
   {
      if (uflag['Z'-'A'] == 0) indication |= LOCKSTEP;
   }

   b0_name = (ea >> 6) & 0x0000FFFF;
   base[0] = b0_name;
   b0p     = &memory.p4k[b0_name];

   apc = &b0p->w[ea & 63];
}
