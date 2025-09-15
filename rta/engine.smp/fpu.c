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
#include "rw.h"
#include "alu.h"
#include "fpu.h"
#include "trace.h"

#define	GUARD_BITS	_register[128+19]
			/*	default 0x00C00000	*/
#undef	TRACE

#if 0
extern int			 psr;
extern int			 iselect;
extern int			 _register[256];

extern char			 flag[];
#endif

/******************************************************************

	these two shifts are for 1 bit position only
	with a 1-bit sign or zero fill

******************************************************************/


static void sright(int insert_bit, int words, int data[])
{
   int			 carry = insert_bit,
			 index = 0;

   while (index < words)
   {
      carry &= 1;
      carry <<= 24;
      carry |= data[index];
      data[index] = carry >> 1;
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
   unsigned int	 carry = 0;
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

/*************************************************************
	normalise, round and store residue result
	in registers 8:9:10:11
*************************************************************/

static int store_minor_result(int index, int signs, int characteristic, int result[], smp *xcore)
{
   int		 normalising_count = 0;
   int		*register_set = xcore->register_set;

   int		*selector = _register + index; /* register_set + index; */

   characteristic -= 72;

   while (normalising_count < 72)
   {
      if ((result[0] ^ signs) & 0x00800000) break;
      sleft(signs, 3, result);
      normalising_count++;
   }

   if (normalising_count < 72) characteristic -= normalising_count;
   else                        characteristic = 0;

   if (characteristic & 0xFF800000)
   {
      /**********************************************
	the minor part cannot be represented
      **********************************************/

      #if 1
      #if 1
      ii(II_XPO, index, xcore);
      #else
      XPO_INTERRUPT
      #endif

      return -3;

      #else

      characteristic = 0;
      result[0] = signs;
      result[1] = signs;
      result[2] = signs;
      #endif
   }

   #if 1

   selector[4] = characteristic ^ signs;
   selector[5] = result[0];
   selector[6] = result[1];
   selector[7] = result[2];

   #else

   scalea  = characteristic ^ signs;
   mantissa1a = result[0];
   mantissa2a = result[1];
   mantissa3a = result[2];

   #endif

   return 0;
}

/************************************************************

	change floating value in two numbers
	to one floating number
	with signs rolled into normalising displacement
	of 2nd part

*************************************************************/

static void pack(int signs, int characteristic_magnitude, int fractions[])
{
   int		 normalising_count = characteristic_magnitude
                                   - (fractions[4] ^ signs)
                                   - 72;

   fractions[4] = fractions[5];
   fractions[5] = fractions[6];
   fractions[6] = fractions[7];
   fractions[7] = signs;

   if ((normalising_count < 0) || (normalising_count > 71))
   {
      fractions[4] = signs;
      fractions[5] = signs;
      fractions[6] = signs;
   }
   else
   {
      while (normalising_count--) sright(signs, 4, fractions+4);
   }
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

#if 0

#include "add_bias.c"

#else

static int add_bias(int bias, int left[], int biased_addend[], smp *xcore)
{
	/*****************************************************

	  bias is now guaranteed 0..+72

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
	int			 mantissa_words = 3;
	int			 inverse = 3;
	int			 downscale = -71;

	int			 right[8];

	if (psr & FLOATING_RESIDUE)
	{
		mantissa_words = 6;

#if 0
		left[7] = left[4];
		left[6] = signs;
		left[5] = signs;
		left[4] = signs;
		biased_addend[7] = carry;
		biased_addend[6] = carry;
		biased_addend[5] = carry;
#endif

		inverse = 6;
		downscale = -143;
		if (bias > 160) return 0;

		/* an unchanged double result has been formatted */
	}
	else if (bias > 80) return 0;


	while (to < word_offset) right[to++] = carry;

	for (;;)
	{
		carry <<= leading_bits;
		carry |= transition >> bits_offset;
		carry &= 0x00FFFFFF;
		right[to] = carry;
		if (to > mantissa_words) break;
		to++;
		carry = transition;
		transition = biased_addend[from++];
	}

	/**********************************************************

	  carry now contains the same value as word 4 or 7 of
	  the biased string

	  subscript [to] contains the value 4 or 7

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

		while (inverse)
		{
			/***************************************************
			  invert [6:5:4] and always 3:2:1
			 ***************************************************/

			carry += left[inverse];
			left[inverse] = carry & 0x00FFFFFF;
			carry >>= 24;
			inverse--;
			carry += signs;
		}

		/*******************************************************
		  FIXME why are signs added to carry every time except 1st?
		  this is the code from before double result was stored

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
		 *******************************************************/
	}

	if ((left[0] ^ signs) & 1)
	{
		/*******************************************************

		  a non-sign has carried from mantissa bit 71
		  shift right, characteristic+

		 *******************************************************/

		sright(signs, mantissa_words + 1, left);
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

			sleft(signs, mantissa_words, left + 1);
			scale--;
			if (scale < downscale) break;
		}
	}

	left[0] = signs;
	return scale;
}

