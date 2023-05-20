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
#include "emulate.h"
#include "smp.h"
#include "rw.h"
#include "alu.h"
#include "../engine.rta/i64.h"

#ifdef	EDGE
#include "../rta.run/idisplay.h"
#endif

#if 0
extern int	 iselect;
extern int	 _register[];
extern int	 psr;
#endif

void gshiftr(int positions, int words, int fill, int target[])
{
   int		 offset_word = words - positions / 24 - 1;
   int		 offset_bits = positions % 24;
   int		 reverse_bits = 24 - offset_bits;

   int		 transition = fill;
   int		 carry = 0;
   int		 index = words;


   if (offset_word < 0)
   {
   }
   else transition = target[offset_word--];

   while (index--)
   {
      carry = transition >> offset_bits;
      transition = fill;
      
      if (offset_word < 0)
      {
      }
      else transition = target[offset_word--];

      carry |= transition << reverse_bits;    
      target[index] = carry & 0x00FFFFFF;
   }
}

void gshiftl(int positions, int words, int fill, int target[])
{
   int		 word_offset = positions / 24;
   int		 bits_offset = positions % 24;
   int		 reverse_bits = 24 - bits_offset;

   int		 index = 0;
   int		 transition = fill;
   int		 carry = 0;


   if (word_offset < words) transition = target[word_offset++];

   while (index < words)
   {
      carry = transition << bits_offset;
      transition = fill;
      if (word_offset < words) transition = target[word_offset++];
      carry |= transition >> reverse_bits;

      target[index++] = carry & 0x00FFFFFF;
   }
}

#if 0
void sar(int ea)	/*	shift A right	*/
{
   a = (a >> ea) & 0x00FFFFFF;
}

void sbr(int ea)	/*	shift B right	*/
{
   b = (b >> ea) & 0x00FFFFFF;
}

void dsr(int ea)	/*	double shift right	*/
{
   gshiftr(ea, 2, 0, &a);
}


void sal(int ea)	/*	shift A left		*/
{
   a = (a << ea) & 0x00FFFFFF;
}

void sbl(int ea)	/*	shift B left		*/
{
   b = (b << ea) & 0x00FFFFFF;
}

void dsl(int ea)	/*	double shift left	*/
{
   gshiftl(ea, 2, 0, &a);
}


void rar(int ea)	/*	rotate A right		*/
{
   gshiftr(ea, 1, a, &a);
}

void rbr(int ea)	/*	rotate B right		*/
{
   gshiftr(ea, 1, b, &b);
}

void drr(int ea)	/*	double rotate right	*/
{
   int		 temp[4] = { a, b, a, b } ;

   gshiftr(ea % 48, 4, 0, temp);
   a = temp[2];
   b = temp[3];
}


void ral(int ea)	/*	rotate A left		*/
{
   gshiftl(ea, 1, a, &a);
}

void rbl(int ea)	/*	rotate B left		*/
{
   gshiftl(ea, 1, b, &b);
}

void drl(int ea)	/*	double rotate left	*/
{
   int		 temp[4] = { a, b, a, b } ;

   gshiftl(ea % 48, 4, 0, temp);
   a = temp[0];
   b = temp[1];
}


void saa(int ea)	/*	shift A algebraic	*/
{
   int		 signs = 0x00FFFFFF & (0 - ((a >> 23) & 1));

   gshiftr(ea, 1, signs, &a);
}

void sba(int ea)	/*	shift B algebraic	*/
{
   int		 signs = 0x00FFFFFF & (0 - ((b >> 23) & 1));

   gshiftr(ea, 1, signs, &b);
}

void dsa(int ea)	/*	double shift algebraic	*/
{
   int		 signs = 0x00FFFFFF & (0 - ((a >> 23) & 1));

   gshiftr(ea, 2, signs, &a);
}

#endif

void lsc(int ea, smp *xcore)	/*	load shift and count	*/
{
   int		*register_set = xcore->register_set;
   int		 operand;
   int		 final_count = 0;
   int		 sign = operand & 0x00800000;
   int		 cycle = operand >> 23;


   if ((operand = operand_read(ea, 7, xcore)) < 0) return;
   
   /***********************************************
        if guard fault has happened break off and
        do not write back to interrupt registers
   ***********************************************/

   while (final_count < 24)
   {
      if ((operand ^ sign) & 0x00800000) break;
      final_count++;
      operand <<= 1;
      operand |= cycle;
   }

   a = operand & 0x00FFFFFF;
   b = final_count;
}

