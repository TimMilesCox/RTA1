#include <stdio.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <unistd.h>

void start_second(void *step_second)
{
   int		 f = open("../epoch.rta/second", O_RDONLY, 0777);
   int		 x;

   if (f < 0) printf("epoch check not available %d\n", errno);
   else
   {
      x = read(f, step_second, 8);
      if (x ^ 8) printf("stored second unavailable\n");
   }

   close(f);
}

void store_second(void *step_second)
{
   int		 f = open("../epoch.rta/second", O_WRONLY | O_TRUNC | O_CREAT, 0777);
   int		 x;

   if (f < 0) printf("epoch step not stored %d\n", errno);
   else
   {
      x = write(f, step_second, 8);
      if (x ^ 8) printf("%d epoch step not stored %d\n", x, errno);
      x = close(f);
      if (x < 0) printf("epoch step not secured %d\n", errno);
   }
}

