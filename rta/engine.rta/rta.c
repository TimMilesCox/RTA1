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
#include "stack.h"

#include "../rta.run/settings.h"

#ifdef	EDGE
#include "../rta.run/idisplay.h"
#endif

extern int			 indication;

extern system_memory		 memory;
#define ROM_PAGE		 &memory.p4k[0].w[0];
extern device			 devices[];

int		 psr = 0x00800000;
word		*apc = ROM_PAGE;

#ifdef	INSTRUCTION_U
word		*apcz = &memory.p4k[0].w[4095];
#endif

int		 iselect = 128;
int		 contingency;
int		 b0_name;
unsigned	 b0_scope;
page		*b0p = memory.p4k;

int		 _register[REGISTERS];

unsigned	*register_set = _register + 128;

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

				SYSMEM_FLAG | (PAGES_IN_MEMORY-1),
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),

                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } ;


void execute(word instruction)
{
   int		 designator = instruction.t1 & 7;
   int		 ea, xtag;

   int		*_p,
		*_q;

   int		 v, w, device_index;
   unsigned	 device_descriptor;
   int		 buffer[4];
   device	*devicep;

   word		*_w;


   contingency = 0;		/* no guard fault this far this instruction */

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

               STACK(1)
               v--;
               _p = _register + v;
               *_p = apc - b0p->w;
               *_q = v;

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

			increment sp after operand read
                        and before operand write

			*_q -> is the same sp even before and after
                        even if operand read faults

               **************************************************/

               STACK_READ(1)
               _p = _register + v;
               w = *_p;
               w += operand_read(ea, 7);

               v++;
               *_q = v;

               psr &= (0x00FFFFFE);
               psr |= ((w >> 24) & 1);
               operand_write(w & 0x00FFFFFF, ea, 7);

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

                        stepping 06.06.2019
                        alter the same sp you had
                        before doing operand read with ea
                        because if operand read excepts
                        incremented sp becomes interrupt sp

               *************************************************/

	       /* 4 stack words -> 2 stored words */

               STACK_READ(2)
               _p = _register + v;
               fpp(ea, _p);
               v += 4;
               *_q = v;

               break;

            case FPX:

               /*************************************************

			floating push and expand	fpu.c

			read a two-word compressed floating-point
			value from storage

			expand it to four-word floating-point format
			and push it on the internal stack

                        stepping 06.06.2019
                        alter the same sp you had
                        before doing operand read with ea
                        because if operand read excepts
                        incremented sp becomes interrupt sp

               *************************************************/

               STACK(4)
               v -= 4;
               _p = _register + v;
               fpx(ea, _p);
               *_q = v;

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

			decrement sp before burst write
			in case the popped objects include sp

               *************************************************/


               STACK_READ(4)
               _p = _register + v;
               *_q = v + 4;

               burst_write4(_p, ea);
               break;

            case QPUSH:

               /*************************************************

			quadruple push

			push four words onto the internal
			stack top

			acquire operands before decrementing sp
			in case the pushed objects include sp

			sp at *_q is the same one before and after
			even if a fault occurs in burst_read

               *************************************************/

               STACK(4)
               v -= 4;
               _p = _register + v;
               burst_read4(_p, ea);
               *_q = v;

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

                        acquire operands before decrementing sp
                        in case the pushed objects include sp

                        sp at *_q is the same one before and after
                        even if a fault occurs in burst_read

               *************************************************/

               STACK(2)
               v -= 2;
               _p = _register + v;
               burst_read2(_p, ea);
               *_q = v;

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

            case CALL:

               /*********************************************************

			same as GO except offset and bank of following
			instruction are first placed in the internal
			stack where far return instruction FRET should
			find them later

               **********************************************************/

               STACK(2)
               v -= 2;
               _p = _register + v;
               _p[1] = apc - b0p->w;
               _p[0] = b0_name;
               *_q = v;

            case GO:

               /***************************************************

			switch instruction base B0 according to
			bits 23..6 of the operand word loaded from
			storage

			jump to location 0..63 of the new B0 bank
			according to bits 6..0 of the operand word

               ***************************************************/

               b0_name = operand_read(ea, 7);

               if (b0_name & 0x00800000)
               {
                  /************************************************
			read the gate
                  ************************************************/

                  burst_read2(buffer, b0_name & 0x007FFFFF);
                  b0_name = buffer[1];
                  if (buffer[0] & 0x00FC0000) b0_name |= 0x00800000;
                  b0_scope = b0_name & 0x003FFFFF;
                  v = b0_scope;
                  b0_scope += buffer[0] >> 18;
                  w = buffer[0] & 262143; 
               }
               else
               {
                  b0_name &= 0x003FFFFF;	/* bit 22 is reserved */
                  w = b0_name & 63;
                  b0_name >>= 6;
                  v = b0_name;
                  b0_scope = v;
               }

               if (b0_scope > (PAGES_IN_MEMORY - 1))
               {
                  GUARD_INTERRUPT
                  break;
               }

               if  (psr & 0x00800000)
               {
               }
               else if (v < base[72])
               {
                  /*****************************************
                     must be an ISR to jump in there
                  *****************************************/

                  GUARD_AUTHORITY
                  break;
               }

               base[0] = v;
               b0p = &memory.p4k[v];
               apc = &b0p->w[w];

               #ifdef INSTRUCTION_U
               base[INSTRUCTION_U] = b0_scope;
               apcz = &memory.p4k[b0_scope].w[4095];
               #endif

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

                     if ((v & 0x00400000) && (device_index = v & 63))
                     {
                        /*********************************************
                           yes that is intended to be an assignment

                           device array outside system memory
                           only interrupt code may base it
                           unless it is array memory
                           with bus behaviour like system memory
                        *********************************************/

                        /********************************************
                           allowed for interrupt code but check
                        ********************************************/

                        #ifdef CHECK_ON_BASE
                        device_descriptor = base[128 + device_index];
                              
                        switch (device_descriptor & 0x00C00000)
                        {
                           case SYSMEM_FLAG:
                              /*************************************
				applications and ISRs
				may both base array memory
                              *************************************/

                              if (((v & 0x003FFFC0) | 63)
                              >   (device_descriptor & 0x003FFFFF)) v = 0x00C00001;

                              break;

                           case DATA16_FLAG:
                           case FSYS24_FLAG:

                              if (psr & 0x00800000)
                              {
                                 /**********************************
                                     ISRs may base peripheral arrays
                                     if the block is in range
                                 **********************************/

                                 if (((v >> 6) & 65535)
                                 >   (base[128 + device_index] & 65535)) v = 0x00C00001;
                              }
                              else
                              {
                                 /**********************************
                                    whereas applications may not
                                    too easy to do inadvertantly
                                 **********************************/

                                 ii(XBASE_U, LP_AUTHORITY);
                                 return;
                              }

                              break;

                           default:
                              /**********************************
				flag value 00 void device
                              **********************************/

                              v = 0x00C00001;
                        }
                        #endif
                     }

                     #ifdef CHECK_ON_BASE
                     else if ((v & 0x003FFFFF) < base[72])
                     {
                        /***********************************************
                           no-one pulls up the restart and ISR pages
                           into their operand space
                           not even if register a was accidentally zero

                           don't drop through
                        ***********************************************/

                        ii(XBASE_U, LP_AUTHORITY);
                        return;
                     }
                     else if ((v & 0x003FFFFF) < PAGES_IN_MEMORY)
                     {
                        /***********************************************
                           you're fine
                        ***********************************************/
                     }
                     else
                     {
                        /***********************************************
                           no such block of storage
                        ***********************************************/

                        v = 0x00C00001;
                     }
                     #endif

                     w = base[65] & 0x0000FFFF;

                     _w = &memory.p4k[w].w[BASE_TABLE+ea];

                     _w->t3 = v;
                     _w->t2 = v >> 8;
                     _w->t1 = v >> 16;

                     #if 0
                     if (v & 0x00800000)
                     {
                        /************************************************
                                implementations with bank index
                                translation have that here
                        ************************************************/
                     }
                     #endif

                     base[ea] = v;
                  }
                  else
                  {
                     /**************************************************
				window tag < 2 or > 63
                     **************************************************/

                     ii(XBASE_U, LP_AUTHORITY);
                  }

                  break;
               }


               if (designator == XI)
               {
                  /*****************************************************
			LRET		(local routine return)
			return within current segment
		  ******************************************************/

                  STACK_RETURN(1)
                  _p = _register + v;
                  w = *_p;
                  w += ea;
                  w &= 0x00FFFFFF;
                  apc = &b0p->w[w];
                  v++;
                  *_q = v;

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

                        stepping 06.06.2019
                        alter the sp which you had before reading memory
                        because if the read gets a guard exception
                        sp is then name of the interrupt sp [iselect | SP]
                  *******************************************************/

                  STACK_RETURN(2)

                  _p = _register + v;
          
                  /***************************************************************
                     sp is updated immediately
                     authority or memory fault can raise after sp increment
                                     and before FRET instruction completion
                     this is in line with versions written in machine code
                  ***************************************************************/

                  v += 2;
                  *_q = v;

                  b0_name = *_p;

                  w = *(_p + 1);
                  w += ea;

                  base[0] = b0_scope = b0_name & 0x003FFFFF;
                  b0_name = *_p;
                  b0p = &memory.p4k[b0_scope];
                  apc = &b0p->w[w];

                  if (b0_name & 0x00800000)
                  {
                     b0_scope += b0p->w[64].t1 >> 2;
                  }

                  if (b0_scope > (PAGES_IN_MEMORY - 1))
                  {
                     GUARD_INTERRUPT
                     break;
                  }

                  if  (psr & 0x00800000)
                  {
                  }
                  else if (base[0] < base[72])
                  {
                     /*****************************************
                        must be an ISR to jump in there
                     *****************************************/

                     GUARD_AUTHORITY
                     break;
                  }

                  #ifdef INSTRUCTION_U
                  base[INSTRUCTION_U] = b0_scope;
                  apcz = &memory.p4k[b0_scope].w[4095];
                  #endif

                  break;
               }

               /***********************************************************

			POP instruction
			store word from the internal stack top

			increment sp before operand write
			in case the popped object is sp

               ***********************************************************/

               STACK_READ(1)
               _p = _register + v;
               *_q = v + 1;
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

                  ii(ea, 0);
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

                  if (((a) | (b)) & 0x00FFFFFF)
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

                  if (psr & 0x00800000)
                  {
                  }
                  else
                  {
                     /***************************************************
                        application issued ir instruction
                        not allowed
                     ***************************************************/

                     GUARD_AUTHORITY
                     break;
                  }

                  STACK_READ(4)
                  _p = _register + v;

                  v += 4;               /* update interrupt stack pointer */
                  *_q = v;		/* immediately in line with	  */
					/* machine code written emulators */

                  /* latent parameter *sp not used on IR */

                  psr = _p[1];
                  b0_name = _p[2];
                  b0_scope = w = b0_name & 0x003FFFFF;
                  b0p = &memory.p4k[w];

                  if (b0_name & 0x00800000)
                  {
                     b0_scope += b0p->w[64].t1 >> 2;
                  }

                  if (b0_scope > (PAGES_IN_MEMORY - 1))
                  {
                     GUARD_INTERRUPT
                     break;
                  }

                  if  (psr & 0x00800000)
                  {
                  }
                  else if (w < base[72])
                  {
                     /****************************************
                        returning PSR is an application
                        must be an ISR to jump in there
                     *****************************************/

                     GUARD_AUTHORITY
                     break;
                  }

                  base[0] = w;

                  #ifdef INSTRUCTION_U	/* update the program counter limit */
                  base[INSTRUCTION_U] = b0_scope;
                  apcz = &memory.p4k[b0_scope].w[4095];
                  #endif

                  w = _p[3];
                  w += ea;
                  w &= 0x00FFFFFF;
                  apc = &b0p->w[w];	/* update the program counter	*/

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

                        acquire operand before decrementing sp
                        in case the pushed object is sp

                        sp at *_q is the same one before and after
                        even if a fault occurs in burst_read

               **********************************************************/

               STACK(1)
               v--;
               _p = _register + v;
              *_p = operand_read(ea, designator);
              *_q = v;

               break;
         }
   }

   #ifdef LP_TSLICE
   #define icount w
   if (psr & 0x00870000)
   {
      /***************************************************
             not during interrupt
             or with interrupt lock
      ***************************************************/
   }
   else
   {
      if (icount = _register[REALTIME_CLOCK] & 0x00FFFFFF)
      {
         icount--;
         _register[REALTIME_CLOCK] = icount;
         if (!icount) ii(YIELD, LP_TSLICE);
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
