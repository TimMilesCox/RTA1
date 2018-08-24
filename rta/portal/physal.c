#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "../include.rta/physa.h"
#include "../include.rta/argue.h"

int main(int argc, char *argv[])
{
   int			 x;
   int			 y;
   int			 ofile;

   unsigned char	*p;
   unsigned char	*q;

   argue(argc, argv);

   if (arguments)
   {
      q = physa(argument[0]);

      if (q)
      {
         p = q - 11;

         ofile = open("physa", O_WRONLY | O_TRUNC | O_CREAT, 0777);

         if (ofile < 0) printf("%s open error %d\n", argument[0], errno);
         else
         {
            y = write(ofile, q, 16);
            y = write(ofile, p, 64);
            if (y < 0) printf("%s write error %d\n", argument[0], errno);
            close(ofile);
         }
      }
      else printf("device %s not retrieved %d\n", argument[0], errno);
   }
   else printf("device name at argument 1\n");

   return 0;
}

