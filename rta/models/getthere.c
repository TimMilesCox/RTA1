#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../util/include/argue.h"

#define	LINE	8
#define	LINES	8
#define	PAGE	LINE * LINES
#define	POINTERS 1024
#define	BLOCK	POINTERS * sizeof(char *)
#define	COMMAND	72
#define	MIDSQUARE

#ifdef	MIDSQUARE
static int midsquare(int pages)
{
   int		 x = 0x200000;

   while (x)
   {
     x >>= 1;
     if (pages > x) break;
   }

   return x;
}
#endif

int main(int argc, char *argv[])
{
   int		 f, x, y, lines, symbol;

   int		 records = 0,
		 blocks = 0,
                 pages = 0;

   int		 point = 0,
		 line = -1,
		 variation;

   char		**p;

   char		***q,
		***top_index;

   char		*data,
		*iput;

   char		 command[COMMAND];

   argue(argc, argv);

   if (arguments)
   {
      f = open(argument[0], O_RDONLY);
      {
        if (f < 0) printf("data load error\n");
        else
        {
           q = calloc(1, BLOCK);

           if (q)
           {
              top_index = q;

              while  (q < (top_index + POINTERS))
              {
                 p = calloc(1, BLOCK);
                 if (p == NULL) break;
                 *q = p;
                 blocks++;

                 y = POINTERS;
                 while (y--)
                 {
                    data = calloc(1, PAGE);
                    if (data == NULL) break;
                    pages++;
                    *p = data;

                    lines = LINES;
                    while (lines--)
                    {
                       x = read(f, data, 7);
                       if (x < 7) break;
                       records++;
                       data += 8;
                    }
                    if (x < 7) break;
                    p++;
                 }
                 if (data == NULL) break;
                 if (x < 7) break;
                 q++;
              }

              printf("initial load %d records\n", records);
              if (p == NULL) printf("next pointer block unavailable\n");
              if (data == NULL) printf("next page unavailable\n");

              if (flag['w'-'a'])
              {
                 for (x = 0; x < blocks; x++)
                 {
                    p = top_index[x];                 
                    printf("[%d %p %p %p]\n", x, top_index, p, *p);
                 }
              }

              for (;;)
              {
                 iput = fgets(command, COMMAND, stdin);
                 if (iput == NULL) break;
                 symbol = *iput;
                 if (symbol == '.') break;

                 switch (symbol)
                 {
                    case '=':
                       #ifdef MIDSQUARE
                       point = midsquare(pages);
                       #else
                       point = pages >> 1;
                       #endif

                       variation = point << 1;

                       while (variation)
                       {
                         /*****************************************************

				< first in page ?

                         *****************************************************/

                          line = 0;

                          variation >>= 1;

                          #ifdef MIDSQUARE
                          if (point < pages)
                          {
                          }
                          else
                          {
                             point -= variation;
                             continue;
                          }
                          #endif

                          x = point >> 10;
                          y = point & 1023;
                          p = top_index[x];
                          data = p[y];
                          if (flag['w'-'a']) printf("[%x/%x %x:%x %p]%s", x, y, point, variation, p, data);

                          x = memcmp(command + 1, data, 6);

                          if ((x < 0) || (*data == 0))
                          {
                             /*************************************************
				zero data line at last page 1st slot = dummy.
				Treat it as higher than the candidate key

				for real, the page would either not be there
				or it would be initial

				either way the above comparison would not happen.
				The not-record would be treated as > end = "higher"
                             *************************************************/

                             if (point < variation) break;
                             point -= variation;
                             continue;
                          }


                         /*****************************************************

                                lucky hit time ?

                         *****************************************************/


                          if (x == 0) break;
                     

                         /*****************************************************

                                > last in page ?

                         *****************************************************/



                          x = memcmp(command + 1, data + LINE * LINES - LINE, 6);

                          printf("[%x >7?] %s", x, data + LINE * LINES - LINE);


                          line = LINES - 1;

                         /*****************************************************

                               lucky hit time ?

                         *****************************************************/



                          if (x == 0)
                          {
                             data += LINE * LINES - LINE;
                             break;
                          }

                          if (x > 0)
                          {
                             /*************************************************

				the chop may eventually bring us back to
				page + 1, but only at its last pass

				then that page must be accepted even if
				key < 1st record

                             ************************************************/

                             
                             if (variation == 0)
                             {
                                point++;
                                if (point < pages)
                                {
                                   line = 0;
                                   p = top_index[point >> 10];
                                   data = p[point & 1023];
                                }
                                else data = "@end";
                                continue;
                             }

                             point += variation;
                             continue;
                          }

                          line = 0;

                         /*****************************************************

                                we have hit the page and are > first record
				and < last record

                         *****************************************************/

                          while (line < LINES - 1)
                          {
                             line++;
                             data += LINE;
                             if (*data == 0) break;

                             /************************************************

				if the page contents change to zeroes
				treat them as higher than the candidate key

				for real the end of chain would be detected

                             ************************************************/

                             x = memcmp(command + 1, data, 6);
                             printf("[%x %x] %s", x, line, data);

                             if (x < 0) break;
                             if (x == 0) break;
                          }

                          break;
                       }

                       printf("%x:%x %s", point, line, data);
                       break;

                    case '\n':
                       if (line < 0)
                       {
                          p = *top_index;
                          data = *p;
                       }
                       else data += LINE;

                       line++;

                       if (line > LINES - 1)
                       {
                          line = 0;
                          point++;

                          if (point < pages)
                          {
                             x = point >> 10;
                             y = point & 1023;
                             p = top_index[x];
                             data = p[y];
                          }
                          else
                          {
                             point = 0;
                             p = *top_index;
                             data = *p;
                          }
                       }

                       printf("%x:%x %s", point, line, data);
                       break;

                    default:
                       break;
                 }
              }
           }
           else printf("top array block not acquired\n");

           close(f);
        }
      }
   }
   else printf("a file of load data is needed\n");
   return 0;
}
