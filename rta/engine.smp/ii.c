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

#include <stdio.h>
#include "../include.rta/argue.h"
#include "emulate.h"
#include "smp.h"
#include "ii.h"
#include "../rta.run/settings.h"

#ifdef	RTA_SMP
extern	device		 devices[];
#define	memory		 devices[0].pages
extern	smp		 core[];
extern	int		 general_indication;
#else
extern int		 iselect,
			 psr,
			 b0_name,
			 _register[],
			 base[];

extern page		*b0p;
extern word		*apc;
extern word		*apcz;
extern int		 indication;
extern char		 flag[];
extern char		 uflag[];
extern system_memory	 memory;
#endif

void ii(int ea, int latent_parameter, smp *xcore)
{
   int			*temp_sp;
   int			 v;


   if (ea == XBASE_U)
   {
      /**************************************************
        switch to single step if option flag large -Z
      **************************************************/

      if (uflag['Z'-'A'])
      {
         general_indication |= LOCKSTEP | BREAKPOINT;
         indication |= LOCKSTEP;
      }
   }

   iselect = 128;
   xcore->register_set = xcore->REGISTER + 128;

   v = x_sp;
   v -= 4;

   if ((v < GUARD_RANGE_ILC) || (v > GUARD_RANGE_IU))
   {
      /**************************************************
	if ii or external interrupt happened and x_sp
	(interrupt internal stack pointer sp_x) is crazed
	it's time to restart -> vector 0:1
        switch to single step if option flag large -Z
        interrupt in trouble does not complete
      **************************************************/

      x_sp = GUARD_RANGE_IU;
      ii(RESTART1,v,xcore);
      if (uflag['Z'-'A']) indication |= LOCKSTEP;
      return;
   }

   x_sp = v;

   temp_sp = _register + v;

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

      x_fp = x_sp;
   }

   psr |= 0x00800000;

   b0_name = (ea >> 6) & 0x0000FFFF;
   base[0] = b0_name;
   base[64] = b0_name;
   b0p     = memory + b0_name;

   apc = b0p->w + (ea & 63);
   apcz = b0p->w + 4095;
   if (flag['f'-'a']) printf("[%x:%x %p %p %p]\n", b0_name, ea & 63, apc, memory, b0p);
   if ((flag['g'-'a']) && (ea < 64)) printf("[%x:%x %x:%x %x]\n",
                                             b0_name, ea & 63, base[77], latent_parameter,
                                             indication & INDICATIONS);
}

extern void *breakpoint;
void xi(smp *xcore)
{
   int		 mask,
		 unmask,
                 vector,
		 index;

   int		 latent_parameter,
		 pin;

   smp		*ascor;

   if (indication & EXTERNAL_INTERRUPT)
   {
      base[74] = (indication >> 8) & 255;
      indication &= 255;
      ii(II_XINT, 0, xcore);
   }

   if (indication & TPULSE)
   {
      /************************************************************
		this little ole present is always from core[0]
		ya wanna make it fancier so make it work yaself
      ************************************************************/

      ascor = core;

      #ifdef __X64	/*	x686 Land	*/

      *((long *) (_register + DAYCLOCK_U))
      = *((long *) (ascor->REGISTER +  DAYCLOCK_U));

      #else		/*	x386 Land	*/

      /************ maybe we should recompare the top half	*/
      /************ no guarantee reading 64 bits <- 1 cycle	*/

      *((long long *) (_register + DAYCLOCK_U))
      = *((long long *) (ascor->REGISTER +  DAYCLOCK_U));

      #endif

      base[101] = ascor->BASE[101];

      /*   timezone + summer seconds offset from GMT   */

      indication &= TPULSE ^ -1;
   }

   if (indication & ATTENTION)
   {
      mask = base[78];
      index = RTA_SMP;
      unmask = -2;
      ascor = core;

      // printf("[%x %x : %x ->]\n", mask, base[77], x_sp);

      while (index--)
      {
         if (mask & 1)
         {
	    latent_parameter = ascor->BASE[79];

            if (flag['g'-'a']) printf("[*I %x %x %x:%x -> %x]\n",
                               pin, latent_parameter, index, ascor->BASE[77], base[77]);

            ii(II_ATTENTION, latent_parameter, xcore);

            base[78] &= unmask;
         }

         ascor++;
	 unmask <<= 1;
	 unmask |= 1;	/* i.e. rotate so you don't suppress new events	*/
			/*	unmask is a rotating zero bit		*/

         mask >>= 1;
         if ((mask &= base[125]) == 0) break;
      }

      if (base[78] == 0) indication &= ATTENTION ^ -1;

      /***********************************************************
		more broadcast events can have got flagged
		while we worked through the mask readout
		if they didn't then push down ATTENTION
      ***********************************************************/
   }

   if (indication & INCREMENTER_CARRY)
   {
      indication ^= INCREMENTER_CARRY;
      ii(II_MSECOND, 0, xcore);
   }
}
