#include <stdio.h>

extern long trample(int[], char *);

static int	 sample[] = { 0, 0x61656d49, 0x0a74696e } ;
static char	 expect[] = "////////";

int main()
{
   long result = trample(sample + 1, expect);
   printf("%lx %s\n", result, expect);
   return 0;
}

