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
#include "fpu.h"

#define	GUARD_BITS	_register[128+19]
			/*	default 0x00C00000	*/

#define	GUARD_BITS	0x00C00000
#undef	TRACE

extern int			 iselect;
extern int			 _register[256];

/******************************************************************

	these two shifts are for 1 bit position only
	with a 1-bit sign or zero fill

******************************************************************/


static void sright(int insert_bit, int words, int data[])
{
   int			 carry = insert_bit & 1,
			 index = 0;

   while (index < words)
   {
      carry <<= 24;
      carry |= data[index];
      data[index] = carry >> 1;
      carry &= 1;
      index++;
   }
}

static void sleft(int insert_bit, int words, int data[])
{
   int			 carry = insert_bit & 1,
			 index = words;

   while (index--)
   {
      carry |= data[index] << 1;
      data[index] = carry & 0x00FFFFFF;
      carry >>= 24;
   }
}

static int add(int words, int to[], int from[])
{
   int		 carry = 0;
   int		 index = words;

   while (index--)
   {
      carry += to[index];
      carry += from[index];
      to[index] = carry & 0x00FFFFFF;
      carry >>= 24;
   }
   return carry;
}



/************************************************************

	this three-address add is used in trial subtracts for
	floating divide

	it does not appear like subtraction because the data2[]
	input is the negative magnitude sliding divisor

	startvalue initialises carry for the 1s complement
	add, turning it into a 2s complement add

	the calling divide routine copies the giving[]
	stream back to the running remainder if carry
	is returned

	the calling divide routine rotates carry whether 0 or 1
	into the quotient

************************************************************/


static int addcarry(int startvalue, int words,
                    int data1[], int data2[], int giving[])
{
   int		 carry = startvalue;
   int		 index = words;

   while (index--)
   {
      carry += data1[index];
      carry += data2[index];
      giving[index] = carry & 0x00FFFFFF;
      carry >>= 24;
   }

   return carry;
}


/************************************************************

	add_bias is the shift+add at the centre of
	add_ones

	add_ones executes both floating add and floating
	add negative

	add_ones places the number with greater scale on
	the left of the addition and calls add_bias
	indicating the characteristic difference with
	parameter bias

	both add strings have a complete word of signs
	at the front

*************************************************************/


static int add_bias(int bias, int left[], int biased_addend[])
{
   /*****************************************************

	bias is now garaunteed 0..+72

	bit string right[] must be shifted right that much

	the source of right[] is biased_addend[], which is
	not yet biased, but still normalised

	both input strings left[] and biased_addend[] have
	a word of signs at the start. The characteristics
	are not represented.

	bias contains the magnitude difference between
	the characteristics. It is a positive integer.
	The string left[] is of higher magnitude, and the
	string biased_addend[] is biased rightwards into
	the working string right[]

	the functional result is an alteration to scale
	-1 or 0 or +1

	word[0] of the resulting number is 24
	resulting sign bits

   *****************************************************/

   int			 scale = 0;
   int			 carry = biased_addend[0];
   int			 word_offset = bias / 24 + 1;
   int			 bits_offset = bias % 24;
   int			 leading_bits = 24 - bits_offset;
   int			 transition = biased_addend[1];
   int			 signs = left[0];
   int			 to = 0;
   int			 from = 2;

   int			 right[7];


   if (bias > 80) return 0;


   while (to < word_offset) right[to++] = carry;

   for (;;)
   {
      carry <<= leading_bits;
      carry |= transition >> bits_offset;
      carry &= 0x00FFFFFF;
      right[to] = carry;
      if (to > 3) break;
      to++;
      carry = transition;
      transition = biased_addend[from++];
   }

   /**********************************************************

	carry now contains the same value as word 4 of
	the biased string

	subscript [to] contains the value 4

	the add to target string left[] now takes place

   **********************************************************/


   for (;;)
   {
      carry += left[to];
      left[to] = carry & 0x00FFFFFF;
      to--;
      if (to < 0) break;
      carry >>= 24;
      carry += right[to];
   }

   if ((left[0] ^ signs) & 0x00800000)
   {
      /******************************************************

	the polarity of the result is flipped

	necessary to add 1 to a +mantissa
	          or signs to a -mantissa

      ******************************************************/

      signs ^= 0x00FFFFFF;

      carry = signs;
      if (!carry) carry = 1;
      carry += left[3];
      left[3] = carry & 0x00FFFFFF;
      carry >>= 24;
      carry += left[2];
      carry += signs;
      left[2] = carry & 0x00FFFFFF;
      carry >>= 24;
      carry += left[1];
      carry += signs;
      left[1] = carry & 0x00FFFFFF;
   }

   if ((left[0] ^ signs) & 1)
   {
      /*******************************************************

	a non-sign has carried from mantissa bit 71
        shift right, characteristic+

      *******************************************************/

      sright(signs, 4, left);
      scale = 1;
   }
   else
   {
      while (((left[1] ^ signs) & 0x800000) == 0)
      {
         /****************************************************

		mantissa bit 71 has become a sign bit
		shift left, characteristic-

         ****************************************************/

         sleft(signs, 4, left);
         scale--;
         if (scale < -71) break;
      }
   }

   left[0] = signs;
   return scale;
}

