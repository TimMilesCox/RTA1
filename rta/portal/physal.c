#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include "../include.rta/physa.h"

int main(int argc, char *argv[])
{
   int			 x;
   int			 y;
   int			 ofile;

   unsigned char	*p;
   unsigned char	*q;

   if (argc > 1)
   {
      q = physa(argv[1]);

      if (q)
      {
         p = q - 11;

         ofile = open("physa", O_WRONLY | O_TRUNC | O_CREAT, 0777);

         if (ofile < 0) printf("%s open error %d\n", argv[1], errno);
         else
         {
            y = write(ofile, q, 16);
            y = write(ofile, p, 64);
            if (y < 0) printf("%s write error %d\n", argv[1], errno);
            close(ofile);
         }
      }
      else printf("device %s not retrieved\n", argv[1]);
   }
   else printf("device name at argument 1\n");

   return 0;
}

