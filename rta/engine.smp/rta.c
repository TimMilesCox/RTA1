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
// #define	RTA_SMP	8
#include "emulate.h"
#include "smp.h"
#include "fpu.h"
#include "rw.h"
#include "alu.h"
#include "sr.h"
#include "ii.h"
#include "../engine.rta/stack.h"
#include "operand.h"
#ifdef	LINUX
#define	_ts	__ts
#define	sabr	_sabr
#endif

#include "../rta.run/settings.h"
#include "../include.rta/argue.h"

#ifdef	EDGE
#include "../rta.run/idisplay.h"
#endif

extern device                    devices[];             /* in rw */
extern int                       register_pointer;      /* in console */

#ifdef	RTA_SMP
#define	memory			 devices[0].pages
extern int			 general_indication;
extern smp			 core[];
#else
extern unsigned int		 base[];

extern int			 iselect;
extern int			 iselectu;
extern int			 indication;

extern word			*apc;
extern word			*apcu;
extern word			*apcz;
extern word			*breakpoint;

extern page			*b0p;
extern unsigned int		 b0_name;
extern unsigned int		 psr;
extern  int			_register[];
extern unsigned int		*register_set;
extern device			*pdevice;

static unsigned			 b0_scope;
extern system_memory		 memory;
#endif

extern word			*breakpoint;
#include "ioportab.c"

extern void rex(int ea, smp *xcore);

static void oport(int ea, int value, smp *xcore)
{
   int		 rule,
                 device;

   int		 mask,
		 from_pin,
                 index;

   int		 device_index = 0;

   smp		*ascor;
   int		*register_set = xcore->register_set;

   if (ea < IO_PORTS)
   {
      rule = mask_port_outAB[ea];

      if (rule)
      {
         if (((rule & ISR_ONLY) == 0) || (psr & 0x00800000))
         {
            if (rule & EXTERNAL_IO)
            {
               base[ea] = value;
               if (ea == 102) indication |= value;

               if (ea == 75)
               {
                   base[74] &= (value & 255) ^ 255;
                   // indication |= (value & 255) << 8;
               }

//               if ((ea == 79) && (value & 1)) indication |= ATTENTION;
               return;
            }

            if (rule & BROADCAST)
            {
               mask = k & base[125];	/* selection AND existing cores	*/
               from_pin = base[73];	/* 1 << core_index		*/
               ascor = core;

               while (mask)
               {
                  if (mask & 1)
                  {
                     if (value & 0x00FFFFFF)
                     {
                        ascor->BASE[78] |= from_pin;
                        base[ea] = value;
                        ascor->INDICATION |= ATTENTION;
                     }
                     else ascor->INDICATION |= TPULSE;
                     
                  }

                  ascor++;
                  mask >>= 1;
               }

               return;
            }

            else if (rule & MEMTYPE)
            {
               if (value & 0x00400000) device_index = value & 63;

               if (device_index)
               {
                  if (value & 0x00800000)
                  {
                     if (psr & 0x00800000)
                     {
                        base[ea] = value;
                        return;
                     }

                     GUARD_AUTHORITY
                     return;
                  }
               }

               device = base[128+device_index];
           
               if ((device & 0x00C00000) == 0x00800000)
               {
                  if ((value & 0x003FFFC0) > (device & 0x003FFFFF))
                  {
                     GUARD_INTERRUPT
                     return;
                  }

                  base[ea] = value;
                  return;
               }
            }
            else if (rule & MEMZERONLY)
            {
               /*******************************************
		page pointer to system memory
               *******************************************/

               if ((value & 0x00400000) == 0)
               {
                  if (value > (base[128] & 0x003FFFFF))
                  {
                     GUARD_INTERRUPT
                     return;
                  }

                  base[ea] = value;
                  return;
               }
            }
         }
      }
   }

   GUARD_AUTHORITY;
}

/* compilation default is this	#undef	INLINE_SABR */