#if 0

void  __da(int ea, int target[])	/*	double add		*/
{
   int		 operand[2];
   int		 carry;

   if ((burst_read2(operand, ea)) < 0) return;
   
   /***********************************************
        if guard fault has happened break off and
        do not write back to interrupt reg1sters
   ***********************************************/

   #ifdef EDGE
   if (psr & 32768) b4double(operand[0], operand[1]);
   #endif
 

   carry = operand[1] + target[1];
   target[1] = carry & 0x00FFFFFF;
   carry = ((carry >> 24) & 1) + target[0] + operand[0];
   target[0] = carry & 0x00FFFFFF;
   psr = (psr & 0x00FFFFFE) | ((carry >> 24) & 1);

   #ifdef EDGE
   if (psr & 32768) q4double();
   #endif
}

void __dan(int ea, int target[])	/*	double add negative	*/
{
   int		 operand[2];
   int		 carry = 1;

   if ((burst_read2(operand, ea)) < 0) return;

   /***********************************************
        if guard fault has happened break off and
        do not write back to interrupt registers
   ***********************************************/


   #ifdef EDGE
   if (psr & 32768) b4double(operand[0], operand[1]);
   #endif
 
   operand[0] ^= 0x00FFFFFF;
   operand[1] ^= 0x00FFFFFF;

   carry = carry + operand[1] + target[1];
   target[1] = carry & 0x00FFFFFF;
   carry = ((carry >> 24) & 1) + target[0] + operand[0];
   target[0] = carry & 0x00FFFFFF;
   psr = (psr & 0x00FFFFFE) | ((carry >> 24) & 1);

   #ifdef EDGE
   if (psr & 32768) q4double();
   #endif
}
#endif

void dlsc(int ea, smp *xcore)	/* double load shift and count	*/
{
   int		*register_set = xcore->register_set;
   int		 operand[2];
   int		 final_count = 0;
   int		 sign;

   if ((burst_read2(operand, ea, xcore)) < 0) return;
   
   /***********************************************
        if guard fault has happened break off and
        do not write back to interrupt registers
   ***********************************************/
   
   sign = operand[0] & 0x00800000;

   while (final_count < 48)
   {
      if ((operand[0] ^ sign) & 0x00800000) break;
      final_count++;
      gshiftl(1, 2, sign, operand);
   }

   a         = operand[0];
   b         = operand[1];
   mantissa2 = final_count;
}

#if 0
void  __or(int ea, int designator, int *target, smp *xcore)	/*	OR with register a register	*/
{
   int operand = operand_read(ea, designator, xcore);

   if (contingency < 0) return;

   /***********************************************
        if guard fault has happened break off
        and do not write back to  registers
   ***********************************************/

   *target |= operand;
}

void __and(int ea, int designator, int *target, smp *xcore)	/*	AND with register a register	*/
{
   int operand = operand_read(ea, designator, xcore);

   if (contingency < 0) return;

   /***********************************************
        if guard fault has happened break off
        and do not write back to registers
   ***********************************************/

   *target &= operand;
}

void __xor(int ea, int designator, int *target, smp *xcore)	/*	XOR with register register	*/
{
   int operand = operand_read(ea, designator, xcore);

   if (contingency < 0) return;

   /***********************************************
        if guard fault has happened break off
        and do not write back to registers
   ***********************************************/

   *target ^= operand;
}

void  __aa(int ea, int designator, int *target, smp *xcore)	/*	add to a register		*/
{
   int		 operand = operand_read(ea, designator, xcore);
   int		 carry = *target + operand;

   if (contingency < 0) return;

   /***********************************************
        if guard fault has happened break off 
        and do not write back to registers
   ***********************************************/

   *target = carry + 0x00FFFFFF;
   psr = (psr & 0x00FFFFFE) | ((carry >> 24) & 1);
}

