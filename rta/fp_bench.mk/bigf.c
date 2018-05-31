#include <stdio.h>

int main()
{
   int		 x = 0;

   while (x < 100000) printf("%d + 1\n", x++);
   printf(".\n");
}

