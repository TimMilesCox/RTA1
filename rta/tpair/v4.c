#include "stdio.h"
#include "sys/fcntl.h"
#include "unistd.h"
#include "string.h"

#include "../include.rta/argue.h"

static int	 f, jumbo, x, y;

static char	 expected[16];
static char	 sample[12];

int main(int argc, char *argv[])
{
   argue(argc, argv);

   if (arguments < 1)
   {
      printf("v12 data_file_name\n");
      return 0;
   }

   f = open(argument[0], O_RDONLY, 0777);

   for (;;)
   {
      x = read(f, sample, 12);
      if (x < 12) break;
      jumbo++;
      sprintf(expected, "jumbo %4.4d  ", jumbo);
      y = memcmp(expected, sample, 12);
      if (y) break;
   }

   printf("scan last read %d difference %d next read %d\n", jumbo, y, x);
   close(f);
   return 0;
}

