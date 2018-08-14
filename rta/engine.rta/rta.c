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

#ifdef	X86_MSW
#include <windows.h>
#endif

#include <stdio.h>
#include "emulate.h"
#include "fpu.h"
#include "rw.h"
#include "alu.h"
#include "sr.h"
#include "ii.h"

#include "../rta.run/settings.h"

#ifdef	EDGE
#include "../rta.run/idisplay.h"
#endif

extern int			 indication;

extern system_memory		 memory;
#define ROM_PAGE		 &memory.p4k[0].w[0];

int		 psr = 0x00800000;
word		*apc = ROM_PAGE;

int		 iselect = 128;

int		 b0_name;
page		*b0p = memory.p4k;

int		 _register[REGISTERS];

			/*********************************************
				The IO ports are given the name
				base[] because the first 72 of them
				are the relocation registers

				base 0 addresses the current instruction segment
				and words 256..4095 of this segment are
				in the operand address space, mostly so
				that instruction code can contain data constants
					
				Application data is otherwise
				based in bases 1..63

				Base 65 addresses the task control block
				and bases 66..71 address kernel data

				data based with 65..71 is not visible
				when data based with 1..7 is visible

				psr flag SEE$1 selects TCB page 65 instead of
				application automatic data page 1, ADP

				psr flags SEE$2..SEE$7 select the kernel
				data buffer bases 66..71 instead of application
				data spaces in base 2..7

                                IO ports base[128..191] contain information
				about the possible 64 Devices (essentially
				memory arrays)

				the first of these, Device 0, is system memory
				or the executable space

				System memory and any other device can be an
				array of up to 16 gigawords

				Therefore system memory plus the maximum
				63 other data arrays may be one teraword.

				Practically at least one device will be other
				than an array of 24-bit words. Network
				interfaces may populate a device with
				dual-ported 16-bit memory on the controller
				chip.


			*********************************************/


int	 base[IO_PORTS] = { 	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,

			/**********************************************
				above 72 base registers
				below 54 spare, then sys$switch
	                 	and sys$leds
			**********************************************/

							0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,

			/**********************************************

				sys$switch = 1 means try to boot
				from array device 1, which should be
				a 24-bit file system with a boot file

				if the external switches are implemented
				they can select any device 1..63 for
				boot

				the next 64 IO-Ports are the device
				control flags, where bit 23 says,
				here is a device, bit 22 says it is an
				array of 24-bit words which can be
				used as a filestore, bit 21 may hint
				that the filestore has a boot file

				device zero at port 128 is always
				system memory in pages. Device flag
				bit 23 is not examined. Emulator startup
				sets the memory size

				bit 18 says this isn't a 24-bit array,
				but the data words in it are 16-bits
				network interfaces are expected to
				be 16 bit memories

				The lower 16 bits may later contain
				an indication of the highest bank index
				in the device, maximum 65535

				There are three devices in this
				emulator: system memory (device 0);
				an array of 24-bit words containing
				a filestore and a boot file (device 1);
				and an array of 16-bit words which
				will buffer a network interface (device 2)

			**********************************************/

				0x00800000,
				0x00E00000,
				0x00880000 | BANKS_IN_DEVICE16,

                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } ;