void __ana(int ea, int designator, int *target, smp *xcore)	/*	add negative to a register	*/
{
   int		 operand = operand_read(ea, designator, xcore);
   int		 carry = (operand ^ 0x00FFFFFF) + 1 + *target;

   if (contingency < 0) return;

   /***********************************************
        if guard fault has happened break off 
        and do not write back to registers
   ***********************************************/

   *target = carry & 0x00FFFFFF;
   psr = (psr & 0x00FFFFFE) | ((carry >> 24) & 1);
}
#endif

#if 1

/********************************************************

	this one is only used in the SIMD version
	of fixed point multiply
	it has 48-bit multiplicand and 48-bit product

********************************************************/

int __mm(int ea, int designator, int target[], smp *xcore)
{
   i64		 multiplicand;
   int		 multiplier;

   if ((multiplier = operand_read(ea, designator, xcore)) < 0) return -LP_ADDRESS;
   
   /***********************************************
        if guard fault has happened break off
        and do not write back to registers
   ***********************************************/

   multiplicand = (((i64) target[0] << 40) | ((i64) target[1] << 16)) >> 16;   
   multiplier = (multiplier << 8) >> 8;

   multiplicand = (i64) multiplicand * multiplier;

   target[1] = multiplicand & 0x00FFFFFF;
   target[0] = (multiplicand >> 24) & 0x00FFFFFF;
   return 0;
}

/*************************************************
	this one is the fixed point multiply
	in the regular accumulators
	multiplier is 24 bits in register b
	product is 48 bits in a:b
*************************************************/

int __m(int ea, int designator, int target[], smp *xcore)
{
   i64		 product;

   int		 multiplicand;
   int		 multiplier;

   if ((multiplier = operand_read(ea, designator, xcore)) < 0) return -LP_ADDRESS;
   
   /***********************************************
        if guard fault has happened break off
        and do not write back to registers
   ***********************************************/

   multiplicand = (target[1] << 8) >> 8;
   multiplier = (multiplier << 8) >> 8;

   product = (i64) multiplicand * multiplier;
   
   target[1] = product & 0x00FFFFFF;
   target[0] = (product >> 24) & 0x00FFFFFF;
   return 0;
}

/*************************************************
	this one is the short multiply used in SIMD
	instead of fractional multiply 
	both sides are signed
	multiplier and product are both 24 bits
*************************************************/

int __ms(int ea, int designator, int *target, smp *xcore)
{
   int		 multiplicand = *target;
   int		 multiplier;

   if ((multiplier = operand_read(ea, designator, xcore)) < 0) return -LP_ADDRESS;

   /***********************************************
        if guard fault has happened break off
        and do not write back to registers
   ***********************************************/

   multiplicand = (multiplicand << 8) >> 8;
   multiplier = (multiplier << 8) >> 8;
   *target = (multiplicand * multiplier) & 0x00FFFFFF;
   return 0;
}

/*************************************************
   this one is multiply fractional
   in the regular accumulators
   multiplicand is unsigned 24 bits in b
   multiplier is signed 24 bits
   product is 48 bits in a:b
*************************************************/

int __mf(int ea, int designator, int target[], smp *xcore)
{
   i64		 product;
   
   int		 multiplicand = target[1] & 0x00FFFFFF;
   int		 multiplier;

   if ((multiplier = operand_read(ea, designator, xcore)) < 0) return -LP_ADDRESS;

   /***********************************************
        if guard fault has happened break off
        and do not write back to registers
   ***********************************************/

   multiplier = (multiplier << 8) >> 8;

   product = (i64) multiplicand * multiplier;

   target[1] = product & 0x00FFFFFF;
   target[0] = (product >> 24) & 0x00FFFFFF;
   return 0;
}

/****************************************************
	this divide is used with regular accumulators
	remainder -> b
	quotient low   -> a
	quotient hhigh -> 6	
****************************************************/

int __d(int ea, int designator, int target[], smp *xcore)
{
   i64		 dividend;
   int		 divisor;

   if ((divisor = operand_read(ea, designator, xcore)) < 0) return -LP_ADDRESS;

   /***********************************************
        if guard fault has happened break off
        and do not write back to registers
   ***********************************************/

   divisor = (divisor << 8) >> 8;
   dividend = (((i64) target[0] << 40) | ((i64) target[1] << 16)) >> 16;
   target[1] = (dividend % divisor) & 0x00FFFFFF;
   dividend = (i64) dividend / divisor;

   target[0] = dividend & 0x00FFFFFF;
   target[2] = (dividend >> 24) & 0x00FFFFFF;
   return 0;
}