/******************************************************************

	ones complement adder does Floating Add
	and Floating Add Negative

	for ADD, direction is zero

	for ADD NEGATIVE, direction is 0x00FFFFFF
	and the right side operand is flipped on
	acquisition

******************************************************************/


static void ones_add(int ea, int direction)
{
   int		 normalised_addend[5];
   int		 biased_addend[5];

   int		 magnitude_characteristic_difference;

   int		 signs,
                 signs_right,
                 characteristic,
                 carry;

   burst_read4(biased_addend, ea);

   biased_addend[0] ^= direction;
   biased_addend[1] ^= direction;
   biased_addend[2] ^= direction;
   biased_addend[3] ^= direction;

   signs       = 0x00FFFFFF & (0 - ((a >> 23) & 1));
   signs_right = 0x00FFFFFF & (0 - ((biased_addend[0] >> 23) & 1));

   if ((biased_addend[1] ^ signs_right) & 0x00800000)
   {
      if ((b ^ signs) & 0x00800000)
      {
         magnitude_characteristic_difference = ((a & 0x00FFFFFF) ^ signs)
                              - ((biased_addend[0] & 0x00FFFFFF) ^ signs_right);

         if (magnitude_characteristic_difference < 0)
         {
            #if 0
            if (magnitude_characteristic_difference < -72) return;
            #endif

            normalised_addend[0] = biased_addend[0];
            normalised_addend[1] = biased_addend[1];
            normalised_addend[2] = biased_addend[2];
            normalised_addend[3] = biased_addend[3];
            normalised_addend[4] = signs_right ^ GUARD_BITS;
            
            biased_addend[0] = signs;
            biased_addend[1] = b;
            biased_addend[2] = mantissa2;
            biased_addend[3] = mantissa3;
            biased_addend[4] = signs;

            magnitude_characteristic_difference =
               0 - magnitude_characteristic_difference;

            signs = signs_right;
         }
         else
         {
            #if 0
            if (magnitude_characteristic_difference > 72) return;
            #endif

            normalised_addend[0] = a;
            normalised_addend[1] = b;
            normalised_addend[2] = mantissa2;
            normalised_addend[3] = mantissa3;
            normalised_addend[4] = signs ^ GUARD_BITS;

            biased_addend[4] = signs_right;
            biased_addend[0] = signs_right;
         }

         characteristic = normalised_addend[0] ^ signs;

         normalised_addend[0] = signs;

         #ifdef TRACE
         printf("%6.6X:%6.6X:%6.6X:%6.6X:%6.6X+%6.6X:%6.6X:%6.6X:%6.6X:%6.6X:=\n",
                normalised_addend[0], normalised_addend[1],
                normalised_addend[2], normalised_addend[3],
                normalised_addend[4], biased_addend[0],
                biased_addend[1],     biased_addend[2],
                biased_addend[3],     biased_addend[4]);
         #endif

         carry = add_bias(magnitude_characteristic_difference,
                          normalised_addend,
                          biased_addend);

         characteristic += carry;
         if (carry < -71) characteristic = 0x00400000;

         /******************************************************

		carry is the change to scale
		which is -1 or 0 or +1

		the sign of the result is in bits 95..72
		it may have flipped

		however if it did, mantissa bits 71..0 are
		already correct polarity

         ******************************************************/


         a         = characteristic ^ normalised_addend[0];
         b         = normalised_addend[1];
         mantissa2 = normalised_addend[2];
         mantissa3 = normalised_addend[3];
      }
      else
      {
         /*************************************************************

		the starboard operand is normalised but the
		accumulator operand A:B:6:7 is not

		therefore the accumulator operand is zero
		and the starboard operand is copied to the
		result register A:B:6:7

         *************************************************************/

         a         = biased_addend[0];
         b         = biased_addend[1];
         mantissa2 = biased_addend[2];
         mantissa3 = biased_addend[3];
      }
      #ifdef TRACE
      printf("%6.6X:%6.6X:%6.6X:%6.6X\n", a, b, mantissa2, mantissa3);
      #endif
   }

   /*******************************************************************

	do nothing at all if starboard operand is not normalised
	unnormalised numbers are zero
	so the portside operand in accumulator A:B:6:7 is not changed

   *******************************************************************/
}


