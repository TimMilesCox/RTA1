#include "emulate.h"
#include "smp.h"
#include "fpu.h"
#include "ii.h"
#include "fpu48.h"
#include "addcarry.h"
#include "trace.h"

int __fa48(int ea, int target[], smp *xcore)
{
   return ones_add48(ea, target, 0, xcore);
}

int __fan48(int ea, int target[], smp *xcore)
{
   return ones_add48(ea, target,  0x00FFFFFF, xcore);
}

int __fm48(int ea, int target[], smp *xcore)
{
   int		*register_set = xcore->register_set;
   int		 index = target - _register;

   int		 signs; 
   int		 signs_right;

   int		 pass = MANTISSA;
   int		 characteristic = 0;

   int		 magnitude_characteristic_multiplicand,
		 magnitude_characteristic_multiplier;

   int		 multiplier[2];

   unsigned long mantissa;
   unsigned long mantissa_right;
   unsigned long product = 0;

   if ((burst_read2(multiplier, ea, xcore)) < 0) return -LP_ADDRESS;

   signs       = 0x00FFFFFF & (0 - ((target[0] >> 23) & 1));
   signs_right = 0x00FFFFFF & (0 - ((multiplier[0] >> 23) & 1));

   magnitude_characteristic_multiplicand = (target[0] ^ signs) >> (24 - FIELD_S_XPO);
   magnitude_characteristic_multiplier = ((multiplier[0]) ^ signs_right) >> (24 - FIELD_S_XPO);

   mantissa = (unsigned long) ((target[1] ^ signs)
            | ((unsigned long) (target[0] ^ signs) << 24)) << (64 - MANTISSA) ;

   mantissa_right = (unsigned long) ((multiplier[1] ^ signs_right)
                  | ((unsigned long) (multiplier[0] ^ signs_right) << 24)) << (64 - MANTISSA);

   if (mantissa & mantissa_right & ((long) 1 << 63))
   {
      /******************************************************
         both normalised or return zero or -zero
      ******************************************************/

      while (pass--)
      {
         /**************************************************
            product does not shift
            multiplicand mantissa magnitude >>  from bit 63
            multiplier mantissa magnitude << over bit 63
            both are normalise so first add takes place
         **************************************************/

         mantissa >>= 1;
         if (mantissa_right & ((long) 1 << 63)) product += mantissa;
         mantissa_right <<= 1;

         #ifdef TRACE48M
         printf("[%16.16lx]\n", product);
         #endif
      }

      characteristic = magnitude_characteristic_multiplicand
                     + magnitude_characteristic_multiplier
                     - MIDPOINT;

      if ((product & ((long) 1 << 63)) == 0)
      {
         product <<= 1;
         characteristic--;
      }

      if (characteristic & EXPONENT_BOUNDARY)
      {
         ii(II_XPO, index, xcore);
         return -3;
      }

      #if 0
      /**********************************************************

        _________________________________
        |	32      |    24	    | 8	|	fp48 form 16:32
        |_______________|___________|___|	mantissa 32 bits
                        +
                        |____________________________ guard word

        _________________________________
        |	36	   |  24      |4|	fp48 form 12:36
        |__________________|__________|_|	mantissa 36 bits
                           +
                           |_________________________ guard word

      **********************************************************/

      product += (unsigned long) GUARD_BITS << (64 - MANTISSA - 24) ;
      #endif

      signs ^= signs_right;
 
      #ifdef TRACE48M
      printf("[%16.16lx]\n", product);
      #endif
  }

   target[0] = ((characteristic << (MANTISSA-24))
             | (product >> (64-MANTISSA+24)))
             ^ signs;

   target[1] = ((product >> (64-MANTISSA)) & 0x00FFFFFF) ^ signs; 
   return 0;
}

