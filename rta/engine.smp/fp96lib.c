#include "emulate.h"
#include "smp.h"
#include "ii.h"
#include "fpu.h"
#include "fp96lib.h"
#include "fp192lib.h"

/******************************************************************

	these two shifts are for 1 bit position only
	with a 1-bit sign or zero fill

******************************************************************/


void sright(int insert_bit, int words, int data[])
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

void sleft(int insert_bit, int words, int data[])
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

int add(int words, int to[], int from[])
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


int addcarry(int startvalue, int words,
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