void fa(int ea)
{
   ones_add(ea, 0);
}

void fan(int ea)
{
   ones_add(ea, 0x00FFFFFF);
}

void fm(int ea)
{
   static int	 around[4] = { 0, 0, 0, GUARD_BITS } ;

   int		 result[7] = { 0, 0, 0, 0, 0, 0, 0 } ;
   int		 addend[7] = { a, b, mantissa2, mantissa3, 0, 0, 0 } ;

   int		 multiplier[4] = { 0, 0, 0, 0 } ;

   int		 index1, index2, shift, characteristic;


   burst_read4(multiplier, ea);

 
   if (addend[0] & 0x00800000)
   {
      result[0] ^= 0x00FFFFFF;
      addend[0] ^= 0x00FFFFFF;
      addend[1] ^= 0x00FFFFFF;
      addend[2] ^= 0x00FFFFFF;
      addend[3] ^= 0x00FFFFFF;
   }

   if (multiplier[0] & 0x00800000)
   {
      result[0]     ^= 0x00FFFFFF;
      multiplier[0] ^= 0x00FFFFFF;
      multiplier[1] ^= 0x00FFFFFF;
      multiplier[2] ^= 0x00FFFFFF;
      multiplier[3] ^= 0x00FFFFFF;
   }

   /************************************************************

	any number not normalised is zero in this architecture
	therefore if the normalising bit is absent in either
	the multiplicand or the multiplier, the algebraic result
	of absolute zero is already in the workspace and only
	needs copying back to accumulator registers A:B:6:7

   ************************************************************/


   if (addend[1] & multiplier[1] & 0x00800000)
   {
      for (index1 = 1; index1 < 4; index1++)
      {
         shift = multiplier[index1];
         index2 = 24;

         while (index2--)
         {
            /*********************************************************

		the addend is always shifted before the add
		the final result never overflows but may underflow
		by one bit position
		Then the characteristic will be decremented
		and the mantissa normalised with a 1-position shift

           **********************************************************/

            sright(0, 6, &addend[1]);
            if (shift & 0x00800000) add(6, &result[1], &addend[1]);
            shift <<= 1;
         }
      }

      /****************************************************************

	now calculate the new midpoint characteristic

      ****************************************************************/

      characteristic = (addend[0]     & 0x007FFFFF)
                     + (multiplier[0] & 0x007FFFFF)
                     -                  0x00400000;

      if (result[1] & 0x00800000)
      {
         /************************************************************

		all those adds carried up to the normalising bit

         ************************************************************/
      }
      else
      {
         /***********************************************************

		no they didn't

         ***********************************************************/

         sleft(0, 4, &result[1]);
         characteristic--;
      }

      /**************************************************************
		because of the conditional shift left just above
		the result string could not be primed with guard
		bits, so they are added afterwards
      **************************************************************/

      shift = add(4, result + 1, around);
      characteristic += shift;			/*      1 or 0     */ 
      if (shift) sright(shift, 3, result + 1); 

      if (characteristic & 0xFF800000)
      {
         /************************************************************

		exponent has underflowed or overflowed

         ************************************************************/

         characteristic = 0x007FFFFF;
      }

      /***************************************************************

	now sign the result and store it in the accumulator

      ***************************************************************/

      result[1] ^= result[0];
      result[2] ^= result[0];
      result[3] ^= result[0];
      result[0] ^= characteristic;
   }

   a = result[0];
   b = result[1];
   mantissa2 = result[2];
   mantissa3 = result[3];
}

