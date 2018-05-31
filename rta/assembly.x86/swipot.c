#include <stdio.h>

#define	COPY_SWAP(X, Y)	__asm__(" movl %0,%%eax" : "=m" (Y));	\
			__asm__(" bswap %eax") ;		\
			__asm__(" movl %%eax,%0" : "=m" (X));

static int x = 0xccbbaa00;
inline int ibswap32(int data)
{
   __asm__(" bswap %0" : "+r" (data)) ;
   return data;
}

int main()
{
   int		 y = ibswap32(x);
   int		 j = 0xa5a5a5a5;

   printf("%x\n", y);
   COPY_SWAP(j, x);
   printf("%x\n", y);
   return 0;
}

