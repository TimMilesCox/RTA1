
/*****************************************************************
   common subroutine of __fa48 __fan48
   separately compiled and more efficient than inline
   separate compilation the only away of avoiding inline
   compiler forced inline is less efficient than this
*****************************************************************/


#include "emulate.h"
#include "smp.h"
#include "fpu.h"
#include "ii.h"
#include "fpu48.h"

int ones_add48(int ea, int target[], int direction, smp *xcore)
{
   int		*register_set = xcore->register_set;
   int		 index = target - _register;

   int		 signs,
		 signs_right,
		 signs2;

   int		 scale,
		 characteristic;

   int		 magnitude_characteristic_difference,
		 magnitude_characteristic_left,
		 magnitude_characteristic_right;

   int		 biased_addend[2];

   int		*normalised = target;
   int		*biased = biased_addend;

   long		 mantissa;
   long		 mantissa_right;
   long		 endaround;

   if ((burst_read2(biased_addend, ea, xcore)) < 0) return -LP_ADDRESS;

   biased_addend[0] ^= direction;
   biased_addend[1] ^= direction;

   signs       = 0x00FFFFFF & (0 - ((target[0] >> 23) & 1));
   signs_right = 0x00FFFFFF & (0 - ((biased_addend[0] >> 23) & 1));
   signs2 = signs_right;

   magnitude_characteristic_left = ((target[0] ^ signs)
                                 &   0x00FFFFFF)
                                 >> (24-FIELD_S_XPO);

   magnitude_characteristic_right = ((biased_addend[0] ^ signs_right)
                                  &   0x00FFFFFF)
                                  >> (24-FIELD_S_XPO);

   if ((biased_addend[0] ^ signs_right) & NORMALISED)
   {
      /********************************************************
	 normalised source-2 = effective nonzero
         else source-1 unchanged. Test normalising bit
      ********************************************************/

      if (((target[0] ^ signs) & NORMALISED) == 0)
      {
         /********************************************************
             source-2 normalised and source-1 not
             write source-2 to source-1 registers
         ********************************************************/

         target[0] = biased_addend[0];
         target[1] = biased_addend[1];
      }
      else
      {
         /*****************************************************
            both inputs normalised
         *****************************************************/

         magnitude_characteristic_difference
            = magnitude_characteristic_left
            - magnitude_characteristic_right;

         if (magnitude_characteristic_difference < 0)
         {
            magnitude_characteristic_difference 
            = magnitude_characteristic_right
            - magnitude_characteristic_left;

            normalised = biased_addend;
            biased = target;
            signs2 = signs;
            signs = signs_right;

            /**************************************************
               after here only address the input by pointers

                  normalised[]
                      biased[]

               not by parameter         target[]
               or local variable biased_addend[]

               except when writing back  target <- normalised
            ***************************************************/
         }

         characteristic = (normalised[0] ^ signs)
                            >> (24 - FIELD_S_XPO);
   

         /********************************

	 63    55	       23	
	 _________________________________
	 |signs|    mantissa   |  guard	 |  FORM_16_32
	 |_____|_______________|_________|


	 63  59			23
	 _________________________________
	 |si|	    mantissa   |  guard	 |  FORM_12_36
	 |__|__________________|_________|

         ********************************/


         if (magnitude_characteristic_difference > 40)
         {
            /*****************************************************
                no overlap of field mantissa++guard
                so result is input of greater unsigned magnitude
            *****************************************************/

            target[0] = normalised[0];
            target[1] = normalised[1];
         }
         else
         {
            mantissa = ((long) signs << (MANTISSA+24))
                     | ((long) (normalised[0] & MANTISSA_U) << 48)
	             | ((long) (normalised[1] & 0x00FFFFFF) << 24)
                     | ((unsigned) signs ^ GUARD_BITS);

            mantissa_right = ((long) signs2 << (MANTISSA+24))
                     | ((long) (biased[0] & MANTISSA_U) << 48) 
                     | ((long) (biased[1] & 0x00FFFFFF) << 24)
                     | (unsigned) signs2;

            /*****************************************************
               algebraic shift here. (long) is cast to signed
               declaration is unsigned as this object mostly needs
            *****************************************************/

            (long) mantissa_right >>= magnitude_characteristic_difference;
            mantissa += mantissa_right;

            if (((mantissa >> 63) ^ signs) & 1)
            {
               /***************************************************
                  sample 1 sign bit from add result
                  mantissa as signed integer
                  mantissa polarity flipped
               ***************************************************/

               signs ^= 0x00FFFFFF;

               if (signs) mantissa += ((long) -1 << 24) ;
               else       mantissa += ((long) +1 << 24) ;

               #if 0
               if ((signs ^ (mantissa >> (MANTISSA+24))) & 1)
               {
                  (long) mantissa >>= 1;
                  characteristic-- ;
               }
               #endif
            }


            /*****************************************************
               carry / borrow is in overflow bit 56 or 60
               has non-sign carried from mantissa into signs field?
               if so -> shift -> exponent update
            *****************************************************/

            scale = 1 & (signs ^ (mantissa >> (MANTISSA+24)));

            if (scale)
            {
               /*******************************************************
                  scale is 1
                  a non-sign has carried from mantissa bit 31 / 35
                  shift the same unsign back down, characteristic+
               *******************************************************/

               (long) mantissa >>= 1;

               /* scale is 1 */
            }
            else
            {
               /*********************************************************
                  scale is zero and might get less
                  until normalising bit = not a sign
               *********************************************************/

               endaround = signs + 1;
               endaround &= 0x0000000001000000;
    
               /********************************************************
                  signs replacement bit 1000000x
                                     or 0000000x
               ********************************************************/    

               while ((((mantissa >> (MANTISSA+24-1)) ^ signs) & 1) == 0)
               {
                  /****************************************************
                     mantissa bit 31 / 35 has become a sign bit
                     shift left, characteristic-
                  ****************************************************/

                  mantissa <<= 1;
                  mantissa &= 0xFFFFFFFFFE000000;
                  mantissa |= endaround;
                  scale--;

                  if (scale < (1-MANTISSA)) break;
               }
            }

            characteristic += scale;

            if (((mantissa >> (MANTISSA+23)) ^ signs) & 1)
            {
               /***************************************************
                  normalised
               ***************************************************/
            }
            else characteristic = 0;

            if (characteristic & EXPONENT_BOUNDARY)
            {
               /***************************************************
                  exponent overflow or underflow
                  indicate for application
                  and take other default ISR action
               ***************************************************/

               ii(II_XPO, index, xcore);
               return -3;
            }

            characteristic ^= signs;

            target[0] = ((characteristic & PROFILE_S_XPO) << (MANTISSA-24))
                      | ((mantissa >> 48) & MANTISSA_U);

            target[1] = (mantissa >> 24) & 0x00FFFFFF;
         }
      }
   }

   return 0;
}

