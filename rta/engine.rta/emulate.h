/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is part of the architecture-and-instruction-subsection
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



/*************************************************************************

	implementation note, this emulation of RTA1

	There is no attempt to emulate any cache algorithm.
	This is an emulation of the architecture only

	and the eventual product range is intended to have
	initially implementations without cache

	_registers are modelled with integers
	the integers are 32 bits, but they model 24-bit _registers

	there are 256 _registers at the start of the address space

	non-addressable 24-bit PSR also modelled with a 32-bit integer

	72 IO-addressable 24-bit relocation pointers
		may be retrieved with instructions INA INB
		may be written   with instructions OUTA OUTB

		applications program the relocation pointers 2..63
		with instruction SABR which writes through to TCB savearea
		as well as to the IO-mapped relocation _registers

		context switch programs the relocation pointers 2..63
		with RELOAD instruction which does not write to TCB
		but reads TCB and writes the IO-mapped relocation _registers.

		RELOAD instruction is only used during the loadup beat
		context switch. Each RELOAD bases two relocation values
		which the SABR instruction has earlier based and simultaneously
		stored through to the TCB

		SABR/RELOAD may write direct values to pointers 2..63
		or values translated from an application list or a system list.
		This translation feature is only  essential if the target
		needs to reposition application storage, and is
		not part of every implementation of the architecture.

		Whether the symbolic relocation base value is identical
		to the base address value it represents or an index value
		for translation depends on its range. Bit 23 of the
		symbolic value indicates that the value is an index
		into a translation list.

		Whether SABR/RELOAD are used to program translated
		or absolute relocation values:

		The value which SABR stores through to the TCB savarea is
		the unchanged symbolic value in accumulator A

		The value SABR/RELOAD writes to the I/O  mapped
		relocation register is an absolute value

		INA INB OUTA OUTB are concerned with direct values only.

		the 72 * 24-bit IO-addressable pointers are modelled with
		72 * pointers to structure page (  4096 * 24-bit word)
                or   pointers to structure bank (262144 * 24-bit word)
		or   pointers to interface storage (262144 * 16-bit word)

		therefore input-output instructions with IO-locations 0..71
		are implemented with translates between pointer and integer.

	Registers are modelled with 32-bit integers. These integers
	are held and processed emulator-platform-way round, although
	they represent _registers, which are canonical order on any target.

	The LS 24 bits of the integers model RTA-target _registers.
	Bit 24 models carry, which is transferred to PSR bit 0
	after fixed point adds AA AB ANA ANB ANU DA DAN POPA:

	add to accumulator A, add to accumulator B, add negative to A,
	add negative to B, add negative upper to A writing B,
	double length add to AB, double length add negative to AB

	Other than the 25-bit single-position rotate instructions
	SRC (shift storage word right through carry), 
	SLC (shift storage word left through carry), and
	and the instructions ON and OFF (turn named PSR bits on/off),
	no instructions other than AA AN ANA ANB ANU DA DAN POPA write PSR
	CARRY bit 0, and there are no other condition bits.

	CARRY is tested with the JC and JNC instructions or captured
	as an integer with the SC, store carry, instruction

	The integer result of ANU, add negative upper to A, is written
	to the other accumulator B and tested  with the JZB, jump zero B,
	instruction, the JPB, jump positive B, JNZB, jump nonzero B,
	and JNB jump negative B instructions.

	The results of AA AB ANA ANB DA and DAN instructions are
	in the acculumators A and B. The accumulators themselves
	are tested with the instructions JZA JZB JNZA JNZB JPA JPB
	JNA JNB JAO (jump if the LS bit of A is set) JPO (jump
	if the logical AND-sum of A and mask _register K has odd
	parity) and JDZ (jump if accumulators AB contain 48 zero bits)

	These conditional jump instructions which read the status
	of the accumulators directly are also the method for testing
	the result of boolean instructions OR ORB AND ANDB XOR

	index _register add instructions AX and AY do not raise any
	result condition, and neither do storage increment/decrement
	instructions INC and DEC. A decremented/incremented storage
	word or _register may be examined with TZ, test zero and skip,
	or TP, test positive and skip instructions.

	The application and interrupt lists of general _registers
	are in the first 256 words of the address space and may be
	the right-side operand of any instruction which has a
	storage operand.

	The non-addressable program-counter is modelled with a pointer
	to structure word. Its saved value is modeled with base - pointer
	yielding an integer value.

	Storage locations are modelled with the following structure word

****************************************************************************/

