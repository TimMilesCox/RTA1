#include <stdio.h>

#ifdef INTEL
static char *consta = "0\n214365870921436587" ;
#else
static char *consta = "\n0123456789012345678" ;
#endif

int main()
{
   unsigned long	 sum = 0;
   unsigned long	 tota = 0;
   unsigned short	 quota;
   unsigned short	*q = (unsigned short *) consta;
   int			 x = 10,
			 y = 2000/20;

   while (x--) tota += *q++;
   while (y--) sum += tota;

   printf("payload sum %lx\n", sum);

   tota = 0;

   for (;;)
   {
      x = fscanf(stdin, "%hx", &quota);
      if (x == 0) break;
      printf("+[%lx]\n", quota);
      tota += quota;
   }

   printf("preliminary part %lx\n", tota);
   sum += tota;
   printf("raw sum %lx\n", sum);

   quota = sum;
   while (sum >>= 16)
   {
      sum += quota;
      quota = sum;
   }

   printf("wrapped %4.4x %4.4x\n", quota, quota ^ 0xFFFF);

   return 0;
}