void execute(word instruction, smp *xcore)
{
   int		 icode = instruction.t1;
   int		 designator = icode & 7;
   int		 ea, xtag;

   int		*_p,
		*_q;

   int		 v, w, device_index;
   unsigned	 device_descriptor;
   int		 buffer[4];
   int		 signs;

   word		*_w;
   word		 x_instruction;
   int		*register_set = xcore->register_set;

   #ifdef INLINE_SABR
   page		*tcbp;
   #endif

   EFFECTIVE_ADDRESS

   switch (designator)
   {
      case 6:
         switch (icode)
         {
            case SAR:
               a = (a >> ea) & 0x00FFFFFF;	/* shift A right	*/
               break;

            case SBR:
               b = (b >> ea) & 0x00FFFFFF;	/* shift B right	*/
               break;

            case DSR:
               gshiftr(ea, 2, 0, &a);		/* double shift right	*/
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
               a = (a << ea) & 0x00FFFFFF;	/* shift A left		*/
               break;

            case SBL:
               b = (b << ea) & 0x00FFFFFF;	/* shift B left		*/
               break;

            case DSL:
               gshiftl(ea, 2, 0, &a);		/* double shift left	*/
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
               gshiftr(ea, 1, a, &a);		/* rotate A right	*/
               break;

            case RBR:
               gshiftr(ea, 1, b, &b);		/* rotate B right	*/
               break;

            case DRR:
               buffer[0] = buffer[2] = a;
               buffer[1] = buffer[3] = b;

               gshiftr(ea % 48, 4, 0, buffer);
               a = buffer[2];			/* double rotate right	*/
               b = buffer[3];
               break;

            case JNC:
						/* 	jump no carry	*/
	       if ((psr & CARRY) == 0) apc = &b0p->w[ea];
               break;

            case RAL:
               gshiftl(ea, 1, a, &a);		/* rotate A left	*/
               break;

            case RBL:
               gshiftl(ea, 1, b, &b);		/* rotate B left	*/
               break;

            case DRL:
               buffer[0] = buffer[2] = a;
               buffer[1] = buffer[3] = b;

               gshiftl(ea % 48, 4, 0, buffer);
               a = buffer[0];			/* double rotate left	*/
               b = buffer[1];
               break;

            case JC:
						/* jump carry		*/
	       if (psr & CARRY) apc = &b0p->w[ea];
               break;

            case SAA:
               signs = 0x00FFFFFF & (((int) a << 8) >> 31);
               gshiftr(ea, 1, signs, &a);
						/* shift A algebraic	*/
               break;

            case SBA:
               signs = 0x00FFFFFF & (((int )b << 8) >> 31);
	       gshiftr(ea, 1, signs, &b);
						/* shift B algebraic	*/
               break;

            case DSA:
               signs = 0x00FFFFFF & (((int) a << 8) >> 31);

               #if 0
               printf("[%p %8.8x +- %6.6x]", xcore, a, signs);
               #endif

               gshiftr(ea, 2, signs, &a);
					/* double shift algebraic	*/
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

            default:
              /*******************************************************
			try to persuade the compiler to do jump-tables
			because it does not know switch variable limit
                        although it is 0..31 with certainty
              *******************************************************/

              break;
         }
         break;

      case 7:
         switch (icode)
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


               #define SMP_TS
               #ifdef ABSOTS

               /************************************************
                  operand_read() and OPERAND_WRITE()
                  don't look at the designator argument
                  before classing EA in register range
                  so switch ABSO uses * _w = memory_hold(ea)
                  to get a pointer for TS instruction
                  and that is one less call to operand_etc()

                  of course TS target is never in range 0..255
               ************************************************/

	       OPERANDH(_w, ea)

               #ifdef SMP_TS
               /***********************************************
		  real multicore emulating real RTA_SMP
		  1 emulator thread -> 1 emulated core
		  real update under lock implements ts
		  note ea is a 24-bit value
                       _w is a platform-size * pointer
               ***********************************************/

               v = _ts(_w);

               if (v & 0x8000)
               {
                  #if 0
                  printf("[%x]", v);
                  #endif
               }
               else apc++;

	       #else

               v = _w->t1;
               v ^= 128;

               if (v & 128)
               {
                  _w->t1 = v;
                  apc++;
               }

               #endif

               #else

               OPERAND(v, ea, 7)

               v ^= 0x00800000;

               if (v & 0x00800000)
               {
                  OPERAND_WRITE(v, ea, 7);
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

               OPERAND(v, ea, 7)
               v ^= 0x00FFFFFF;
               OPERAND_WRITE(v, ea, 7);
               break;

            case INC:

               /***********************************************

			increment

			touch no flags

               ***********************************************/

               OPERAND(v, ea, 7)
               v++;
               OPERAND_WRITE(v & 0x00FFFFFF, ea, 7);
               break;

            case DEC:

               /**************************************************

			decrement

			touch no flags

               **************************************************/

               OPERAND(v, ea, 7)

               v--;
               OPERAND_WRITE(v & 0x00FFFFFF, ea, 7);
               break;

            case SIM:

               /**************************************************

			switch interrupt mask

			exchange the interrupt mask in PSR 22..16
			with the value in the storage operand

               **************************************************/

               OPERAND(v, ea, 7)
               OPERAND_WRITE(((psr >> 16) & 0x7F), ea, 7);
               psr = (psr & 0x0080FFFF) | ((v & 0x7F) << 16);
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
               OPERAND(w, ea, 7)
               w += *_p;

               v++;
               *_q = v;

               psr = (psr & 0x00FFFFFE) | ((w >> 24) & 1);
               OPERAND_WRITE(w & 0x00FFFFFF, ea, 7);

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

               OPERAND(v, ea, 7)
               OPERAND_WRITE((v >> 1) | ((psr & CARRY) << 23),
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

               OPERAND(v, ea, 7)
               OPERAND_WRITE(0x00FFFFFF & ((v << 1) | (psr & CARRY)), ea, 7);
               psr &= CARRY^0x00FFFFFF;
               psr |= (v & 0x00800000) >> 23;               
               break;

            case QS:

               /*************************************************

			quadruple store

			store the four accumulators A:B:6:7

               *************************************************/

               BURST_WRITE4(&a, ea);
               break;

            case QL:

               /*************************************************

			quadruple load

			load the four accumulators A:B:6:7

               *************************************************/

               BURST_READ4(&a, ea);
               break;

            #ifdef FPP
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
            #endif	/*	FPP	*/	

            #ifdef FPX
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
            #endif	/*	FPX	*/

            case DTE:

               /*************************************************

                        double test subtract [ a b ] - operand
                        skip equal

               *************************************************/

               BURST_READ2(buffer, ea)

               if ((a ^ buffer[0]) | (b ^ buffer[1]))
               {
               }
               else apc++;

               break;

            case DPOP:

               /*************************************************

                        double pop

                        pop two words from the internal
                        stack top

                        decrement sp before burst write
                        in case the popped objects include sp

               *************************************************/

               STACK_READ(2);
               _p = _register + v;
               *_q = v + 2;

               BURST_WRITE2(_p, ea);
               break;

            case FA:

               /*************************************************

			floating add			fpu.c

			add the four-word floating-point operand
			to the floating-point value in the four
			accumulators A:B:6:7

               *************************************************/

               __fa(ea, &a, xcore);
               break;

            case FAN:

               /*************************************************

			floating add negative		fpu.c

			add the negative (i.e. subtract the value)
			of the four-word floating-point operand
         		to the floating-point value in the four
			accumulators A:B:6:7

               *************************************************/

               __fan(ea, &a, xcore);
               break;

            case FM:

               /*************************************************

			floating multiply		fpu.c

			multiply the floating-point value
			in the four accumulators A:B:6:7
			by the four-word floating-point operand

               *************************************************/

               __fm(ea, &a, xcore);
               break;

            case FD:

               /*************************************************

			floating divide			fpu.c

			divide the floating-point value
			in the four accumulators A:B:6:7
			by the four-word floating-point operand

               *************************************************/

               __fd(ea, &a, xcore);
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

               BURST_WRITE4(_p, ea);
               break;

            case QPUSH:

               /*************************************************

			quadruple push

			push four words onto the internal
			stack top

			acquire operands before decrementing sp
			in case the pushed objects include sp

			sp at *_q is the same one before and after
			even if a fault occurs in BURST_READ

               *************************************************/

               STACK(4)
               v -= 4;
               _p = _register + v;
               BURST_READ4(_p, ea)
               *_q = v;

               break;

            case EX:

               /*************************************************

			execute

			read an instruction word from storage
			and execute it

               *************************************************/

               x_instruction = memory_read(ea, xcore);
               execute(x_instruction, xcore);
               break;

            case DPUSH:

               /*************************************************

			double push

			push two words onto the internal
			stack top

                        acquire operands before decrementing sp
                        in case the pushed objects include sp

                        sp at *_q is the same one before and after
                        even if a fault occurs in BURST_READ

               *************************************************/

               #if 0
               printf("[%x %x %x]\n", base[77], register_set - _register, sp, iselect);
               #endif

               STACK(2)
               v -= 2;
               _p = _register + v;
               BURST_READ2(_p, ea)
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

               lsc(ea, xcore);
               break;

            case MTA:

               /*************************************************

			masked test A			alu.c

			test subtract
			storage operand AND mask _register K
			from accumulator A AND mask _register K

			skip next instruction if equal

               *************************************************/


               
               OPERAND(v, ea, 7)

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

               OPERAND_WRITE(psr & CARRY, ea, 7);
               break;

            case MLB:

               /*************************************************

			masked load B

			accumulator B = (B AND (NOT mask _register K))
			OR (storage operand AND mask _register K)

               **************************************************/

               OPERAND(v, ea, 7)
               b = ((b & (k ^ 0x00FFFFFF)) | (v & k)) & 0x00FFFFFF;
               break;

            case DS:

               /*************************************************

			double store

			store the two accumulators A:B

               *************************************************/

               BURST_WRITE2(&a, ea);
               break;

            case DL:

               /*************************************************

			double load

			load the two accumulators A:B

               *************************************************/

               BURST_READ2(&a, ea);

               #ifdef EDGE
               #error YOU ARE NOT HERE
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

               BURST_READ2(buffer, ea)

               buffer[1] += b;
               b = buffer[1] & 0x00FFFFFF;
               buffer[0] = buffer[0] + a + ((buffer[1] >> 24) & 1);
               a = buffer[0] & 0x00FFFFFF;
               psr = (psr & 0x00FFFFFE) | ((buffer[0] >> 24) & 1);
               break;

            case DAN:

               /*************************************************

			double add negative		alu.c

			add the negative (i.e. subtract the value)
			of two-word integer from storage
			to accumulators A:B

			set or clear CARRY

               *************************************************/

               BURST_READ2(buffer, ea)

               buffer[1] ^= 0x00FFFFFF;
               buffer[0] ^= 0x00FFFFFF;
               buffer[1] = buffer[1] + b + 1;
               b = buffer[1] & 0x00FFFFFF;
               buffer[0] = buffer[0] + a + ((buffer[1] >> 24) & 1);
               a = buffer[0] & 0x00FFFFFF;
               psr = (psr & 0x00FFFFFE) | ((buffer[0] >> 24) & 1);
               break;

            case DLSC:

               /*************************************************

			double load shift and count	alu.c

			load accumulators A:B from storage and shift
			left until bit 47 is not the sign polarity

			stop at 48 if all bits = sign

			shift count in accumulator _register 6

               *************************************************/

               dlsc(ea, xcore);
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

               OPERAND(b0_name, ea, 7)

               if (b0_name & 0x00800000)
               {
                  /************************************************
			read the gate
                  ************************************************/

                  BURST_READ2(buffer, b0_name & 0x007FFFFF)
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
               b0p = memory + v;
               apc = &b0p->w[w];

               #ifdef INSTRUCTION_U
               base[INSTRUCTION_U] = b0_scope;
               apcz = &memory[b0_scope].w[4095];
               #endif

               break;

            default:
              /*******************************************************
                        try to persuade the compiler to do jump-tables
                        because it does not know switch variable limit
                        although it is 0..31 with certainty
              *******************************************************/
         
              break;
         }
         break;

      default:

         icode &= 248;

         switch (icode)
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

               OPERAND_WRITE(r, ea, designator);
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

               OPERAND_WRITE(k, ea, designator);
               break;

            case SX:
               if (designator == I)
               {
                  /******************************************************

                        OUTA instruction
			write accumulator A to IO port

                  *******************************************************/

                  #if 1
                  oport(ea, a, xcore);
                  #else
                  if (ea < IO_PORTS) base[ea] = a;
                  if (ea == 102) indication |= a;
                  #endif

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
               
                  rsr(ea, xcore);
                  break;
               }

               /********************************************************

			SX instruction
			store index _register X

               *********************************************************/

               OPERAND_WRITE(x, ea, designator);
               break;

            case SY:
               if (designator == I)
               {
                  /******************************************************

                        OUTB instruction
			write accumulator B to IO port

                  *******************************************************/

                  #if 1
                  oport(ea, b, xcore);
                  #else
                  if (ea < IO_PORTS) base[ea] = b;
                  if (ea == 102) indication |= b;
                  #endif

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

                  wsr(ea, xcore);
                  break;
               }

               /*********************************************************

			SY instruction
			store index _register Y

               *********************************************************/

               OPERAND_WRITE(y, ea, designator);
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

                  BURST_READ2(buffer, a)

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

               OPERAND_WRITE(a, ea, designator);
               break;

            case SB:
               if (designator ==  I) break;
               if (designator == XI) break;

               /*********************************************************

			SB instruction
			store accumulator B

               *********************************************************/

               OPERAND_WRITE(b, ea, designator);
               break;

            case Z:
               if (designator == I)
               {
                  /******************************************************
                        SABR instruction
                  *******************************************************/

                  #ifndef INLINE_SABR

                  extern int csabr(int ea, smp *xcore);
                  w = csabr(ea, xcore);

                  if (w < 0)
                  {
                     GUARD_AUTHORITY;
                  }

                  #else			/*	INLINE_SABR defined	*/

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

                              if ((v & 0x003FFFC0) 
                              >   (device_descriptor & 0x003FFFC0)) v = 0x00C00001;

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
                                 >   (device_descriptor & 65535)) v = 0x00C00001;
                              }
                              else
                              {
                                 /**********************************
                                    whereas applications may not
                                    too easy to do inadvertantly
                                 **********************************/

                                 GUARD_AUTHORITY
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
                           covers register a accidentally zero
                        ***********************************************/

                        v = 0x00C00001;
                     }
                     else if ((v & 0x003FFFFF) < pages_in_memory)
                     {
                        /***********************************************
                           you're fine

			   PAGES_IN_MEMORY was programmed to redefine
			   to pages_in_memory but somehow did not

			   pages_in_memory is a variable when <- malloc

			   PAGES_IN_MEMORY is an immediate value
			   when file _memory is assembled / compiled

			   this is gcc and uses malloc & variable size
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

                     tcbp = memory + base[65];

                     #if 1
                     _w = tcbp->w + BASE_TABLE + ea;
                     _w->t3 = v;
                     _w->t2 = v >>  8;
                     _w->t1 = v >> 16;
                     _w->parity =   0;
                     #else
                     store24(tcbp->i + BASE_TABLE + ea, v);
                     #endif

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

                     GUARD_AUTHORITY
                  }

                  #endif	/*	INLINE_SABR or not	*/

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

               OPERAND_WRITE(0, ea, designator);
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
                  b0p = memory + b0_scope;
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
                  apcz = memory[b0_scope].w + 4095;
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
               OPERAND_WRITE(*_p, ea, designator);

               break;

            case LR:

               /**********************************************************

			load repeat _register R

               **********************************************************/

               OPERAND(v, ea, designator)
               r = v;
               break;

            case LK:

               /*********************************************************

			load mask _register K

               *********************************************************/

               OPERAND(v, ea, designator)
               k = v;
               break;

            case LX:

               /*********************************************************

			load index _register X

               *********************************************************/

               OPERAND(v, ea, designator)
               x = v;
               break;

            case LY:

               /********************************************************

			load index _register Y

               ********************************************************/

               OPERAND(v, ea, designator)
               y = v;
               break;

            case LA:

               /********************************************************

			load accumulator A

               ********************************************************/

               // _register[280] = ea;
               OPERAND(v, ea, designator)
               // _register[281] = v;
               a = v;
               break;

            case LB:

               /********************************************************

			load accumulator B

               ********************************************************/

               OPERAND(v, ea, designator)
               b = v;
               break;

            case TZ:

               if (designator == I)
               {
                  /*****************************************************
                                repeat execute instruction
                  *****************************************************/

                  rex(ea, xcore);
                  break;
               }

               if (designator == XI)
               {
                  /******************************************************
                        II instruction (initiate internal interrupt)
                  *******************************************************/

                  /******************************************************
			derive page # ea
			ea is sign-extended but then &= 0x00FFFFFF
                  ******************************************************/

                  v = ea >> 6;

                  if (((v == 0) && ((psr & 0x00800000) == 0))
                  ||  (v >= base[72]))	/*	application threshold	*/
                  {
                     /*********************************************
                        system call ii for applications
                        are not in the restart page
                     *********************************************/

                     GUARD_AUTHORITY;
                  }
                  else ii(ea, 0, xcore);
                  break;
               }

               /************************************************

			TZ instruction

			Test Zero and Skip

               ************************************************/

               OPERAND(v, ea, designator)

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
                  b0p = &memory[w];

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
                  apcz = &memory[b0_scope].w[4095];
                  #endif

                  w = _p[3];
                  w += ea;
                  w &= 0x00FFFFFF;
                  apc = &b0p->w[w];	/* update the program counter	*/

	          iselect = (psr & 0x00800000) >> 16;
		  xcore->register_set = _register + iselect;

                  #if 0
                  printf("[*I %x]\n", iselect);
                  #endif

                  break;
               }

               /************************************************

                        Test Positive and Skip

               ************************************************/

               OPERAND(v, ea, designator)

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

               OPERAND(v, ea, designator)
               x = (x + v) & 0x00FFFFFF;
               break;

            case AY:
               
               /***********************************************

                        add operand to index _register Y
                        
                        psr flag CARRY is not changed

               ************************************************/

               OPERAND(v, ea, designator)
               y = (y + v) & 0x00FFFFFF;
               break;

            case OR:

               /*****************************************************

			OR instruction			alu.c

			or accumulator A with the operand value
                        
                        psr flag CARRY is not changed

               *****************************************************/

               OPERAND(v, ea, designator)
               a = a | v;
               break;

            case ORB:

               /****************************************************

			ORB instruction			alu.c

			or accumulator B with the operand value
                        
                        psr flag CARRY is not changed

               ****************************************************/

               OPERAND(v, ea, designator)
               b = b | v;
               break;

            case AND:

               /***************************************************

			AND instruction			alu.c

			and accumulator A with the operand value
                        
                        psr flag CARRY is not changed

               ****************************************************/

               OPERAND(v, ea, designator)
               a = a & v;
               break;

            case ANDB:

               /***************************************************

			ANDB instruction		alu.c

			and accumulator B with the operand value
                        
                        psr flag CARRY is not changed

               ****************************************************/

               OPERAND(v, ea, designator)
               b = b & v;
               break;

            case XOR:

               /***************************************************

			XOR instruction			alu.c

			xor accumulator A with the operand value
                        
                        psr flag CARRY is not changed

               ***************************************************/

               OPERAND(v, ea, designator)
               a = a ^ v;
               break;


            case XORB:

               /***************************************************

			XORB instruction		 alu.c

			xor accumulator B with the operand value

			psr flag CARRY is not changed

               ***************************************************/

               OPERAND(v, ea, designator)
               b = b ^ v;
               break;

            case AA:

               /*************************************************

			add to A			alu.c

			add the operand to accumulator A
                        
                        psr flag CARRY is updated

               **************************************************/

               OPERAND(v, ea, designator)
               v += a;
               a = v & 0x00FFFFFF;
               psr = (psr & 0x00FFFFFE) | ((v >> 24) & 1);
               break;

            case AB:

               /*************************************************

			add to B			alu.c

			add the operand to accumulator B
                        
                        psr flag CARRY is updated

               *************************************************/

               OPERAND(v, ea, designator)
               v += b;
               b = v & 0x00FFFFFF;
               psr = (psr & 0x00FFFFFE) | ((v >> 24) & 1);
               break;

            case ANA:

               /***********************************************************

			add negative to A		alu.c

			add the 2s complement of the operand
			to accumulator A, i.e. subtract
                        
                        psr flag CARRY is updated

               ***********************************************************/

               OPERAND(v, ea, designator)
               v = (v ^ 0x00FFFFFF) + 1 + a;
               a = v & 0x00FFFFFF;
               psr = (psr & 0x00FFFFFE) | ((v >> 24) & 1);
               break;

            case ANB:

               /***********************************************************

			add negative to B		alu.c

			add the 2s complement of the operand
			to accumulator B, i.e. subtract
                        
                        psr flag CARRY is updated

               **********************************************************/

               OPERAND(v, ea, designator)
               v = (v ^ 0x00FFFFFF) + 1 + b;
               b = v & 0x00FFFFFF;
               psr = (psr & 0x00FFFFFE) | ((v >> 24) & 1);
               break;

            case M:

               /**********************************************************

			multiply			alu.c

			multiply _register B by the operand
			algebraically, giving a 48-bit product
			in accumulators A:B
                        
                        psr flag CARRY is not changed

               ***********************************************************/

               __m(ea, designator, &a, xcore);
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

               __mf(ea, designator, &a, xcore);
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

               __d(ea, designator, &a, xcore);
               break;

            case PUSH:

               /**********************************************************

			place a word on the internal stack

                        acquire operand before decrementing sp
                        in case the pushed object is sp

                        sp at *_q is the same one before and after
                        even if a fault occurs in BURST_READ

               **********************************************************/

               STACK(1)
               v--;
               _p = _register + v;
               OPERAND(*_p, ea, designator)

               *_q = v;

               break;

            default:
              /*******************************************************
                        try to persuade the compiler to do jump-tables
                        because it does not know switch variable limit
                        although it is 0..31 with certainty
              *******************************************************/
         
              break;
         }
   }

   #ifdef GCC_CC

   /*	otherwise LP_TSLICE is actioned in leloup()	*/

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
      if ((icount = _register[REALTIME_CLOCK] & 0x00FFFFFF))
      {
         icount--;
         _register[REALTIME_CLOCK] = icount;
         if (!icount) ii(YIELD, LP_TSLICE);
      }
   }

   #endif	/*	LP_TSLICE	*/
   #endif 	/*	GCC_CC		*/
}