#define	IO_PORTS	192
#define	BASE_TABLE	64

#define	BANKS_IN_DEVICE			2

#if	(BANKS_IN_DEVICE)
#define	PAGES_IN_MEMORY			64 * BANKS_IN_DEVICE
#else
#define	PAGES_IN_MEMORY			32
#endif

#define	WORDS_IN_MEMORY			4096 * PAGES_IN_MEMORY

#define	BANKS_IN_DEVICE16		1

typedef struct { unsigned char  parity,	t1,
					t2,
					t3; } word;

typedef struct { unsigned char		t1,
					t2,
					t3; } msw;

typedef struct { unsigned char    octet[6]; } dmsw;

typedef struct { word		   w[4096]; } page;

typedef struct { word		 w[262144]; } bank;

typedef struct { msw		 w[262144]; } fsbank;

typedef union  { word   array[4096 * PAGES_IN_MEMORY];
                 page	p4k[PAGES_IN_MEMORY]; } system_memory;

typedef union  { msw	array[262144 * BANKS_IN_DEVICE];
                 fsbank   b[BANKS_IN_DEVICE]; } device24;


/**************************************************************************

	dual ported interface memory is modelled with the word16 structure
	on the system side it reads out as zero-padded 24 bits
	and writes lower 16-bits back, upper 8 bits truncated

***************************************************************************/

#define	DEVICE	32768
#define	DATA16	1

typedef struct { unsigned char	      left,
				     right; } word16;

typedef struct { word16		 w[262144]; } bank16;

typedef union  { word16 array[262144 * BANKS_IN_DEVICE16];
		 bank16	b[BANKS_IN_DEVICE16]; } device16;



typedef struct { unsigned short flags,		 banks;
                 union  { system_memory    *pages;
                          device24         *dev24;
                          device16         *dev16; } s; } device;


#define W0	0

#define T1	1
#define T2	2
#define T3	3

#define H1	T2
#define H2	T3

#define I	4
#define XI	5

#define	SHIFT_JUMP 6
#define WORD_MULTIWORD 7

#define R	0
#define K       1
#define X       2
#define Y       3
#define A       4
#define B       5

#define P       12
#define Q       13
#define FP      14
#define SP      15

#define RDATAC  20
#define RDATA   21
#define WDATAC  22
#define WDATA   23


#define r	_register[iselect | R]
#define k	_register[iselect | K]
#define x	_register[iselect | X]
#define y	_register[iselect | Y]
#define a	_register[iselect | A]
#define b	_register[iselect | B]

#define mantissa2 _register[iselect | 6]
#define mantissa3 _register[iselect | 7]

#define p	_register[iselect | P]
#define q	_register[iselect | Q]
#define fp	_register[iselect | FP]
#define sp	_register[iselect | SP]


/**********************************************************************

	RDATAC	RTDATA
	WDATAC	WDATA

	are not duplicated in the interrupt register stack

	their places 148..151 are taken by clock and timer registers

	ISRs are not likely to do bit-stream programming, but if
	they do, they must save RDATAC RDATA WDATAC WDATA first

**********************************************************************/


#define rdatac	_register[RDATAC]
#define rdata	_register[RDATA]
#define wdatac	_register[WDATAC]
#define wdata	_register[WDATA]

#define SEE1	16384
#define SEE2    8192
#define SEE3    4096
#define SEE4    2048
#define SEE5    1024
#define SEE6    512
#define SEE7    256

#define HALFWORD 128
#define BSTREAMZR 64
#define BSTREAM16R 32
#define	BSTREAM16W 16

#define CARRY	1

#define	ADP	1	/* automatic data page */

#define TCB	65	/* task control block  */
#define LIST2	66	/* page and bank translating list */




/***********************************************************************

	the effective address is either sign-extended-immediate XI

	or it is direct, instruction bits 14..0 unsigned

        or it is indirect via a location in the first 8192 words
	of the address space:
        	   via any _register 0..255
		or via w[256..4095] of the instruction stream  B0
		or via w[  0..4095] of the automatic data page B1
		which is at addresses 4096..8191 of the current address space
		or instead when PSR flag SEE$1 is asserted
		   via w[  0..4095] of the task control block  B65
		which is at addresses 4096..8191 of the current address space

	or is is base displacement
		where base is	X	_register 2	or ISR _register 130
				Y	_register 3	or ISR _register 131
				A	_register 4	or ISR _register 132
				B	_register 5	or ISR _register 133
				FP	_register 14	or ISR _register 142
				SP	_register 15	or ISR _register 143
		and displacement is instruction bits 11..0 unsigned

**************************************************************************/


