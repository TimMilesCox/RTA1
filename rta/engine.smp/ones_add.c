#include "emulate.h"
#include "smp.h"
#include "ii.h"

#include "rw.h"
#include "alu.h"
#include "fpu.h"
#include "fp96lib.h"
#include "fp192lib.h"
#include "ones_add.h"

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

/******************************************************************

  ones complement adder does Floating Add
  and Floating Add Negative

  for ADD, direction is zero

  for ADD NEGATIVE, direction is 0x00FFFFFF
  and the right side operand is flipped on
  acquisition

 ******************************************************************/


int ones_add(int ea, int target[], int direction, smp *xcore)
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

