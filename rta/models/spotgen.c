
/***********************************************************

	this models distribution of calc chain heads
	according to a variable algorithm
	in a primary area of given size

***********************************************************/



#include <stdio.h>
#include "util/include/argue.h"

#define	PAGES		4096
#define	MAX_PAGES	4096*4096
#define KEYBYTES	6
#define	MAX_KEYBYTES	24
#define	LINE		1024

static int	 pages = PAGES;
static int	 keybytes = KEYBYTES;
static int	 distance,
		 passes,
                 mask;

static int	 field = 8,
		 fmask = 255;

static unsigned short chain[MAX_PAGES];
static char		key[LINE];

static int plog(int pages)
{
   int		 fulcrum = pages - 1;
   int		 x = 0;

   for (;;)
   {
      if ((1 << x) > fulcrum) break;
      x++;
   }

   return x;
}

static int acquire(int offset)
{
   int		 bits = (keybytes * field) - offset;
   int		 byte = offset / field;
   int		 bias = offset % field;
   int		 scope;
   int		 x = 0;
   int		 symbol;

   if (bits > distance) bits = distance;
   scope = bits + bias;

   while (scope)
   {
      x <<= field;
      symbol = key[byte++];
      if (flag['w'-'a']) putchar(symbol);
      if (flag['r'-'a']) symbol >>= (8 - field);
      x |= symbol & fmask;
      scope -= field;
      if (scope < 0) break;
   }

   if (scope < 0) x >>= -scope;
   x &= (1 << bits) - 1;

   if (flag['w'-'a']) printf("[%x:%x]", bits, x);

   return x;
}

static void seed()
{
   int		 x = 0;
   int		 y = passes;
   int		 offset = 0;


   while (y--)
   {
      x += acquire(offset);
      offset += distance;
      x += x >> distance;
      x &= mask;
   }

   if (flag['w'-'a']) printf("[%x]\n", x);

   x &= mask;

   if (x < pages)
   {
   }
   else x >>= 1;

   chain[x]++;
}

int main(int argc, char *argv[])
{
   int		 x, y, z = 0;
   int           symbol, w;
   char		*p;
   long long	 inserts = 0;

   argue(argc, argv);

   if (flag['a'-'a']) field = 5;
   if (flag['b'-'a']) field = 6;
   if (flag['c'-'a']) field = 7;
   if (flag['d'-'a']) field = 4;
   if (flag['e'-'a']) field = 3;

   fmask = (1 << field) - 1;

   if (arguments)
   {
      sscanf(argument[0], "%d", &pages);
      if (pages < 1) pages = PAGES;
      if (pages > MAX_PAGES) pages = MAX_PAGES;

      if (arguments > 1)
      {
         sscanf(argument[1], "%d", &keybytes);
         if (keybytes < 1) keybytes = KEYBYTES;
         if (keybytes > MAX_KEYBYTES) keybytes = MAX_KEYBYTES;
      }
   }

   distance = plog(pages);
   mask = (1 << distance) - 1;

   #if 0
   distance--;
   #elif 0
   if ((distance & 3) == 0) distance--;
   #else
   if ((distance & 7) == 0) distance--;
//   if (distance > 15) distance = 15;
   #endif

   passes = (field * keybytes + distance - 1) / distance;

   for (;;)
   {
      p = fgets(key, LINE - 1, stdin);
      if (!p) break;
      symbol = *p;
      if (symbol == '.') break;

      if (symbol == '+')
      {
         sscanf(p, "%d", &keybytes);
         passes = (field * keybytes + distance - 1) / distance;
         continue;
      }

      inserts++;
      seed();
   }

   for (y = 0; y < pages; y += 16)
   {
      printf("%6.6x: ", y);

      for (x = 0; x < 16; x++)
      {
         w = chain[x | y];
         printf(" %4.4x", w);
         if (w == 0) z++;
      }
      putchar('\n');
   }

   printf("[%d:%d] %d unused pages %lld inserts\n", distance, passes, z, inserts);
   return 0;
}
