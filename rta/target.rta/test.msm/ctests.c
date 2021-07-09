#include <stdio.h>
#include "csample.h"

static long	 left	= 9000000000000,
		 right	= 7000000000000,
		 centre = 3000000000000,
		 or_mask	= 0x5a5a5a5a5a5a,
		 xor_mask	= 0x999999999999,
		 and_mask	= 0xFFFF0000FFFF,
		 long_scale_left	= 9,
		 long_scale_right	= 3,
		 long_multiplier	= 6000000000,
		 long_divisor		= 3000000000;

static int	 scale_left	= 8,
		 scale_right	= 4,
		 multiplier	= 5000,
		 divisor	= 2500,
		 or_short	= 0xC00000,
		 xor_short	= -1,
		 and_short	= -8,
		 short_left	= 2000000,
		 short_right	= 4000000,
		 short_centre	= 6000000;

static unsigned	 and_u32	= -8;

int main()
{
   printf("%ld\n", TEST1);
   printf("%ld\n", TEST2);
   printf("%ld\n", TEST3);
   printf("%ld\n", TEST4);
   printf("%ld\n", TEST5);
   printf("%ld\n", TEST6);
   printf("%ld\n", TEST7);
   printf("%d\n", TEST8);
   printf("%d\n", TEST9);
   printf("%ld\n", TEST10);
   printf("%ld\n", TEST11);
   printf("%ld\n", TEST12);

   return 0;
}