#endif

/******************************************************************

  ones complement adder does Floating Add
  and Floating Add Negative

  for ADD, direction is zero

  for ADD NEGATIVE, direction is 0x00FFFFFF
  and the right side operand is flipped on
  acquisition

 ******************************************************************/


static int ones_add(int ea, int target[], int direction, smp *xcore)
{
   int		 normalised_addend[8] = { __CHARACTERISTIC,
					  __MANTISSA_1,
					  __MANTISSA_2,
					  __MANTISSA_3	} ;

   int		 biased_addend[8];

   int		 magnitude_characteristic_difference;

   int		 signs,
   		 signs_right,
		 signs_quite_right,
   		 characteristic,
		 carry;

   int		*register_set = xcore->register_set;

   int		 index = target - _register;

   int		 magnitude_characteristic_left,
		 magnitude_characteristic_right;

   int		*normalised = normalised_addend;
   int		*biased = biased_addend;


   if ((burst_read4(biased_addend, ea, xcore)) < 0) return -LP_ADDRESS;

   biased_addend[0] ^= direction;
   biased_addend[1] ^= direction;
   biased_addend[2] ^= direction;
   biased_addend[3] ^= direction;

   signs       = 0x00FFFFFF & (0 - ((__CHARACTERISTIC >> 23) & 1));
   signs_right = 0x00FFFFFF & (0 - ((biased_addend[0] >> 23) & 1));

   biased_addend[4] = signs_right;
   normalised_addend[4] = signs;

   magnitude_characteristic_left  = (target[0] ^ signs) & 0x00FFFFFF;
   magnitude_characteristic_right = (biased_addend[0] ^ signs_right) & 0x00FFFFFF;
   signs_quite_right = signs_right;

   if ((biased_addend[1] ^ signs_right) & 0x00800000)
   {
      /***********************************************************
         source 2 normalised?
      ***********************************************************/

      if ((__MANTISSA_1 ^ signs) & 0x00800000)
      {
         /********************************************************
            register source normalised
         ********************************************************/

         if (psr & FLOATING_RESIDUE)
         {
            if ((index & 127) == A)
            {
               /**************************************************
                  fixed accumulators operation has 96-bit operands
                  and stores a 96-bit residue
                **************************************************/

               normalised_addend[4] = signs;
               normalised_addend[5] = signs;
               normalised_addend[6] = signs;
               normalised_addend[7] = signs /* ^ GUARD_BITS */ ;

               biased_addend[4] = signs_right;
               biased_addend[5] = signs_right;
               biased_addend[6] = signs_right;
               biased_addend[7] = signs_right;
            //  ?  biased_addend[0] = signs_right;
            }
            else
            {
               /**************************************************
                  SIMD operation has 192-bit operands
                  accumulators in internal stack
               **************************************************/

               if (index > 120) return -LP_AUTHORITY;

               if ((burst_read4(biased_addend + 4, ea + 4, xcore)) < 0) return -LP_ADDRESS;

               biased_addend[4] ^= direction;
               biased_addend[5] ^= direction;
               biased_addend[6] ^= direction;
               biased_addend[7] ^= direction;

               normalised_addend[4] = __CHARACTERISTIC_R;
               normalised_addend[5] = __MANTISSA_4;
               normalised_addend[6] = __MANTISSA_5;
               normalised_addend[7] = __MANTISSA_6;

               pack(signs, magnitude_characteristic_left, normalised_addend);
               pack(signs_right, magnitude_characteristic_right, biased_addend);
//               normalised_addend[7] ^= GUARD_BITS;
            }
         }

         magnitude_characteristic_difference = magnitude_characteristic_left
            - magnitude_characteristic_right;

         if (magnitude_characteristic_difference < 0)
         {
            normalised = biased_addend;
            biased = normalised_addend;
            signs_quite_right = signs;
            signs = signs_right;

            magnitude_characteristic_difference =
            0 - magnitude_characteristic_difference;
         }

         if (psr & FLOATING_RESIDUE) normalised[7] = signs ^ GUARD_BITS;
         else                        normalised[4] = signs ^ GUARD_BITS;

         characteristic = normalised[0] ^ signs;
         normalised[0] = signs;
         biased[0] = signs_quite_right;

         carry = add_bias(magnitude_characteristic_difference,
                          normalised,
                          biased, xcore);

         characteristic += carry;

         #ifdef XPO_INTERRUPT
         if (characteristic & 0xFF800000)
         {
            /***************************************************
               exponent overflow or underflow
               indicate for application
               and take other default ISR action
            ***************************************************/

            ii(II_XPO, index, xcore);
            return -3;
         }
         #endif

         if ((normalised[0] ^ normalised[1]) & 0x00800000)
         {
         }
         else characteristic = 0;

         /******************************************************

            carry is the change to scale
            which is -1 or 0 or +1

            the sign of the result is in bits 95..72
            it may have flipped

            however if it did, mantissa bits 71..0 are
            already correct polarity

         ******************************************************/


         __CHARACTERISTIC = characteristic ^ normalised[0];
         __MANTISSA_1 = normalised[1];
         __MANTISSA_2 = normalised[2];
         __MANTISSA_3 = normalised[3];

         if (psr & FLOATING_RESIDUE)
         return store_minor_result(index, normalised[0],
                                   characteristic, normalised + 4, xcore);
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

         __CHARACTERISTIC  = biased_addend[0];
         __MANTISSA_1 = biased_addend[1];
         __MANTISSA_2 = biased_addend[2];
         __MANTISSA_3 = biased_addend[3];

         if (psr & FLOATING_RESIDUE)
         {
            scalea = signs_quite_right;
            mantissa1a = signs_quite_right;
            mantissa2a = signs_quite_right;
            mantissa3a = signs_quite_right;
         }
      }
   }
   else if (psr & FLOATING_RESIDUE)
   {
      /*******************************************************************

	do nothing at all if starboard operand is not normalised
        unless store minor result is opted
	unnormalised numbers are zero
	so the portside operand in accumulator A:B:6:7 is not changed

      *******************************************************************/

      scalea = signs;
      mantissa1a = signs;
      mantissa2a = signs;
      mantissa3a = signs;
   }

   return 0;
}


