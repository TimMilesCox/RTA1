#include "emulate.h"
#include "smp.h"
#include "ii.h"
#include "fpu.h"
#include "fp96lib.h"
#include "fp192lib.h"

/*************************************************************
	normalise, round and store residue result
	in registers 8:9:10:11
*************************************************************/

int store_minor_result(int index, int signs, int characteristic, int result[], smp *xcore)
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

void pack(int signs, int characteristic_magnitude, int fractions[])
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