#if 1
void fd(int ea)
{
   int		 remainder[7] = { a, b, mantissa2, mantissa3,
                                  GUARD_BITS, 0, 0 } ;

   int		 divisor[7]   = { 0, 0, 0, 0,
				  0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF } ;

   int		 lookaside[6];

   int		 reciprocal[4] = { 0, 0, 0, 0 } ;

   int		 signs = 0,
		 signs1 = 0,
		 beats = 72,
                 carry;

   burst_read4(divisor, ea);

   if (divisor[0] & 0x00800000) signs = 0x00FFFFFF;
   else
   {
      divisor[1] ^= 0x00FFFFFF;
      divisor[2] ^= 0x00FFFFFF;
      divisor[3] ^= 0x00FFFFFF;
   }

   divisor[0] ^= signs;

   if (remainder[0] & 0x00800000) signs1 = 0x00FFFFFF;

   remainder[0] ^= signs1;
   remainder[1] ^= signs1;
   remainder[2] ^= signs1;
   remainder[3] ^= signs1;

   signs1 ^= signs;

   while(beats--)
   {
      carry = addcarry(1, 6, remainder + 1, divisor + 1, lookaside);

      if (carry)
      {
         remainder[1] = lookaside[0];
         remainder[2] = lookaside[1];
         remainder[3] = lookaside[2];

         #if 0
         remainder[4] = lookaside[3];
         remainder[5] = lookaside[4];
         remainder[6] = lookaside[5];
         #endif
      }

      sleft(carry, 3, reciprocal + 1);
      sright(1, 6, divisor + 1);
   }

   reciprocal[0] = remainder[0] - divisor[0] + 0x00400001;

   if (reciprocal[1] & 0x00800000)
   {
   }
   else
   {
      beats = 72;
      while (beats--)
      {
         sleft(0, 3, reciprocal + 1);
         reciprocal[0]--;
         if (reciprocal[1] & 0x00800000) break;
      }
   }

   a = reciprocal[0] ^ signs1;
   b = reciprocal[1] ^ signs1;
   mantissa2 = reciprocal[2] ^ signs1;
   mantissa3 = reciprocal[3] ^ signs1;
}
#else
void fd(int ea)
{
   int		 signs = 0x00FFFFFF & (0 - ((a >> 23) & 1));
   int		 dividend[7] = { a ^ signs,
                                 b ^ signs,
                                 mantissa2 ^ signs,
                                 mantissa3 ^ signs,
                                 GUARD_BITS, 0, 0 } ;

   int		 signs2;
   int		 divisor[7]  = { 0, 0, 0, 0,
                                 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF } ;

   int		 carry;
   int		 beats = 72;
   int		 characteristic;

   int		 quotient[3] = { 0, 0, 0 } ;
   int		 lookaside[6];


   burst_read4(divisor, ea);
   signs2 = 0x00FFFFFF & (0 - ((divisor[0] >> 23) & 1));

   signs ^= signs2;
   divisor[0] ^= signs2;

   if (!signs2)
   {
      divisor[1] ^= 0x00FFFFFF;
      divisor[2] ^= 0x00FFFFFF;
      divisor[3] ^= 0x00FFFFFF;
   }

   characteristic = dividend[0] - divisor[0] + 0x00400000;

   while (beats--)
   {
      #if 0
      sright(1, 6, divisor);
      #endif

      carry = addcarry(1, 6, &dividend[1], &divisor[1], lookaside);

      #if 0
      printf("[%x:%6.6x:%6.6x:%6.6x:%6.6x:%6.6x:%6.6x]\n",
               carry, lookaside[0], lookaside[1], lookaside[2],
                      lookaside[3], lookaside[4], lookaside[5]);
      #endif

      if (carry)
      {
         dividend[1] = lookaside[0];
         dividend[2] = lookaside[1];
         dividend[3] = lookaside[2];

         #if 1
         dividend[4] = lookaside[3];
         dividend[5] = lookaside[4];
         dividend[6] = lookaside[5];
         #endif
      }

      sleft(carry, 3, quotient);

      #if 1
      sright(1, 6, divisor);
      #endif
   }

   if (quotient[0] & 0x00800000)
   {
      /*************************************************

	normalised

      *************************************************/

      characteristic++;
   }
   else
   {
      sleft(0, 3, quotient);
      #if 0
      characteristic--;
      #endif
   }

   a = characteristic ^ signs;
   b = quotient[0] ^ signs;
   mantissa2 = quotient[1] ^ signs;
   mantissa3 = quotient[2] ^ signs;
}
#endif