#define EFFECTIVE_ADDRESS				\
							\
ea = (instruction.t2 << 8) | instruction.t3;		\
							\
if (designator == XI)					\
{							\
   ea -= (ea & 32768) << 1;				\
}							\
else							\
{							\
   if (ea & 32768)					\
   {							\
      if (ea < 0x0000a000)				\
      {							\
         ea = operand_read(ea & 0x00001FFF, 0);		\
      }							\
      else						\
      {							\
         xtag = (ea >> 12) & 7;				\
         ea &= 4095;					\
							\
         switch (xtag)					\
         {						\
            case 2:					\
               ea += x;					\
               break;					\
            case 3:					\
               ea += y;					\
               break;					\
            case 4:					\
               ea += a;					\
               break;					\
            case 5:					\
               ea += b;					\
               break;					\
            case 6:					\
               ea += fp;				\
               break;					\
            case 7:					\
               ea += sp;				\
               break;					\
         }						\
      }							\
   }							\
}							\
							\
ea &= 0x00FFFFFF;


#define	GUARD_INTERRUPT	ii(XBASE_U);
#define	EXIT_INTERRUPT	ii(II_TXIT);

#define	GUARD_RANGE_SP	24
#define	GUARD_RANGE_UP	128


/********************************************************

	instruction codes

********************************************************/

#define	SR	0
#define	SK	8
#define	SX	16
#define SY	24
#define	SA	32
#define	SB	40
#define	Z	48
#define	POP	56

#define	LR	64
#define	LK	72
#define	LX	80
#define	LY	88
#define	LA	96
#define	LB	104
#define	TZ	112
#define	TP	120

#define	AX	128
#define	AY	136
#define	OR	144
#define	ORB	152
#define	AND	160
#define	ANDB	168
#define	XOR	176
#define	XORB	184

#undef	ANU	184

#define AA	192
#define	AB	200
#define	ANA	208
#define	ANB	216
#define M	224
#define MF	232
#define	D	240
#define	PUSH	248

#define INA	SR+I
#define	INB	SK+I
#define	OUTA	SX+I
#define	OUTB	SY+I
#define RELOAD	SA+I

#define SABR	Z+I


#define	ON	SR+XI
#define	OFF	SK+XI

#define	RSR	SX+XI
#define	WSR	SY+XI
#define	LRET	Z+XI
#define	FRET	POP+XI

#define	II	TZ+XI
#define	IR	TP+XI

#define	SAR	0+6
#define	SBR	8+6
#define	DSR	16+6
#define	JDR	24+6
#define	SAL	32+6
#define	SBL	40+6
#define	DSL	48+6
#define	LCAL	56+6

#define	RAR	64+6
#define	RBR	72+6
#define	DRR	80+6
#define	JNC	88+6
#define	RAL	96+6
#define	RBL	104+6
#define	DRL	112+6
#define	JC	120+6

#define	SAA	128+6
#define	SBA	136+6
#define	DSA	144+6
#define	JAO	152+6
#define	JPA	160+6
#define	JPB	168+6
#define	J	176+6
#define	JPO	184+6

#define	JZA	192+6
#define	JZB	200+6
#define	JNZA	208+6
#define	JNZB	216+6
#define	JNA	224+6
#define	JNB	232+6
#define	JXGE	240+6
#define	JYGE	248+6

#define	TS	0+7
#define	N	8+7
#define	INC	16+7
#define	DEC	24+7
#define	SIM	32+7
#define	POPA	40+7
#define	SRC	48+7
#define	SLC	56+7

#define	QS	64+7
#define	QL	72+7
#define	FPP	80+7
#define	FPX	88+7
#define	FA	96+7
#define	FAN	104+7
#define	FM	112+7
#define	FD	120+7

#define	QPOP	128+7
#define	QPUSH	136+7
#define	EX	144+7
#define	DPUSH	152+7
#define	LSC	160+7
#define	MTA	168+7
#define	SC	176+7
#define	MLB	184+7

#define	DS	192+7
#define	DL	200+7
#define	DA	208+7
#define	DAN	216+7
#define	DLSC	224+7

#define	GO	240+7
#define	CALL	248+7