int __fd48(int ea, int target[], smp *xcore)
{
   int          *register_set = xcore->register_set;
   int           index = target - _register;

   int           signs,
                 signs_right,
                 carry,
                 beats = MANTISSA;

   int		 characteristic = 0;

   int		 magnitude_characteristic_dividend,
		 magnitude_characteristic_divisor;

   unsigned long minuend;
   unsigned long subtrahend;
   unsigned long temp;
   unsigned long quotient = 0;

   int		 divisor[2];

   if ((burst_read2(divisor, ea, xcore)) < 0) return -LP_ADDRESS;

   if (1 & ((divisor[0] >> 23) ^ (divisor[0] >> (MANTISSA - 24 - 1))))
   {
      /********************************************************
         divisor normalised
      ********************************************************/
   }
   else
   {
      /********************************************************
         deliver value -0.0
      ********************************************************/

      target[0] = 0x00FFFFFF;
      target[1] = 0x00FFFFFF;
      return 0;
   }

   signs       = 0x00FFFFFF & (0 - ((target[0] >> 23) & 1));
   signs_right = 0x00FFFFFF & (0 - ((divisor[0] >> 23) & 1));

   minuend = (((unsigned long) (target[0] ^ signs) << 24) | (target[1] ^ signs));
   minuend <<= (64 - MANTISSA);
   minuend |= (unsigned long) GUARD_BITS << (64 - MANTISSA - 24);

   magnitude_characteristic_dividend = (target[0] ^ signs) >> (MANTISSA - 24);
   magnitude_characteristic_divisor = (divisor[0] ^ signs_right) >> (MANTISSA - 24);

   /***********************************************************
	_________________________________________________
	|  positive magnitude mantissa	| guard	|  zero	|
	|_______________________________|_______|_______|

	positive magnitude mantissa minuend ++ guard bits

   ***********************************************************/

   if (minuend >> 63)
   {
      /********************************************************
         and if not the result is already computed
      ********************************************************/

      subtrahend =  (unsigned long) divisor[0] << 24;
      subtrahend |= (unsigned long) divisor[1];
      subtrahend <<= 64 - MANTISSA;

      if (signs_right == 0)
      {
          subtrahend ^= 0xFFFFFFFFFFFFFFFF;
          subtrahend++;
      }

      /********************************************************
         add negative magnitude divisor
         by rolling dividend left, divisor not at all
         we can processs a mantissa most of register size
      ********************************************************/

      while (beats--)
      {
         quotient <<= 1;

         carry = _addcarry_u64(0, minuend, subtrahend, &temp);

         #ifdef TRACE48D 
         printf("[%x] [%16.16lx + ^%16.16lx = %16.16lx, %16.16lx]\n", carry,
                minuend, subtrahend, temp, quotient);
         #endif

         if (carry)
         {
            /*************************************************
               minuend - divisor = +

               minuend is held as poaitive magnitude
               subtrahend as minus nagnitude so + = -
	       test subtract underflow should give
               high order bit = underflow = not this time
            *************************************************/

            quotient |= (long) 1 << (64 - MANTISSA);
            minuend = temp;

            /************************************************
               positive test subtract
            ************************************************/
         }

         if (minuend >> 63)
         {
            /************************************************
               shifting dividend left allows longer mantissa
               but only kill a high-order bit by subtraction
               not by shifting
               instead shift divisor negative magnitude right
               when necessary
            ************************************************/

            subtrahend >>= 1;
            subtrahend |= (long) 1 << 63;
         }
         else minuend <<= 1;

         /***************************************************
             roll minuend away if you can
         ***************************************************/
      }

      if (quotient)
      {
         /***************************************************
            characteristic otherwise stays at zero
            consistent with generated zero mantissa
         ***************************************************/

         characteristic =  magnitude_characteristic_dividend
                        -  magnitude_characteristic_divisor
                        +  MIDPOINT + 1;

         #ifdef TRACE48D
         printf("%x - %x + %x = %x",  magnitude_characteristic_dividend,
                                      magnitude_characteristic_divisor,
                                      MIDPOINT+1, characteristic);
         #endif

         beats = MANTISSA;

         while (beats--)
         {
            if (quotient >> 63) break;
            characteristic--;
            quotient <<= 1;

            #ifdef TRACE48D
            putchar('.');
            #endif
         }
      }

      if (characteristic & EXPONENT_BOUNDARY)
      {
         ii(II_XPO, index, xcore);
         return -3;
      }

      signs ^= signs_right;
      
      target[0] = (quotient >> (24 + 64 - MANTISSA)) | (characteristic << (MANTISSA-24))
                ^ signs;

      target[1] = ((quotient >> (64 - MANTISSA)) & 0x00FFFFFFF) ^ signs;
   }
   return 0;
}