/***************************************************
	this divide is used in SIMD
	quotient is a word pair
	remainder is optional @ quotient + 2 
***************************************************/

int __dd(int ea, int designator, int target[], smp *xcore)
{
   i64           dividend;
   int           divisor;

   if ((divisor = operand_read(ea, designator, xcore)) < 0) return -LP_ADDRESS;

   /***********************************************
        if guard fault has happened break off
        and do not write back to registers
   ***********************************************/

   divisor = (divisor << 8) >> 8;
   dividend = (((i64) target[0] << 40) | ((i64) target[1] << 16)) >> 16;
   if (psr & FLOATING_RESIDUE) target[2] = (dividend % divisor) & 0x00FFFFFF;
   dividend = (i64) dividend / divisor;
   
   target[1] = dividend & 0x00FFFFFF;
   target[0] = (dividend >> 24) & 0x00FFFFFF;
   return 0;
}


#else

/*************************************************
	these bitrace routines have always worked
	but the multiplies and divides of the
	platform circuitry probably work faster
	and cooler
*************************************************/

void   m(int operand, smp *xcore)	/*	multiply		*/
{
   int		*register_set = xcore->register_set;

   int		 product[2] = { 0, 0 } ;
   int		 temp[2] = { b, operand };
   int		 signs = 0;
   int		 beats = 24;
   int		 carry = 0;
   
   if (temp[0] & 0x00800000)
   {
      signs = 0x00FFFFFF;
      temp[0] ^= signs;
      temp[0] += 1;
   }

   if (temp[1] & 0x00800000)
   {
      signs ^= 0x00FFFFFF;
      temp[1] ^= 0x00FFFFFF;
      temp[1] += 1;
   }

   while (beats--)
   {
      product[0] <<= 1;
      product[0] |= product[1] >> 23;
      product[1] <<= 1;
      product[1] &= 0x00FFFFFF;

      temp[1] <<= 1;

      if (temp[1] & 0x01000000)
      {
         carry = temp[0] + product[1];
         product[1] = carry & 0x00FFFFFF;
         carry >>= 24;
         product[0] += carry;
      }
   }

   if (signs)
   {
      carry = 1;
      carry += product[1] ^ 0x00FFFFFF;
      product[1] = carry;
      carry >>= 24;
      carry  &=  1;
      carry += product[0] ^ 0x00FFFFFF;
      product[0] = carry;
   }

   a = product[0] & 0x00FFFFFF;
   b = product[1] & 0x00FFFFFF;
}


/*********************************************************

	multiply fractional MF
	is as integer multiply M

	except that in MF only the multiplier is signed
        not the multiplicand

	therefore the product is the same polarity
	as the multiplier always

	and the bit at the ms position of the
	multiplicand is +factorial of the other 23 bits

*********************************************************/

void  mf(int operand, smp *xcore)	/*	multiply fractional	*/
{
   int	     *register_set = xcore->register_set;

   int		 product[2] = { 0, 0 } ;
   int		 temp[2] = { b, operand } ;
   int		 signs = 0;
   int		 beats = 24;
   int		 carry = 0;

   if (temp[1] & 0x00800000)
   {
      signs ^= 0x00FFFFFF;
      temp[1] ^= 0x00FFFFFF;
      temp[1] += 1;
   }

   while (beats--)
   {
      product[0] <<= 1;
      product[0] |= product[1] >> 23;
      product[1] <<= 1;
      product[1] &= 0x00FFFFFF;

      temp[1] <<= 1;

      if (temp[1] & 0x01000000)
      {
         carry = temp[0] + product[1];
         product[1] = carry & 0x00FFFFFF;
         carry >>= 24;
         product[0] += carry;
      }
   }

   if (signs)
   {
      carry = 1;
      carry += product[1] ^ 0x00FFFFFF;
      product[1] = carry;
      carry >>= 24;
      carry  &=  1;
      carry += product[0] ^ 0x00FFFFFF;
      product[0] = carry;
   }

   a = product[0] & 0x00FFFFFF;
   b = product[1] & 0x00FFFFFF;
}