void execute(word instruction)
{
   int		 designator = instruction.t1 & 7;
   int		 ea, xtag;

   int		*_p;

   int		 v, w;
   int		 buffer[2];

   word		*_w;

   EFFECTIVE_ADDRESS

   switch (designator)
   {
      case 6:
         switch (instruction.t1)
         {
            case SAR:
               sar(ea);		/* shift A right	alu.c	*/
               break;

            case SBR:
               sbr(ea);         /* shift B right        alu.c   */
               break;

            case DSR:
               dsr(ea);         /* double shift right   alu.c   */
               break;

            case JDR:

               /****************************************************

                        jump decrement R

                        always decrement R
                        jump unless R was zero before the instruction

               *****************************************************/

               v = (r & 0x00FFFFFF) + 0x00FFFFFF;
               r = v & 0x00FFFFFF;
               if (v & 0x01000000) apc = &b0p->w[ea];

               break;

            case SAL:
               sal(ea);         /* shift A left         alu.c   */
               break;

            case SBL:
               sbl(ea);         /* shift B left         alu.c   */
               break;

            case DSL:
               dsl(ea);         /* double shift left    alu.c   */
               break;

            case LCAL:
               /****************************************************

                        local call

                        push the saved program counter
                        = location of following instructiom
                        = absolute program counter minus base B0
                        on the internal stack

                        jump to ea

                ****************************************************/

               sp--;

               if (psr & 0x00800000)
               {
               }
               else
               {
                  if (sp < GUARD_RANGE_SP)
                  {
                     GUARD_INTERRUPT
                     break;
                  }
               }

               _p = &_register[sp];
               *_p = apc - b0p->w;

               apc = &b0p->w[ea];
               break;

            case RAR:
               rar(ea);         /* rotate A right       alu.c   */
               break;

            case RBR:
               rbr(ea);         /* rotate B right       alu.c   */
               break;

            case DRR:
               drr(ea);         /* double rotate right  alu.c   */
               break;

            case JNC:

				/* 	jump no carry		*/
	       if ((psr & CARRY) == 0) apc = &b0p->w[ea];
               break;

            case RAL:
               ral(ea);         /* rotate A left        alu.c   */
               break;

            case RBL:
               rbl(ea);         /* rotate B left        alu.c   */
               break;

            case DRL:
               drl(ea);         /* double rotate left   alu.c   */
               break;

            case JC:
				/* jump carry			*/
	       if (psr & CARRY) apc = &b0p->w[ea];
               break;

            case SAA:
               saa(ea);		/* shift A algebraic	alu.c	*/
               break;

            case SBA:
               sba(ea);         /* shift B algebraic    alu.c   */
               break;

            case DSA:
               dsa(ea);         /* double shift algebraic alu.c */
               break;

            case JAO:
               if (a & 1) apc = &b0p->w[ea];
               break;

            case JPA:		

               /****************************************************

			jump positive A

			jump if bit 23 of A is clear

               *****************************************************/


               if (a & 0x00800000)
               {
               }
               else apc = &b0p->w[ea];

               break;

            case JPB:

               /****************************************************

			jump positive B

			jump if bit 23 of B is clear

		****************************************************/


               if (b & 0x00800000)
               {
               }
               else apc = &b0p->w[ea];
               break;

            case J:

               /******************************************************
                        J instruction. Jump intra bank
               *******************************************************/

               apc = &b0p->w[ea];
               break;

            case JPO:
               v = a & k;
               v &= 0x00FFFFFF;
               v ^= v >> 16;
               v ^= v >>  8;
               v ^= v >>  4;
               v ^= v >>  2;
               v ^= v >>  1;

               if (v & 1)  apc = &b0p->w[ea];

               break;

            case JZA:

	       if (a & 0x00FFFFFF)
               {
               }
               else apc = &b0p->w[ea];

               break;

            case JZB:
	       if (b & 0x00FFFFFF)
	       {
               }
               else  apc = &b0p->w[ea];
               break;

            case JNZA:
               if (a & 0x00FFFFFF) apc = &b0p->w[ea];
               break;

            case JNZB:
               if (b & 0x00FFFFFF) apc = &b0p->w[ea];
               break;

            case JNA:
               if (a & 0x00800000) apc = &b0p->w[ea];
               break;

            case JNB:
               if (b & 0x00800000) apc = &b0p->w[ea];
               break;

            case JXGE:

               /******************************************************
                        JXGE instruction (jump x not less than r)
               ******************************************************/

               if ((x & 0x00FFFFFF) < (r & 0x00FFFFFF))
               {
               }
               else apc = &b0p->w[ea];

               break;

            case JYGE:

               /******************************************************
                        JYGE instruction (jump y not less than r)
               ******************************************************/

               if ((y & 0x00FFFFFF) < (r & 0x00FFFFFF))
               {
               }
               else apc = &b0p->w[ea];

               break;
         }
         break;

      case 7:
         switch (instruction.t1)
         {
            case TS:

               /*************************************************

			test and set

			always set bit 23 of the storage
			operand

			skip next instruction if that bit
			was previously clear

			touch no flags

               *************************************************/


               #ifdef ABSOTS

               /************************************************
                  operand_read() and operand_write()
                  don't look at the designator argument
                  before classing EA in register range
                  so switch ABSO uses * _w = memory_hold(ea)
                  to get a pointer for TS instruction
                  and that is one less call to operand_etc()

                  of course TS target is never in range 0..255
               ************************************************/

	       _w = memory_hold(ea);
               if (_w == NULL) break;
               v = _w->t1;
               v ^= 128;

               if (v & 128)
               {
                  _w->t1 = v;
                  apc++;
               }

               #else

               v = operand_read(ea, 7);
               v ^= 0x00800000;

               if (v & 0x00800000)
               {
                  operand_write(v, ea, 7);
                  apc++;
               }

               #endif

               break;

            case N:

               /************************************************

			negate

			ones complement invert the storage
			operand

			touch no flags

               ************************************************/

               v = operand_read(ea, 7);
               v ^= 0x00FFFFFF;
               operand_write(v, ea, 7);
               break;

            case INC:

               /***********************************************

			increment

			touch no flags

               ***********************************************/

               v = operand_read(ea, 7);
               v++;
               operand_write(v & 0x00FFFFFF, ea, 7);
               break;

            case DEC:

               /**************************************************

			decrement

			touch no flags

               **************************************************/

               v = operand_read(ea, 7);
               v--;
               operand_write(v & 0x00FFFFFF, ea, 7);
               break;

            case SIM:

               /**************************************************

			switch interrupt mask

			exchange the interrupt mask in PSR 22..16
			with the value in the storage operand

               **************************************************/

               v = operand_read(ea, 7);
               operand_write(((psr >> 16) & 0x7F), ea, 7);
               psr &= (0x0080FFFF);
               psr |= (v & 0x7F) << 16;
               break;

            case POPA:

               /**************************************************

			pop and add

			take a word form the internal stack
			and add it to the target word

		        25th bit of sum -> carry

               **************************************************/

	       v = operand_read(ea, 7) + operand_read(sp, 0);
               sp++;

               if (psr & 0x00800000)
               {
               }
               else
               {
                  if (sp > GUARD_RANGE_UP)
                  {
                     GUARD_INTERRUPT
                     break;
                  }
               }

               psr &= (0x00FFFFFE);
               psr |= ((v >> 24) & 1);
	       operand_write(v & 0x00FFFFFF, ea, 7);

               break;

            case SRC:

               /*************************************************

			shift right through carry

			25-bit rotate one bit position
			to the right

			carry moves into high bit of storage operand
			low bit of the storage operand moves into carry

			touch no other flags

               *************************************************/

               v = operand_read(ea, 7);
               operand_write((v >> 1) | ((psr & CARRY) << 23),
                             ea, 7);
               psr &= CARRY^0x00FFFFFF;
               psr |= v & 1;
               break;

            case SLC:

               /*************************************************

			shift left through carry

			25-bit rotate one bit position
			to the left

			carry moves into low bit of storage operand
			high bit of the storage operand moves into carry

			touch no other flags

               *************************************************/

               v = operand_read(ea, 7);
               operand_write(0x00FFFFFF & ((v << 1) | (psr & CARRY)), ea, 7);
               psr &= CARRY^0x00FFFFFF;
               psr |= (v & 0x00800000) >> 23;               
               break;

            case QS:

               /*************************************************

			quadruple store

			store the four accumulators A:B:6:7

               *************************************************/

               burst_write4(&a, ea);
               break;

            case QL:

               /*************************************************

			quadruple load

			load the four accumulators A:B:6:7

               *************************************************/

               burst_read4(&a, ea);
               break;

            case FPP:

               /*************************************************

			floating pop and pack		fpu.c

			pop a four-word floating-point value
			from the internal stack top

			compress the value to two-word format
			and store it

               *************************************************/

               fpp(ea, &_register[sp]);
	       sp += 4;			/* 4 stack words -> 2 stored words */

               if (psr & 0x00800000)
               {
               }
               else
               {
                  if (sp > GUARD_RANGE_UP)
                  {
                     GUARD_INTERRUPT
                     break;
                  }
               }

               break;

            case FPX:

               /*************************************************

			floating push and expand	fpu.c

			read a two-word compressed floating-point
			value from storage

			expand it to four-word floating-point format
			and push it on the internal stack


               *************************************************/

               sp -= 4;

               if (psr & 0x00800000)
               {
               }
               else
               {
                  if (sp < GUARD_RANGE_SP)
                  {
                     GUARD_INTERRUPT
                     break;
                  }
               }

               fpx(ea, &_register[sp]);
               break;

            case FA:

               /*************************************************

			floating add			fpu.c

			add the four-word floating-point operand
			to the floating-point value in the four
			accumulators A:B:6:7

               *************************************************/

               fa(ea);
               break;

            case FAN:

               /*************************************************

			floating add negative		fpu.c

			add the negative (i.e. subtract the value)
			of the four-word floating-point operand
         		to the floating-point value in the four
			accumulators A:B:6:7

               *************************************************/

               fan(ea);
               break;

            case FM:

               /*************************************************

			floating multiply		fpu.c

			multiply the floating-point value
			in the four accumulators A:B:6:7
			by the four-word floating-point operand

               *************************************************/

               fm(ea);
               break;

            case FD:

               /*************************************************

			floating divide			fpu.c

			divide the floating-point value
			in the four accumulators A:B:6:7
			by the four-word floating-point operand

               *************************************************/

               fd(ea);
               break;

            case QPOP:

               /*************************************************

			quadruple pop

			pop four words from the internal
			stack top

               *************************************************/


               #ifdef EDGE
               if (psr & 32768) stacktop4();
               #endif

               _p = &_register[sp];
               burst_write4(_p, ea);
               sp += 4;

               #ifdef EDGE
               if (psr & 32768) stored4(ea);
               #endif

               break;

            case QPUSH:

               /*************************************************

			quadruple push

			push four words onto the internal
			stack top

               *************************************************/

               sp -= 4;

               if (psr & 0x00800000)
               {
               }
               else
               {
                  if (sp < GUARD_RANGE_SP)
                  {
                     GUARD_INTERRUPT
                     break;
                  }
               }

               _p = &_register[sp];
               burst_read4(_p, ea);
               break;

            case EX:

               /*************************************************

			execute

			read an instruction word from storage
			and execute it

               *************************************************/

               execute(memory_read(ea));
               break;

            case DPUSH:

               /*************************************************

			double push

			push two words onto the internal
			stack top

               *************************************************/

               sp -=2;

               if (psr & 0x00800000)
               {
               }
               else
               {
                  if (sp < GUARD_RANGE_SP)
                  {
                     GUARD_INTERRUPT
                     break;
                  }
               }

               _p = &_register[sp];
               burst_read2(_p, ea);
               break;

            case LSC:

               /*************************************************

			load shift and count		alu.c

			load accumulator A from storage and shift
			left until bit 23 is not the sign polarity

			stop at 24 if all bits = sign

			shift count in accumulator B

               *************************************************/

               lsc(ea);
               break;

            case MTA:

               /*************************************************

			masked test A			alu.c

			test subtract
			storage operand AND mask _register K
			from accumulator A AND mask _register K

			skip next instruction if equal

               *************************************************/


               
               v = operand_read(ea, 7);

               if ((a & k) ^ (v & k))
               {
               }
               else apc++;

               break;

            case SC:

               /*************************************************

			store carry

			store the value if the CARRY flag
			as a data word containing zero or one

               *************************************************/

               operand_write(psr & CARRY, ea, 7);
               break;

            case MLB:

               /*************************************************

			masked load B

			accumulator B = (B AND (NOT mask _register K))
			OR (storage operand AND mask _register K)

               **************************************************/

               v = k & 0x00FFFFFF;
               b = (b & (v ^ 0x00FFFFFF)) | (operand_read(ea, 7) & v);
               break;

            case DS:

               /*************************************************

			double store

			store the two accumulators A:B

               *************************************************/

               burst_write2(&a, ea);
               break;

            case DL:

               /*************************************************

			double load

			load the two accumulators A:B

               *************************************************/

               burst_read2(&a, ea);

               #ifdef EDGE
               if (psr & 32768) q4readout();
               #endif

               break;

            case DA:

               /*************************************************

			double add			alu.c

			add the two-word integer from storage
			to accumulators A:B

			set or clear CARRY

               *************************************************/

               da(ea);
               break;

            case DAN:

               /*************************************************

			double add negative		alu.c

			add the negative (i.e. subtract the value)
			of two-word integer from storage
			to accumulators A:B

			set or clear CARRY

               *************************************************/

               dan(ea);
               break;

            case DLSC:

               /*************************************************

			double load shift and count	alu.c

			load accumulators A:B from storage and shift
			left until bit 47 is not the sign polarity

			stop at 48 if all bits = sign

			shift count in accumulator _register 6

               *************************************************/

               dlsc(ea);
               break;

		#ifdef DTAB
            case DTAB:

               /*************************************************

			double test AB			alu.c

			test-subtract the two-word storage
			operand from accumulators A:B

			set or clear CARRY MINUS ZERO PARITY ODD
			discard data result

               *************************************************/

               dtab(ea);
               break;
		#endif

            case GO:

               /***************************************************

			switch instruction base B0 according to
			bits 23..6 of the operand word loaded from
			storage

			jump to location 0..63 of the new B0 bank
			according to bits 6..0 of the operand word

               ***************************************************/

               v = operand_read(ea, 7);

               if (v & 0x00800000)
               {
                  b0_name = v >> 6;

                  /***********************************************

			system list of translate base values in B71
			for descriptor handle values  0x008xxxxx

			or user list of translate base values in TCB
                        for descriptor handle values  0x00Cxxxxx

                  ************************************************/
               }
               else
               {
                  /***********************************************

			absolute values
			32K bank on any 8-page boundary
			in the executable space = 16 gigawords
			with 8 entry vectors at the bank start

			or bank on any page boundary
			in the first 256 megawords of storage
			with 64 entry vectors

                  ************************************************/

                  if (v & 0x00400000)
                  {
                     b0_name  = v & 0x003FFFF8;
                     v &= 7;
                  }
                  else
                  {
                     b0_name  = (v & 0x003FFFC0) >> 6;
                     v &= 63;
                  }
               }

               base[0] = b0_name;
               b0p = &memory.p4k[b0_name];
               apc = &b0p->w[v];

               break;

            case CALL:

               /*********************************************************

			same as GO except offset and bank of following
			instruction are first placed in the internal
			stack where far return instruction FRET should
			find them later

              **********************************************************/

               sp -= 2;

               if (psr & 0x00800000)
               {
               }
               else
               {
                  if (sp < GUARD_RANGE_SP)
                  {
                     GUARD_INTERRUPT
                     break;
                  }
               }

               _p = &_register[sp];
               _p[1] = apc - b0p->w;
               _p[0] = b0_name;

               /***************************************************

			switch instruction base B0 according to
			bits 23..6 of the operand word loaded from
			storage

			jump to location 0..63 of the new B0 bank
			according to bits 6..0 of the operand word

               ***************************************************/

               v = operand_read(ea, 7);

               if (v & 0x00800000)
               {
                  b0_name = v >> 6;

                  /***********************************************

			system list of translate base values in B71
			for descriptor handle values  0x008xxxxx

			or user list of translate base values in TCB
                        for descriptor handle values  0x00Cxxxxx

                  ************************************************/
               }
               else
               {
                  /***********************************************

			absolute values
			32K bank on any 8-page boundary
			in the executable space = 16 gigawords
			with 8 entry vectors at the bank start

			or bank on any page boundary
			in the first 256 megawords of storage
			with 64 entry vectors

                  ************************************************/

                  if (v & 0x00400000)
                  {
                     b0_name  = v & 0x003FFFF8;
                     v &= 7;
                  }
                  else
                  {
                     b0_name  = (v & 0x003FFFC0) >> 6;
                     v &= 63;
                  }
               }

               base[0] = b0_name;
               b0p = &memory.p4k[b0_name];
               apc = &b0p->w[v];

               break;
         }
         break;

      default:
         switch (instruction.t1 & 248)
         {
            case SR:
               if (designator == I)
               {
                  /******************************************************

			INA instruction
			read IO port to accumulator A

                  *******************************************************/

                  a = 0x00FFFFFF;
                  if (ea < IO_PORTS) a = base[ea];
                  break;
               }
 
               if (designator == XI)
               {
                  /******************************************************

                        ON instruction (or PSR flags)

                  *******************************************************/

                  psr |= ea & 65535;
                  break;
               }

               /*********************************************************

			SR instruction
			store repeat _register R

               *********************************************************/

               operand_write(r, ea, designator);
               break;

            case SK:
               if (designator == I)
               {
                  /******************************************************

                        INB instruction
			read IO port to accumulator B

                  *******************************************************/

                  b = 0x00FFFFFF;
                  if (ea < IO_PORTS) b = base[ea];
                  break;
               }
               
               if (designator == XI)
               {
                  /******************************************************

                        OFF instruction (nor PSR flags)

                  *******************************************************/
               
                  psr &= (ea & 65535) ^ 0x00FFFFFF;
                  break;
               }

               /*********************************************************

			SK instruction
			store mask _register K

               *********************************************************/

               operand_write(k, ea, designator);
               break;

            case SX:
               if (designator == I)
               {
                  /******************************************************

                        OUTA instruction
			write accumulator A to IO port

                  *******************************************************/

                  if (ea < IO_PORTS) base[ea] = a;
                  if (ea == 102) indication |= a;
                  break;
               }
               
               if (designator == XI)
               {
                  /******************************************************

                        read staging _register			sr.c

			shift bits from staging _register RDATA to
			accumulator A, decrementing available data bits
			count in _register RDATAC. Read memory
			via pointer _register Q and update Q if more
			data required

                  *******************************************************/
               
                  rsr(ea);
                  break;
               }

               /********************************************************

			SX instruction
			store index _register X

               *********************************************************/

               operand_write(x, ea, designator);
               break;

            case SY:
               if (designator == I)
               {
                  /******************************************************

                        OUTB instruction
			write accumulator B to IO port

                  *******************************************************/

                  if (ea < IO_PORTS) base[ea] = b;
                  if (ea == 102) indication |= b;
                  break;
               }

               if (designator == XI)
               {
                  /******************************************************

                        write staging _register			sr.c

			shift data bits from accumulator A to staging
			_register WDATA, incrementing data bits count
			in _register WDATAC. When WDATAC reaches 24
			write WDATA to storage via pointer _register Q,
			update Q and restart WDATAC

                  *******************************************************/

                  wsr(ea);
                  break;
               }

               /*********************************************************

			SY instruction
			store index _register Y

               *********************************************************/

               operand_write(y, ea, designator);
               break;

            case SA:
               if (designator ==  I)
               {
                  /*****************************************************

			RELOAD instruction
			used to reload the base registers from
			the base table in the TCB on task switch

                  *****************************************************/

                  if (ea <  2) break;
                  if (ea > 62) break;

                  burst_read2(buffer, a);

                  if (buffer[0] & 0x00800000)
                  {
                     /**************************************************
			implement bank index translation here
                     **************************************************/
                  }

                  base[ea] = buffer[0];

                  if (buffer[1] & 0x00800000)
                  {
                     /**************************************************
			implement bank index translation here
                     **************************************************/
                  }

                  base[ea+1] = buffer[1];

                  a += 2;
                  a &= 0x00FFFFFF;
                  break;
               }

               if (designator == XI) break;

               /******************************************************

			SA instruction
			store accumulator A

               ******************************************************/

               operand_write(a, ea, designator);
               break;

            case SB:
               if (designator ==  I) usleep(ea);
               if (designator ==  I) break;
               if (designator == XI) break;

               /*********************************************************

			SB instruction
			store accumulator B

               *********************************************************/

               operand_write(b, ea, designator);
               break;

            case Z:
               if (designator == I)
               {
                  /******************************************************
                        SABR instruction
                  *******************************************************/

                  if ((ea > 1) && (ea < 64))
                  {
                     v = a;

                     if (v & 0x00400000)
                     {
                        if (v & 63)
                        {
                           /*********************************************

				device array outside system memory
				only interrupt code may base it

                           *********************************************/

                           if (psr & 0x00800000)
                           {
                           }
                           else
                           {
                              ii(XBASE_U);
                              break;
                           }
                        }
                     }

                     w = base[65] & 0x0000FFFF;

                     _w = &memory.p4k[w].w[BASE_TABLE+ea];

                     _w->t3 = v;
                     _w->t2 = v >> 8;
                     _w->t1 = v >> 16;

                     if (v & 0x00800000)
                     {
                        /************************************************
                                implementations with bank index
                                translation have that here
                        ************************************************/
                     }

                     base[ea] = v;
                  }

                  break;
               }


               if (designator == XI)
               {
                  /*****************************************************
			LRET		(local routine return)
			return within current segment
		  ******************************************************/

                  _p = &_register[sp];
                  sp++;

                  if (psr & 0x00800000)
                  {
                  }
                  else
                  {
                     if (sp == GUARD_RANGE_UP + 1)
                     {
                        EXIT_INTERRUPT;
                        break;
                     }

                     if (sp > GUARD_RANGE_UP)
                     {
                        GUARD_INTERRUPT
                        break;
                     }
                  }

                  v = *_p;
                  v += ea;
                  v &= 0x00FFFFFF;

                  if ((unsigned) v > 262143)
                  {
                     GUARD_INTERRUPT
                     break;
                  }

                  apc = &b0p->w[v];
                  break;
               }

               /*********************************************************

			Z instruction
			store value zero

               *********************************************************/

               operand_write(0, ea, designator);
               break;

            case POP:

               if (designator == I) break;

               /*******************************************************
                                        spare
               *******************************************************/



               if (designator == XI)
               {
                  /******************************************************
                        FRET instruction (far routine return)
                  *******************************************************/

                  _p = &_register[sp];
                  sp += 2;

                  if (psr & 0x00800000)
                  {
                  }
                  else
                  {
                     if (sp == GUARD_RANGE_UP + 2)
                     {
                        EXIT_INTERRUPT;
                        break;
                     }

                     if (sp > GUARD_RANGE_UP)
                     {
                        GUARD_INTERRUPT
                        break;
                     }
                  }


                  v = *_p++;

                  b0_name = v;

                  if (v & 0x00800000)
                  {
                  }
                  else
                  {
                     if (v & 0x00400000)
                     {
                        v &= 0x003FFFF8;
                     }
                     #if 0
                     else
                     {
                        v >>= 6;
                        v &= 0x0000FFFF;
                     }
                     #endif
                  }

                  base[0] = v;

                  if ((unsigned) v > (PAGES_IN_MEMORY-1))
                  {
                     GUARD_INTERRUPT
                     break;
                  }

                  b0p = &memory.p4k[v];

                  v = *_p;
                  v += ea;

                  if ((unsigned) v > 262143)
                  {
                     GUARD_INTERRUPT
                     break;
                  }

                  v &+ 0x00FFFFFF;
                  apc = &b0p->w[v];
                  break;
               }

               /***********************************************************

			POP instruction
			store word from the internal stack top

               ***********************************************************/

               _p = &_register[sp];
               sp++;

               if (psr & 0x00800000)
               {
               }
               else
               {
                  if (sp > GUARD_RANGE_UP)
                  {
                     GUARD_INTERRUPT
                     break;
                  }
               }

               operand_write(*_p, ea, designator);
               break;

            case LR:

               /**********************************************************

			load repeat _register R

               **********************************************************/

               r = operand_read(ea, designator);
               break;

            case LK:

               /*********************************************************

			load mask _register K

               *********************************************************/

               k = operand_read(ea, designator);
               break;

            case LX:

               /*********************************************************

			load index _register X

               *********************************************************/

               x = operand_read(ea, designator);
               break;

            case LY:

               /********************************************************

			load index _register Y

               ********************************************************/

               y = operand_read(ea, designator);
               break;

            case LA:

               /********************************************************

			load accumulator A

               ********************************************************/

               a = operand_read(ea, designator);
               break;

            case LB:

               /********************************************************

			load accumulator B

               ********************************************************/

               b = operand_read(ea, designator);
               break;

            case TZ:

               if (designator == I) break;

               /*******************************************************
					spare
               *******************************************************/


               if (designator == XI)
               {
                  /******************************************************
                        II instruction (initiate internal interrupt)
                  *******************************************************/

                  ii(ea);
                  break;
               }

               /************************************************

			TZ instruction

			Test Zero and Skip

               ************************************************/

               v = operand_read(ea, designator);

               if (v & 0x00FFFFFF)
               {
               }
               else apc++;

               break;

            case TP:

               if (designator == I)
               {
                  /******************************************************
                        JDZ instruction
                  *******************************************************/

                  if ((a | b) & 0x00FFFFFF)
                  {
                  }
                  else apc = &b0p->w[ea];
                  break;
               }


               if (designator == XI)
               {
                  /******************************************************
                        IR instruction. return from interrupt
                  *******************************************************/

                  _p = &_register[sp];
                  sp += 4;
                  _p++;
                  psr = *_p++;
                  v = *_p++;

                  b0_name = v;

                  if (v & 0x00800000)
                  {
                  }
                  else
                  {
                     if (v & 0x00400000)
                     {
                        v &= 0x003FFFF8;
                     }
                     else
                     {
                        #if 1
                        v &= 0x003FFFFF;
                        #else
                        v >>= 6;
                        v &= 0xFFFF;
                        #endif
                     }
                  }

                  base[0] = v;

                  b0p = &memory.p4k[v];

                  v = *_p;
                  v += ea;
                  v &= 0x00FFFFFF;
                  apc = &b0p->w[v];
                  iselect = (psr & 0x00800000) >> 16;
                  break;
               }

               /************************************************

                        Test Positive and Skip

               ************************************************/

               v = operand_read(ea, designator);

               if (v & 0x00800000)
               {
               }
               else apc++;

               break;

            case AX:

               /***********************************************

			add operand to index _register X
                        
                        psr flag CARRY is not changed

               ************************************************/

               v = x + operand_read(ea, designator);
               x = v & 0x00FFFFFF;
               break;

            case AY:
               
               /***********************************************

                        add operand to index _register Y
                        
                        psr flag CARRY is not changed

               ************************************************/

               v = y + operand_read(ea, designator); 
               y = v & 0x00FFFFFF;
               break;

            case OR:

               /*****************************************************

			OR instruction			alu.c

			or accumulator A with the operand value
                        
                        psr flag CARRY is not changed

               *****************************************************/

               or(operand_read(ea, designator));
               break;

            case ORB:

               /****************************************************

			ORB instruction			alu.c

			or accumulator B with the operand value
                        
                        psr flag CARRY is not changed

               ****************************************************/

               orb(operand_read(ea, designator));
               break;

            case AND:

               /***************************************************

			AND instruction			alu.c

			and accumulator A with the operand value
                        
                        psr flag CARRY is not changed

               ****************************************************/

               and(operand_read(ea, designator));
               break;

            case ANDB:

               /***************************************************

			ANDB instruction		alu.c

			and accumulator B with the operand value
                        
                        psr flag CARRY is not changed

               ****************************************************/

               andb(operand_read(ea, designator));
               break;

            case XOR:

               /***************************************************

			XOR instruction			alu.c

			xor accumulator A with the operand value
                        
                        psr flag CARRY is not changed

               ***************************************************/

               xor(operand_read(ea, designator));
               break;


            case XORB:

               /***************************************************

			XORB instruction		 alu.c

			xor accumulator B with the operand value

			psr flag CARRY is not changed

               ***************************************************/

               xorb(operand_read(ea, designator));
               break;


               #ifdef ANU

            case ANU:

               /**************************************************

			add negative upper		alu.c

			test-subtract the operand from accumulator A
                        
                        psr flag CARRY is updated

			the data result is stored in register 5 or 133
			accumulator B

			the instruction code assign is now xorB

			ANU is substituted with macros

				anu	[b = a - operand]
				anuba	[a = b - operand]

               **************************************************/

               anu(operand_read(ea, designator));
               break;

               #endif

            case AA:

               /*************************************************

			add to A			alu.c

			add the operand to accumulator A
                        
                        psr flag CARRY is updated

               **************************************************/

               aa(operand_read(ea, designator));
               break;

            case AB:

               /*************************************************

			add to B			alu.c

			add the operand to accumulator B
                        
                        psr flag CARRY is updated

               *************************************************/

               ab(operand_read(ea, designator));
               break;

            case ANA:

               /***********************************************************

			add negative to A		alu.c

			add the 2s complement of the operand
			to accumulator A, i.e. subtract
                        
                        psr flag CARRY is updated

               ***********************************************************/

               ana(operand_read(ea, designator));
               break;

            case ANB:

               /***********************************************************

			add negative to B		alu.c

			add the 2s complement of the operand
			to accumulator B, i.e. subtract
                        
                        psr flag CARRY is updated

               **********************************************************/

               anb(operand_read(ea, designator));
               break;

            case M:

               /**********************************************************

			multiply			alu.c

			multiply _register B by the operand
			algebraically, giving a 48-bit product
			in accumulators A:B
                        
                        psr flag CARRY is not changed

               ***********************************************************/

               m(operand_read(ea, designator));
               break;

            case MF:

               /***********************************************************

			multiply fractional		alu.c

			multiple accumulator B by the operand
			giving a 48-bit product in accumulators A:B

			B is treated as unsigned
			the operand is treated as signed
                        
                        psr flag CARRY is not changed

               ***********************************************************/

               mf(operand_read(ea, designator));
               break;

            case D:

               /***********************************************************

			divide				alu.c

			divide the 48-bit dividend in accumulators A:B
			by the operand algebraically.

			store the quotient in A and the remainder in B

			quotient high-order bits 47..24 are stored in
			register 6 MANTISSA2 (register 134 in ISR mode)

			psr flag CARRY is not changed

                **********************************************************/

               d(operand_read(ea, designator));
               break;

            case PUSH:

               /**********************************************************

			place a word on the internal stack

               **********************************************************/

               sp--;

               if (psr & 0x00800000)
               {
               }
               else
               {
                  if (sp < GUARD_RANGE_SP)
                  {
                     GUARD_INTERRUPT
                     break;
                  }
               }

               _p = &_register[sp];
               *_p = operand_read(ea, designator);
               break;
         }
   }

   #ifdef T_SLICE
   if (psr & 0x00870000)
   {
      /*****************************************************************
		not while ISR or interrupt mask
      *****************************************************************/
   }
   else
   {
      if (w = _register[REALTIME_CLOCK] & 0x00FFFFFF)
      {
         w--;
         _register[REALTIME_CLOCK] = w;

         if (w == 0)
         {
            YIELD_INTERRUPT
         }
      }
   }
   #endif
}

void rta()
{
   for (;;)
   {
      execute(*apc++);
   }
}