int __fa(int ea, int target[], smp *xcore)
{
   return ones_add(ea, target, 0, xcore);
}

int __fan(int ea, int target[], smp *xcore)
{
   return ones_add(ea, target,  0x00FFFFFF, xcore);
}

int __fm(int ea, int target[], smp *xcore)
{
   int		 around[8] = { 0, 0, 0, GUARD_BITS, 0, 0, GUARD_BITS, 0 } ;

   int		 result[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
   int		 addend[8] = { __CHARACTERISTIC, __MANTISSA_1, __MANTISSA_2, __MANTISSA_3, 0, 0, 0, 0 } ;

   int		 multiplier[8] = { 0, 0, 0, 0,   0, 0, 0, 0 } ;

   int		 index1, index2, shift, characteristic, signs;
   int		 add_words = (psr & FLOATING_RESIDUE) ? 7 : 3;

   int		*register_set = xcore->register_set;
   int		 index = target - _register;

   int		 operative = 4;
   int		 signs_left = 0;
   int		 signs_right = 0;

   if ((burst_read4(multiplier, ea, xcore)) < 0) return -LP_ADDRESS;

   if (addend[0] & 0x00800000)
   {
      /******************************************************
         maultiplicand is negative
         change characteristic result to 1s
         invert polarity  multiplicand to plus
      ******************************************************/

      signs_left = 0x00FFFFFF;

      result[0] ^= 0x00FFFFFF;
      addend[0] ^= 0x00FFFFFF;
      addend[1] ^= 0x00FFFFFF;
      addend[2] ^= 0x00FFFFFF;
      addend[3] ^= 0x00FFFFFF;

      #if 0
      if (psr & FLOATING_RESIDUE)
      {
         addend[4] ^= 0x00FFFFFF;
         addend[5] ^= 0x00FFFFFF;
         addend[6] ^= 0x00FFFFFF;
         addend[7] ^= 0x00FFFFFF;
      }
      #endif
   }

   if (multiplier[0] & 0x00800000)
   {
      /******************************************************
         multiplier is negative
         invert characteristic result
         invert polarity multiplier to plus
      ******************************************************/

      signs_right = 0x00FFFFFF;

      result[0]     ^= 0x00FFFFFF;
      multiplier[0] ^= 0x00FFFFFF;
      multiplier[1] ^= 0x00FFFFFF;
      multiplier[2] ^= 0x00FFFFFF;
      multiplier[3] ^= 0x00FFFFFF;
   }

   if (psr & FLOATING_RESIDUE)
   {
      around[3] = 0;
     
      if ((index & 127) == A)
      {
         /***************************************************
            96 bit operation generating 96-bit residue
         ***************************************************/
      }
      else
      {
         /***************************************************
           192-bit SIMD operation
         ***************************************************/

         operative = 8;

         if ((burst_read4(multiplier + 4, ea + 4, xcore)) < 0) return -LP_ADDRESS;

         addend[4] = __CHARACTERISTIC_R ^ signs_left;
         addend[5] = __MANTISSA_4       ^ signs_left;
         addend[6] = __MANTISSA_5       ^ signs_left;
         addend[7] = __MANTISSA_6       ^ signs_left;

         multiplier[4] ^= signs_right;
         multiplier[5] ^= signs_right;
         multiplier[6] ^= signs_right;
         multiplier[7] ^= signs_right;

         pack(0, addend[0], addend);
         pack(0, multiplier[0], multiplier);
      }
   }

   signs = result[0];
   characteristic = 0;

   /************************************************************

	any number not normalised is zero in this architecture
	therefore if the normalising bit is absent in either
	the multiplicand or the multiplier, the algebraic result
	of absolute zero is already in the workspace and only
	needs copying back to accumulator registers A:B:6:7

   ************************************************************/


   if (addend[1] & multiplier[1] & 0x00800000)
   {
      for (index1 = 1; index1 < operative; index1++)
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

		each add is a 168-bit add

           **********************************************************/

            sright(0, 7, &addend[1]);
            if (shift & 0x00800000) add(7, &result[1], &addend[1]);
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

         #define FP_EITHER_OR
         #ifdef  FP_EITHER_OR
//         if (psr & FLOATING_RESIDUE) around[3] = 0;
         shift = add(add_words + 1, result + 1, around);
         characteristic += shift;                  /*      1 or 0     */
         if (shift) sright(shift, add_words, result + 1);
         #endif
      }
      else
      {
         /***********************************************************

		no they didn't

         ***********************************************************/

         sleft(0, add_words + 1, result + 1);
         characteristic--;
      }

      #if 0
      characteristic &= 0x007FFFFF;
      #endif

      /**************************************************************
		because of the conditional shift left just above
		the result string could not be primed with guard
		bits, so they are added afterwards
      **************************************************************/

      #ifndef FP_EITHER_OR
//      if (psr & FLOATING_RESIDUE) around[3] = 0;
      shift = add(add_words + 1, result + 1, around);
      characteristic += shift;			/*      1 or 0     */ 
      if (shift) sright(shift, add_words, result + 1); 
      #endif

      if (characteristic & 0xFF800000)
      {
         /************************************************************

		exponent has underflowed or overflowed

         ************************************************************/

         #ifdef XPO_INTERRUPT
         #if 1
         ii(II_XPO, index, xcore);
         #else
         XPO_INTERRUPT
         #endif

         return -3;

         /***************************************************
		indicate for application
		and other default ISR action
         ***************************************************/

         #else
         characteristic = 0x007FFFFF;
         #endif
      }

      /***************************************************************

	now sign the result and store it in the accumulator

      ***************************************************************/

      result[1] ^= result[0];
      result[2] ^= result[0];
      result[3] ^= result[0];

      if (psr & FLOATING_RESIDUE)
      {
         signs = result[0];
         result[4] ^= result[0];
         result[5] ^= result[0];
         result[6] ^= result[0];
      }

      result[0] ^= characteristic;
   }
   else
   {
      result[1] = signs;
      result[2] = signs;
      result[3] = signs;

      if (psr & FLOATING_RESIDUE)
      {
         result[4] = signs;
         result[5] = signs;
         result[6] = signs;
      }
   }

   __CHARACTERISTIC = result[0];
   __MANTISSA_1 = result[1];
   __MANTISSA_2 = result[2];
   __MANTISSA_3 = result[3];

   if (psr & FLOATING_RESIDUE)
   return store_minor_result(index, signs, characteristic, result + 4, xcore);

   return 0;
}

