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
#include "rw.h"
#include "alu.h"

#ifdef	EDGE
#include "../rta.run/idisplay.h"
#endif

extern int	 iselect;
extern int	 _register[];
extern int	 psr;

static void gshiftr(int positions, int words, int fill, int target[])
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

static void gshiftl(int positions, int words, int fill, int target[])
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


void lsc(int ea)	/*	load shift and count	*/
{
   int		 operand = operand_read(ea, 7);
   int		 final_count = 0;
   int		 sign = operand & 0x00800000;
   int		 cycle = operand >> 23;

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

void  da(int ea)	/*	double add		*/
{
   int		 operand[2];
   int		 carry;

   burst_read2(operand, ea);

   #ifdef EDGE
   if (psr & 32768) b4double(operand[0], operand[1]);
   #endif
 
   carry = operand[1] + b;
   b = carry & 0x00FFFFFF;
   carry >>= 24;
   carry += a;
   carry += operand[0];
   a = carry & 0x00FFFFFF;
   carry >>= 24;
   psr &= 0x00FFFFFE;
   psr |= carry;

   #ifdef EDGE
   if (psr & 32768) q4double();
   #endif
}

void dan(int ea)	/*	double add negative	*/
{
   int		 operand[2];
   int		 carry = 1;

   burst_read2(operand, ea);

   #ifdef EDGE
   if (psr & 32768) b4double(operand[0], operand[1]);
   #endif
 
   operand[0] ^= 0x00FFFFFF;
   operand[1] ^= 0x00FFFFFF;

   carry += b;
   carry += operand[1];
   b = carry & 0x00FFFFFF;
   carry >>= 24;
   carry += a;
   carry += operand[0];
   a = carry & 0x00FFFFFF;
   carry >>= 24;
   psr &= 0x00FFFFFE;
   psr |= carry;

   #ifdef EDGE
   if (psr & 32768) q4double();
   #endif
}

void dlsc(int ea)	/* double load shift and count	*/
{
   int		 operand[2];
   int		 final_count = 0;
   int		 sign;

   burst_read2(operand, ea);
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


void  or(int operand)	/*	OR with register A	*/
{
   a |= operand;
}

void orb(int operand)	/*	OR with register B	*/
{
   b |= operand;
}

void and(int operand)	/*	AND with register A	*/
{
   a &= operand;
}

void andb(int operand)	/*	AND with register B	*/
{
   b &= operand;
}

void xor(int operand)	/*	XOR with register A	*/
{
   a ^= operand;
}

void xorb(int operand)	/*	XOR with register B	*/
{
   b ^= operand;
}

#ifdef	ANU

/*********************************************************

	substituted with macros

	anu[b = a - operand] and anuba[a = b - operand]

*********************************************************/

void  anu(int operand)	/*	test subtract from A	*/
			/*	result in B		*/
{
   int		 carry = operand ^ 0x00FFFFFF;

   carry += 1;
   carry += a;

   b = carry & 0x00FFFFFF;
   carry >>= 24;
   psr &= 0x00FFFFFE;
   psr |= carry;
}

#endif

void  aa(int operand)	/*	add to A		*/
{
   int		 carry = a + operand;

   a = carry & 0x00FFFFFF;
   carry >>= 24;
   psr &= 0x00FFFFFE;
   psr |= carry;
}

void  ab(int operand)	/*	add to B		*/
{
   int		 carry = b + operand;

   b = carry & 0x00FFFFFF;
   carry >>= 24;
   psr &= 0x00FFFFFE;
   psr |= carry;
}

void ana(int operand)	/*	add negative to A	*/
{
   int		 carry = operand ^ 0x00FFFFFF;

   carry += 1;
   carry += a;
   a = carry & 0x00FFFFFF;

   carry >>= 24;
   psr &= 0x00FFFFFE;
   psr |= carry;
}

void anb(int operand)	/*	add negative to B	*/
{
   int		 carry = operand ^ 0x00FFFFFF;

   carry += 1;
   carry += b;
   b = carry & 0x00FFFFFF;

   carry >>= 24;
   psr &= 0x00FFFFFE;
   psr |= carry;
}

void   m(int operand)	/*	multiply		*/
{
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

void  mf(int operand)	/*	multiply fractional	*/
{
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


void   d(int operand)	/*	divide			*/
{
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