/******************************************************************

	FPX	floating push and expand

	expands compressed floating item from storage
	into the internal stack top for computation

	compressed format is 48 bits and does not compute
	It is for storage/transmission economy only.

	computation format is 96 bits

	if the number is normalised, 12 sign bits are
	added to the left of the lower 10 exponent bits
	unless the exponent is lower than midpoint: then
	12 reverse-sign bits are inserted to maintain
	the same proximity to midpoint

		Sign
		 Midpoint

		00 [11 1111 1111 11] xx xxxx xxxx...	+ number < midpoint
		01 [00 0000 0000 00] xx xxxx xxxx...	+ number >= midpoint
		10 [11 1111 1111 11] yy yyyy yyyy...	1s complement number
		11 [00 0000 0000 00] yy yyyy yyyy...	1s complement number

	if the number is zero or otherwise unnormalised
	then 12 sign bits are inserted in the 11-bit exponent

		0x [00 0000 0000 00] xx xxxx xxxx...
		1y [11 1111 1111 11] yy yyyy yyyy...

	36 sign bits are added to the mantissa

******************************************************************/


void fpx(int ea, int _stack_top[])
{
   unsigned int			 temp[4];
   unsigned int			 extrascale;
   unsigned int			 signs;

   burst_read2(temp, ea);
   extrascale = (temp[0] & 0x00800000) ? 0x003FFC00 : 0;
   signs      = (temp[0] & 0x00800000) ? 0x00FFFFFF : 0;

   if (2048 & (temp[0] ^ signs))		/*  nonZero/normalised?	*/
   {
      if (0x00400000 & (temp[0] ^ signs))	/*	  >= midpoint ?	*/
      {
      }
      else extrascale ^= 0x003FFC00;
   }

   temp[3] = signs;
   temp[2] = 0x00FFFFFF & ((temp[1] << 12) | (signs & 0x00000FFF));
   temp[1] = 0x00FFFFFF & ((temp[0] << 12) | (temp[1] >> 12));
   temp[0] = (0x00C00000 & temp[0])
           | ((temp[0] >> 12) & 0x000003FF)
           | (extrascale);

   _stack_top[0] = temp[0];
   _stack_top[1] = temp[1];
   _stack_top[2] = temp[2];
   _stack_top[3] = temp[3];
}


/******************************************************************

	FPP	floating pop and pack

	compression instruction for storage

	pops a 96-bit floating item from the internal stack top
	stores it as 48 bits
	discards 12 exponent bits and 36 mantissa bits

	There is no rounding attempted, because:

	1. This compression is for numbers which can be
	   represented in 48 bits. That is quite a lot of
	   numbers

	2. The instruction is intended to be strictly
	   logical, not arithmetic and certainly not complex

	3. You can't un-round the numbers on later expansion
	   with the FPX instruction

******************************************************************/

void fpp(int ea, int _stack_top[])
{
   unsigned int			 temp[3];


   temp[0] = _stack_top[0];
   temp[1] = _stack_top[1];
   temp[2] = _stack_top[2];

   temp[0] = 0x00FFFFFF & ( (temp[0] & 0x00C00000)
                          |((temp[0] & 0x000003FF) << 12)
                          | (temp[1] >> 12) );

   temp[1] = 0x00FFFFFF & ((temp[1] << 12) | (temp[2] >> 12));

   burst_write2(temp, ea);
}