void leloup(smp *xcore)
{
   int		 temp;
   word		*tempo;

   for (;;)
   {
      if (apc > apcz) GUARD_INTERRUPT;

      execute(*apc++, xcore);

      #ifdef RATIO
      #elif METRIC
      metric++
      #endif

      #ifdef LP_TSLICE
      if ((temp = _register[REALTIME_CLOCK] & 0x00FFFFFF))
      {
         if (psr & 0x00870000)
         {
         }
         else
         {
            temp--;
	    if ((_register[REALTIME_CLOCK] = temp & 0x00FFFFFF) == 0) 
            {
               TIMESLICE_INTERRRUPT;
            }
         }
      }
      #endif

      if ((general_indication & BREAKPOINT) && (apc == breakpoint))
      {
         #if 1
         if (flag['g'-'a']) printf("[*B %x]\n", base[77]);
         #endif

         general_indication |= LOCKSTEP;
         indication |= LOCKSTEP;
         flag['s'-'a'] = 1;
         break;;
      }

      #ifdef RATIO
      temp = _register[283] - 1;
      _register[283] = temp;

      if (temp < 0)
      {
         indication |= TIME_UPDATE;
      }
      #endif

      #if 0
      {
         /**********************************************************************
		just one core obtains dayclock millisecond time
		and broadcasts it to all the others
		each core holds current millisecond time
		in interrupt registers DAYCLOCK$U : DAYCLOCK$
         **********************************************************************/

//         temp = _register[283] - 1;
         if (_register[283] == 0)
         /* if ((_register[283] = temp & 0x00FFFFFF) == 0) */  indication |= TIME_UPDATE;
      }
      #endif

      if (indication & INDICATIONS) break;

      if (indication & EXTERNAL_INDICATIONS)
      {
         // if (flag['h'-'a']) printf("[%x <- %p]\n", indication, xcore);

         if (psr & 0x0070000)
         {
            // if (flag['g'-'a']) printf("[*N %x:%x]\n", indication, base[77]);
         }
         else
         {
             xi(xcore);
             indication &= 127;
         }

         /*************************************************************************
	   xi() function works through all potential external indications in 1 pass
           so it does not need to loop here
         *************************************************************************/
      }

//      if (indication & INDICATIONS) break;
   }
}
