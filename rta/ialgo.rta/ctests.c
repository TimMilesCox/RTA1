#include <stdio.h>
#include "csample.h"
#include "csample1.h"
#include "csample2.h"
#include "csample3.h"
#include "csample4.h"

static long	 left	= 9000000000000,
		 right	= 7000000000000,
		 centre = 3000000000000,
		 or_mask	= 0x5a5a5a5a5a5a,
		 xor_mask	= 0xFFFF999999999999,
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

static int	*indirect_scale_right	= &scale_right;
static int	*indirect_scale_left	= &scale_left;

static unsigned	 and_u32	= -8;

static int	 one = 1,
		 two = 2,
		 four = 4,
		 eight = 8,
		 sixteen = 16;

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
   printf("%ld\n", (long) TEST11);
   printf("%ld\n", TEST12);

//   printf("TEST21 not feasible on 64-bit PC using clang %s\n", "TEST21");
//   printf("TEST22 not feasible on 64-bit PC using clang %s\n", "TEST22");
   printf("%ld\n", TEST21);
   printf("%ld\n", TEST22);
   printf("%ld\n", TEST23);
   printf("%ld\n", TEST24);
   printf("%ld\n", TEST25);
   printf("%ld\n", TEST26);
   printf("%ld\n", TEST27);
   printf("%ld\n", TEST28);
   printf("%ld\n", TEST29);

   printf("%ld\n", TEST31);
   printf("%ld\n", TEST32);
   printf("%ld\n", TEST33);
   printf("%ld\n", TEST34);
   printf("%ld\n", TEST35);
   printf("%ld\n", TEST36);

   printf("%d\n", TEST37);
   printf("%d\n", TEST38);
   printf("%d\n", TEST39);
   printf("%d\n", TEST40);
   printf("%d\n", TEST41);
   printf("%d\n", TEST42);

   printf("%ld\n", TEST45);
   printf("%ld\n", TEST46);
   printf("%ld\n", TEST47);
   printf("%ld\n", TEST48);
   printf("%ld\n", TEST49);
   printf("%ld\n", TEST50);

   printf("%d\n", TEST51);
   printf("%d\n", TEST52);
   printf("%d\n", TEST53);
   printf("%d\n", TEST54);
   printf("%d\n", TEST55);
   printf("%d\n", TEST56);

   printf("%ld\n", TEST61);
   printf("%ld\n", TEST62);
   printf("%ld\n", TEST63);
   printf("%ld\n", TEST64);
   printf("%ld\n", TEST65);
   printf("%ld\n", TEST66);

   printf("%ld\n", TEST71);
   printf("%ld\n", TEST72);
   printf("%ld\n", TEST73);
   printf("%ld\n", TEST74);

   printf("%ld\n", TEST75);
   printf("%ld\n", TEST76);

   printf("%ld\n", TEST80);
   printf("%ld\n", TEST81);
   printf("%ld\n", TEST82);
   printf("%ld\n", TEST83);
   printf("%ld\n", TEST84);
   return 0;
}