/*******************************************************************

	divide D supports a possible quotient longer than a word
	with minimum loss of efficiency

	dividend is in A:B
	divisor is one word
	quotient is in A
	remainder is in B

	nonzero remainder has the original sign of the dividend

	fixed-point instructions are 2s-complement, so a dividend
	of either polarity may have an absolute zero remainder

	unrefined method is to place 1s complement of magnitude
	of divisor left of a word of 1s

	for example if divisor is 3, then

	_________________________________________________________________
	| 1111 1111 1111 1111 1111 1100 | 1111 1111 1111 1111 1111 1111 |
        |_______________________________|_______________________________|

	and for 24 beats, this subtrahend is shifted right one position,
	bit value 1 inserted at high order position, and trial-added + 1
	to the remainder = initially the magnitude of the 48-bit dividend

	the qotient is shifted left, and carry from the above test-add
	is copied to quotient low order bit.

	A refinement saves quotient overflow, and prevents the quotient
	from generating as 1111 1111 1111 1111 1111 1111, where the dividend
	magnitude has more significant bits than the divisor magnitude + 24

	Firstly, the 1s complement of dividend magnitude is shifted left
	algebraically until the high order bit is no longer 1, and the number
	of subtract beats is increased accordingly.

	So in the case of a divisor of +3 or -3, the number of subtract
        beats becomes 46, and the initial encoding of the subtrahend is

	_________________________________________________________________
	| 0011 1111 1111 1111 1111 1111 | 1111 1111 1111 1111 1111 1111 |
	|_______________________________|_______________________________|


	Secondly, the high-order shifts from the quotient are
	stored in register 6 (134 in interrupt mode), MANTISSA2

	Therefore MANTISSA2 contains the high-order bits 47..24 of the quotient

************************************************************************/


void   d(int operand, smp *xcore)	/*	divide			*/
{
   int		*register_set = xcore->register_set;

   int		 quotient    = 0;
   int		 dividend[2] = { a, b } ;
   int		 divisor[2]  = { operand, 0x00FFFFFF } ;

   int		 signs =  0x00FFFFFF & (0 - ((dividend[0] >> 23) & 1));
   int           signs2 = 0x00FFFFFF & (0 -  ((divisor[0] >> 23) & 1));

   int		 beats = 24;
   int		 lookaside;
   int		 carry = 1;
   int		 upper = 0;


   if (signs)
   {
      carry += dividend[1] ^ 0x00FFFFFF;
      dividend[1] = carry & 0x00FFFFFF;
      carry >>= 24;
      carry += dividend[0] ^ 0x00FFFFFF;
      dividend[0] = carry & 0x00FFFFFF;
   }

   if (signs2) divisor[0]--;
   else divisor[0] ^= 0x00FFFFFF;

   while (divisor[0] & 0x00800000)
   {
      if (divisor[0] == 0x00FFFFFF) break;
      beats++;
      divisor[0] <<= 1;
      divisor[0]  |= 1;
      divisor[0]  &= 0x00FFFFFF;
   }

   while (beats--)
   {
      upper <<= 1;
      upper |= (quotient >> 23) & 1;
      quotient <<= 1;
      quotient &= 0x00FFFFFF;

      divisor[1] >>= 1;
      divisor[1] |= (divisor[0] & 1) << 23;
      divisor[0] >>= 1;
      divisor[0] |= 0x00800000;

      lookaside = dividend[1] + divisor[1] + 1;
      carry = lookaside >> 24;
      carry += dividend[0];
      carry += divisor[0];

      if (carry & 0x01000000)
      {
         dividend[1] = lookaside & 0x00FFFFFF;
         dividend[0] =     carry & 0x00FFFFFF;
         quotient |= 1;
      }
   }

   if (signs)
   {
      /*********************************************

	nonzero remainder keeps the original
	polarity of the dividend
	so that quotient * divisor + remainder
	truly = dividend

      *********************************************/

      dividend[1] ^= signs;
      dividend[1] += 1;

      /********************************************

	now you may make the quotient sign algebraic

      ********************************************/
   }

   signs ^= signs2;

   if (signs)
   {
      quotient ^= signs;
      quotient++;

      upper ^= signs;
      upper += (quotient >> 24) & 1;
   }

   mantissa2 = upper & 0x00FFFFFF;
   a = quotient    & 0x00FFFFFF;
   b = dividend[1] & 0x00FFFFFF;
}
#endif
