#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define	HOW_WIDE	84
#define	HOW_MANY	5000000

static int		 fi,	/*  input file handle	*/
			 fo;	/* output file handle	*/

static unsigned char	 response[HOW_WIDE];
static unsigned char	 wide[HOW_WIDE * HOW_MANY];

/*******************************************************
	ipresent offers one segment from each connection
	in a BPF-style buffer array
	osserv skips the ended connections
	and returns the number of connections operating
*******************************************************/

static int ipresent(int dimension)
{
   return 0;
}

/*******************************************************
	iprogress writes outputs to files
        and replaces acknowledged segments in the
	input array from the input traffic file
*******************************************************/

static int iprogress()
{
   return 0;
}

static int irequest(unsigned char response[])
{
   /***************************************************
	an output queue from osserv
   ****************************************************/
   return 0;
}

int main(int argc, char *argv[])
{
   int			 x,
			 dimension;

   if (argc > 1)
   {
      fi = open(argv[1], O_RDONLY, 0777);
      x = errno;
      fo = open(argv[2], O_RDWR, 0777);
      if      (fi < 0) printf(" input traffic open error %d\n", x);
      else if (fo < 0) printf("output traffic open error %d\n", errno);
      else
      {
         dimension = read(fi, wide, HOW_WIDE * HOW_MANY);

         for (;;)
         {
            x = ipresent(dimension);
            if (x == 0) break;
            
            for (;;)
            {
               x = irequest(response);
               if (x == 0) break;
               iprogress();
            }
         }
      }
   }
   else printf("argument 1 must be a test traffic file\n"
               "argument 2 must be an output capture file\n");

   return 0;
}