#if 1
int __fd(int ea, int target[], smp *xcore)
{
   int		 remainder[8] = { __CHARACTERISTIC, __MANTISSA_1, __MANTISSA_2, __MANTISSA_3,
                                GUARD_BITS, 0, 0, GUARD_BITS } ;

   int		 divisor[8]   = { 0, 0, 0, 0,
				  0x00FFFFFF, 0x00FFFFFF,
                                  0x00FFFFFF, 0x00FFFFFF } ;

   int		 lookaside[7];

   int		 reciprocal[8] = { 0, 0, 0, 0, 0, 0, 0, 0 } ;

   int		 signs = 0,
		 signs1 = 0,
		 beats = 72,
                 mantissa_words = 3,
                 carry;

   int		 signs_left;

   int		*register_set = xcore->register_set;
   int		 index = target - _register;


   if ((burst_read4(divisor, ea, xcore)) < 0) return -LP_ADDRESS;

   if ((divisor[0] ^ divisor[1]) & 0x00800000)
   {
   }
   else
   {
      /*****************************************************
         divisor not normalised. Return -0.0
      *****************************************************/

      __CHARACTERISTIC = 0x00FFFFFF;
      __MANTISSA_1     = 0x00FFFFFF;
      __MANTISSA_2     = 0x00FFFFFF;
      __MANTISSA_3     = 0x00FFFFFF;

      if (psr & FLOATING_RESIDUE)
      {
         __CHARACTERISTIC_R = 0x00FFFFFF;
         __MANTISSA_4       = 0x00FFFFFF;
         __MANTISSA_5       = 0x00FFFFFF;
         __MANTISSA_6       = 0x00FFFFFF;
      }

      return 0;
   }

   if (divisor[0] & 0x00800000) signs = 0x00FFFFFF;
   else
   {
      /*****************************************************
         divisor is positive
         so turn divisor mantissa negative
         for trial negative adds
      *****************************************************/

      divisor[1] ^= 0x00FFFFFF;
      divisor[2] ^= 0x00FFFFFF;
      divisor[3] ^= 0x00FFFFFF;
   }

   /********************************************************
      turn divisor characteristic positive in any case
   ********************************************************/

   divisor[0] ^= signs;

   if (remainder[0] & 0x00800000) signs1 = 0x00FFFFFF;

   /********************************************************
      take a copy of dividend signs
      before dividend signs converted to quotient signss
   ********************************************************/
   
   signs_left = signs1;

   /********************************************************
      turn dividend positive
   ********************************************************/
   
   remainder[0] ^= signs1;
   remainder[1] ^= signs1;
   remainder[2] ^= signs1;
   remainder[3] ^= signs1;

   /********************************************************
      convert dividend signs to quotient signs
   ********************************************************/
   
   signs1 ^= signs;

   if (psr & FLOATING_RESIDUE)
   {
      beats = 144;
      remainder[4] = 0;
      mantissa_words = 6;

      if ((index & 127) == A)
      {
         /**************************************************
             operation 96-bit dividend 96-bit divisor
             produces 96-bit residue
         **************************************************/   
      }
      else
      {
         /**************************************************
            192-bit SIMD operation
         **************************************************/   

         if ((burst_read4(divisor + 4, ea + 4, xcore)) < 0) return -LP_ADDRESS;

         remainder[4] = __CHARACTERISTIC_R ^ signs_left;
         remainder[5] = __MANTISSA_4       ^ signs_left;
         remainder[6] = __MANTISSA_5       ^ signs_left;
         remainder[7] = __MANTISSA_6       ^ signs_left;

         if (signs == 0)
         {
            /**********************************************
               trial add negative magnitude
            **********************************************/

            divisor[4] ^= 0x00FFFFFF;
            divisor[5] ^= 0x00FFFFFF;
            divisor[6] ^= 0x00FFFFFF;
            divisor[7] ^= 0x00FFFFFF;
         }

         pack(0, remainder[0], remainder);
         pack(0x00FFFFFF, divisor[0], divisor);
         remainder[7] ^= GUARD_BITS;
      }
   }

   while(beats--)
   {
      carry = addcarry(1, 7, remainder + 1, divisor + 1, lookaside);

      if (carry)
      {
         remainder[1] = lookaside[0];
         remainder[2] = lookaside[1];
         remainder[3] = lookaside[2];

         remainder[4] = lookaside[3];
         remainder[5] = lookaside[4];
         remainder[6] = lookaside[5];
         remainder[7] = lookaside[6];
      }

      sleft(carry, mantissa_words, reciprocal + 1);
      sright(1, 7, divisor + 1);
   }

   reciprocal[0] = remainder[0] - divisor[0] + 0x00400001;

   if (reciprocal[1] & 0x00800000)
   {
   }
   else
   {
      beats = 72;

      if (psr & FLOATING_RESIDUE) beats = 144;

      while (beats--)
      {
         sleft(0, mantissa_words, reciprocal + 1);
         reciprocal[0]--;
         if (reciprocal[1] & 0x00800000) break;
      }
   }

   if (reciprocal[1] & 0x00800000)
   {
   }
   else reciprocal[0] = 0;
   

   #ifdef XPO_INTERRUPT
   if (reciprocal[0] & 0xFF800000)
   {
      /***************************************************
		over or underflow exponent
                indicate for application
		and take default ISR action
      ***************************************************/

      #if 1
      ii(II_XPO, index, xcore);
      #else
      XPO_INTERRUPT
      #endif

      return -3;
   }
   #else
   reciprocal[0] &= 0x007FFFFF;
   #endif

   __CHARACTERISTIC = reciprocal[0] ^ signs1;
   __MANTISSA_1 = reciprocal[1] ^ signs1;
   __MANTISSA_2 = reciprocal[2] ^ signs1;
   __MANTISSA_3 = reciprocal[3] ^ signs1;

   if (psr & FLOATING_RESIDUE)
   {
      reciprocal[4] ^= signs1;
      reciprocal[5] ^= signs1;
      reciprocal[6] ^= signs1;
      return store_minor_result(index, signs1, reciprocal[0], reciprocal + 4, xcore);
   }

   return 0;
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


   burst_read4(divisor, ea, xcore);
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

	12 bits opposite polarity to midpoint bit are inserted
	after midpoint bit

		s1[00 0000 0000 00] xx xxxx xxxx...
		s0[11 1111 1111 11] xx xxxx xxxx...

 	Effect is they are 12 sign bits unless the
	exponent is lower than midpoint: then they are
	12 reverse-sign bits giving

		Sign
		 Midpoint

		00 [11 1111 1111 11] xx xxxx xxxx...	+ number < midpoint
		01 [00 0000 0000 00] xx xxxx xxxx...	+ number >= midpoint
		10 [11 1111 1111 11] yy yyyy yyyy...	1s complement number
		11 [00 0000 0000 00] yy yyyy yyyy...	1s complement number

	exponents in range 3FFC00..4003FF / C003FF..BFFCOO
	or -1024 / +1023 shift positions representing decimal
	exponent +- around 300 can be compressed and recovered

	if the number is zero or otherwise unnormalised
	then 12 sign bits are inserted in the 11-bit exponent
	to avoid making zero inputs into untidy outputs

	or better still the whole thing is output as signed zero

		0x [00 0000 0000 00] xx xxxx xxxx...
		1y [11 1111 1111 11] yy yyyy yyyy...

	36 sign bits are added to the mantissa

******************************************************************/


void fpx(int ea, int _stack_top[], smp *xcore)
{
   int				 temp[4];
   unsigned int			 extrascale;
   unsigned int			 signs;

   burst_read2(temp, ea, xcore);

   #if 1
   /***************************************************
	switch this on for testing / acceptance
	midpoint ^ 1 is the same thing as
	signs ^ midpoint ^ 1 ^ signs
   ***************************************************/

   signs      = (temp[0] & 0x00800000) ? 0x00FFFFFF : 0;

   if (2048 & (temp[0] ^ signs))
   {
      /************************************************
	normalised: extra exponent bits are
	reverse midpoint bit
      ************************************************/

      extrascale = (temp[0] & 0x00400000) ? 0 : 0x003FFC00;

      _stack_top[0] =  (temp[0] & 0x00C00000)
                    | ((temp[0] >> 12) & 1023)
                    | extrascale;
      _stack_top[1] = ((temp[0] & 4095) << 12)
                    | ((temp[1] >> 12) & 4095);
      _stack_top[2] = ((temp[1] & 4095) << 12) | (signs & 4095);
      _stack_top[3] = signs;
   }
   else
   {
      /************************************************
	unnormalised = zero = don't mess around
      ************************************************/

      _stack_top[0] = signs;
      _stack_top[1] = signs;
      _stack_top[2] = signs;
      _stack_top[3] = signs;
   }

   #else

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

   #endif
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

void fpp(int ea, int _stack_top[], smp *xcore)
{
    int			 temp[3];


   temp[0] = _stack_top[0];
   temp[1] = _stack_top[1];
   temp[2] = _stack_top[2];

   temp[0] = 0x00FFFFFF & ( (temp[0] & 0x00C00000)
                          |((temp[0] & 0x000003FF) << 12)
                          | (temp[1] >> 12) );

   temp[1] = 0x00FFFFFF & ((temp[1] << 12) | (temp[2] >> 12));

   burst_write2(temp, ea, xcore);
}

#include "trace.c"

