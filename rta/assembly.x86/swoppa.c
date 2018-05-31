#include <stdio.h>
static char	 data[] = "abcd";
int main()
{
   int	x = __builtin_bswap32((int) *((int *)data));
   printf("%x\n", x);
   return 0;
}

