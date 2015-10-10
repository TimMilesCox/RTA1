#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../util/include/argue.h"

#define	LINE	8
#define	LINES	8
#define	PAGE	LINE * LINES
#define	POINTERS 1024
#define	INDEX_POINTERS	1024
#define	BLOCK	POINTERS * sizeof(char *)
#define	TOP_INDEX INDEX_POINTERS * sizeof(indexp)
#define	COMMAND	72
#define	MIDSQUARE

typedef struct {  int				pages;
		  char				  **p; } indexp;

typedef struct {  int				space;
		  int			       *first,
						*last;
		  char	 data[PAGE - 3 * sizeof(int)]; } page;


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
                 pages = 0,
		 top_pages;

   int		 point = 0,
		 line = -1,
		 variation;

   int		 pointers = POINTERS,
		 intra;

   char		**p;

   indexp	*q,
		*top_index;

   char		*data,
		*iput,
		*oput,
		*newpage;

   char		 command[COMMAND];

   argue(argc, argv);

   if (arguments)
   {
      if (arguments > 1)
      {
         sscanf(argument[1], "%d", &pointers);
         if (pointers < 1) pointers = 1;
         if (pointers > POINTERS) pointers = pointers;
      }

      f = open(argument[0], O_RDONLY);
      {
        if (f < 0) printf("data load error\n");
        else
        {
           q = calloc(1, TOP_INDEX);

           if (q)
           {
              top_index = q;

              while  (q < (top_index + INDEX_POINTERS))
              {
                 p = calloc(1, BLOCK);
                 if (p == NULL) break;
                 q->p = p;
                 blocks++;

                 intra = 0;
                 y = pointers;
                 while (y--)
                 {
                    data = calloc(1, PAGE);
                    if (data == NULL) break;

                    intra++;
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

                 q->pages = intra;
                 pages += POINTERS;
                 top_pages = pages - POINTERS + pointers - y;
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
                    p = top_index[x].p;                 
                    printf("[%d %p %p %p] %s", x, top_index, p, *p, *p);
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
                    case '+':
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
                             if (flag['w'-'a']) putchar('^');
                             point -= variation;
                             continue;
                          }
                          #endif

                          x = point >> 10;
                          y = point & 1023;
                          p = top_index[x].p;

                          data = p[y];

                          if (data == NULL)
                          {
                             if (flag['w'-'a']) putchar('*');
                             point -= variation;
                             continue;
                          }

                          if (flag['w'-'a']) printf("[%x/%x %x:%x %p]%s", x, y, point, variation, p, data);

                          x = memcmp(command + 1, data, 6);

                          if (flag['w'-'a']) printf("[%d]\n", x);

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

                          iput = data + LINE * LINES - LINE;
                          while (*iput == 0) iput -= LINE;

                          /****************************************************
				this part will get replaced by proper logic
				last instance of this record type on page
                          ****************************************************/

                          if (iput < data)
                          {
                             printf("bad anomaly\n");
                             exit(0);
                          }

                          x = memcmp(command + 1, iput, 6);

                          if (flag['w'-'a']) printf("[%x >7?] %s", x, iput);


                          line = (iput - data) / LINE;

                          /*****************************************************

                               lucky hit time ?

                          *****************************************************/



                          if (x == 0)
                          {
                             data = iput;
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
                                if (point < top_pages)
                                {
                                   line = 0;
                                   p = top_index[point >> 10].p;
                                   data = p[point & 1023];

                                   if (data == NULL)
                                   {
                                      point += 1023;
                                      point &= -1024;
                                      if (point < pages)
                                      {
                                         p = top_index[point >> 10].p;
                                         data = *p;
                                      }
                                      else
                                      {
                                         p = top_index->p;
                                         data = *p;
                                      }
                                   }
                                }
                                else
                                {
                                   p = top_index->p;
                                   data = *p;
                                }
                                variation = 1;
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
                             if (flag['w'-'a']) printf("[%x %x] %s", x, line, data);

                             if (x < 0) break;
                             if (x == 0) break;
                          }

                          break;
                       }

                       if (symbol == '+')
                       {
                          if (flag['w'-'a']) printf("+[%p %p]\n", p, data);


                          if (data == NULL)
                          {
                             /**********************************************
				in this model this should mean
				insert is beyond the last existing record

				logic using a chain will be much simpler

				there may be space in the highest page
				and if there are interative inserts beyond
				the end, you don't need a new page for each
                             **********************************************/

                             point = top_pages - 1;
                             q = top_index + (point >> 10);
                             p = q->p;
                             data = p[point & 1023];

                             printf("++[%p %p %2.2x]\n", p, data, *data);

                             for (line = 0; line < LINES; line++)
                             {
                                if (*data == 0) break;
                                data += LINE;
                             }

                             if (line < LINES) memcpy(data, command + 1, LINE);
                             else
                             {
                                data = calloc(1, PAGE);
                                if (q->pages < pointers)
                                {
                                   top_pages++;
                                   point++;
                                   q->pages++;
                                   p[point & 1023] = data;
                                   memcpy(data, command + 1, LINE);
                                }
                                else
                                {
                                }
                             }
                          }


                          else if (*data == 0) memcpy(data, command + 1, LINE);
                          else if (x < 0)
                          {
                             iput = (p[point & 1023]);
                             line = (data - iput) / LINE;

                             if (*(iput + LINES * LINE - LINE) == 0)
                             {
                                /*******************************************
					space in this page
					data is @ next higher record
                                *******************************************/

                                iput += LINES * LINE - LINE;

                                while (iput > data)
                                {
                                   iput -= LINE;
                                   memcpy(iput + LINE, iput, LINE);
                                }

                                memcpy(data, command + 1, LINE);
                             }
                             else
                             {
                                /********************************************

				if new record < first in page, new page
				inserted before old page

				otherwise

				split, pointer data -> next higher record. A new 
				page is sequenced after the page for insert, so
				records before the new record do not reposition

				physical page split chosen. Array pointers
				need only be relative page numbers within area

				and this will work for data files as well as
				network database

                                ********************************************/                              

                                newpage = calloc(1, PAGE);

                                if (line == 0) data = newpage;
                                else
                                {
                                   p[point & 1023] = newpage;

                                   /****************************************
					swap the new and old pointers, so the
					pointer to incremented old page is
					reinserted before the pointer to the
					new page containing higher keyed
					records than the new record
                                   ****************************************/

                                   oput = data;
                                   x = line;

                                   while (x < LINES)
                                   {
                                      if (flag['w'-'a'])
                                      {
                                         printf("[%p > %p %2.2x]", oput, newpage, *oput);
                                         printf("(Z > %p]\n", oput);
                                      }

                                      memcpy(newpage, oput, LINE);
                                      memset(oput, 0, LINE);
                                      newpage += LINE;
                                      oput += LINE;
                                      x++;
                                   }

                                   newpage = iput;
                                   /***************************************
					that is really the old page
					containing the new record
                                   ***************************************/
                                }

                                memcpy(data, command + 1, 8);

                                y = q->pages;

                                /*******************************************
				the new page containing higher keyed records
				is inserted after the page with the new record
                                *******************************************/

                                if (y < POINTERS)
                                {
				   q->pages++;
                                   while (y > (point & 1023))
                                   {
                                      y--;
                                      p[y + 1] = p[y];
                                   }
                                   p[y] = newpage;
                                }
                                else
                                {
                                   /***************************************
					not complete
					a pointer block must be split unless
					the new page with one record is 
					between the ranges of two adjacent
					pointer blocks
                                   ***************************************/

                                   p = calloc(1, BLOCK);
                                   *p = newpage;
                                   x = blocks++;
                                   
                                   while (x > (point >> 10))
                                   {
                                      top_index[x + 1] = top_index[x];
                                      x--;
                                   }
                                   top_index[x + 1].p = p;
                                   top_index[x + 1].pages = 1;
                                }
                             }
                             x = 0;
                          }
                       }

                       printf("%x %x:%x %s", x, point, line, data);
                       break;

                    case '\n':
                       if (line < 0)
                       {
                          p = top_index->p;
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
                             p = top_index[x].p;
                             data = p[y];

                             if (data == NULL)
                             {
                                if (x < ((pages + 1023) >> 10) - 1)
                                {
                                   x++;
                                   point = x << 10;
                                   p = top_index[x].p;
                                }
                                else
                                {
                                   point = 0;
                                   p = top_index->p;                                   
                                }
                                data = *p;
                             }
                          }
                          else
                          {
                             point = 0;
                             p = top_index->p;
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
