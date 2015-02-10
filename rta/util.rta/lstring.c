/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file lstring.c is free software.

    lstring.c is a utility program for constructing target instruction
    code for the freeware processor architecture

                RTA1

    RTA1 is a free processor architecture design.

    The executable emulation of RTA1 is free software.

    Instruction code for the target RTA1 architecture is free software
    if it is delivered with this software

    Software programs delivered with this software to connect the
    emulated RTA1 with real network interfaces in the emulator host
    are free software

    Scripts and programs delivered with this software for running
    on other computers and interacting with the RTA1 are free software

    The masmx target-independent meta-asembler delivered here
    is free software whether it is used for constructing RTA1 code
    or any other code. masmx is a separate free program by the
    same author and included with this software to save searching for it

    Scripts and utility programs for constructing RTA1 target
    executable software are free software

    You can redistribute and/or modify RTA1 and its materials
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RTA1 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with RTA1.  If not, see <http://www.gnu.org/licenses/>.

**********************************************************************/




#ifdef DOS
#include <stdio.h>
#include <io.h>
#include <dos.h>
#include <fcntl.h>
#include <sys\stat.h>
#else
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#define	BLOCKSUM

#define DEFAULT_LINE	4096
#define S_RECORD	'S'
#define BLOCK_BOOT	'B'
#define S_FORMAT	'F'

static char include[72] = { 1,1,1,1,1,1,1,1,1,1,1,1, 
                            1,1,1,1,1,1,1,1,1,1,1,1,
                            1,1,1,1,1,1,1,1,1,1,1,1,
                            1,1,1,1,1,1,1,1,1,1,1,1,
                            1,1,1,1,1,1,1,1,1,1,1,1,
                            1,1,1,1,1,1,1,1,1,1,1,1 } ;

static unsigned long long bankp[72];

static char				 flag[28];
static int				 format = S_RECORD;
static int				 columns = 8-1;
static int				 first = 73;

void exout(int locator, long loc, char *b, int c, int handle, int flag)
{
   static unsigned long long loc0 = 0;
   static char data[3*4096+16];
   static char plen[4];
   static int index=12, sum=0, p=0;
   register char digit;

   int					 t1, t2, t3;

   #ifdef BLOCKSUM
   int					 carry;
   #endif

   if (c % 3) printf("octet count wrong %d\n", c);
   c /= 3;

   if (!include[locator]) return;

   switch (format)
   {
      case BLOCK_BOOT:

         if ((loc) || (first != locator) || (flag == 7))
         {
            first = locator;

            if (p)
            {
               data[0] = p >> 16;
               data[1] = p >>  8;
               data[2] = p;

               #ifdef BLOCKSUM
               sum += p;

               if (carry = (sum >> 24) & 255)
               {
                  sum += carry;
                  sum &= 0x00FFFFFF;
               }

               sum ^= 0x00FFFFFF;

               data[3] = sum >> 16; 
               data[4] = sum >>  8;
               data[5] = sum;
               #endif

               write(handle, data, index);
            }

            p = 0;
            loc0 = loc + bankp[locator];
            index = 12;

            sum = (loc0 & 0x00FFFFFF) + ((loc0 >> 24 ) & 0x00FFFFFF);

            data[6]  = loc0 >> 40;
            data[7]  = loc0 >> 32;
            data[8]  = loc0 >> 24;

            data[9]  = loc0 >> 16;
            data[10] = loc0 >>  8;
            data[11] = loc0;

            if (flag == 7)
            {
               data[0] = 0;
               data[1] = 0;
               data[2] = 0;

               sum ^= 0x00FFFFFF;
               data[3] = sum >> 16;
               data[4] = sum >>  8;
               data[5] = sum;

               write(handle, data, 12);
               return;
            }
         }

         while (c--)
         {
            t1 = *b++;
            t2 = *b++;
            t3 = *b++;

            data[index++] = t1;
            data[index++] = t2;
            data[index++] = t3;

            #ifdef BLOCKSUM
            sum += (t1 << 16) |  (t2 << 8) | t3;
            carry = sum >> 24;
            sum &= 0x00FFFFFF;
            sum += carry;
            #endif

            p++;

            if (p > columns)
            {
               data[0] = p >> 16;
               data[1] = p >>  8;
               data[2] = p;

               #ifdef BLOCKSUM
               sum += p;
               
               if (carry = (sum >> 24) & 255)
               {
                  sum += carry;
                  sum &= 0x00FFFFFF;
               }

               sum ^= 0x00FFFFFF;

               data[3] = sum >> 16;
               data[4] = sum >>  8;
               data[5] = sum;
               #endif

               write(handle, data, index);
               loc0 += p;
               sum = (loc0 & 0x00FFFFFF) + ((loc0 >> 24) & 0x00FFFFFF);
               index = 12;
               p = 0;

               data[6]  = loc0 >> 40;
               data[7]  = loc0 >> 32;
               data[8]  = loc0 >> 24;

	       data[9]  = loc0 >> 16;
               data[10] = loc0 >>  8;
               data[11] = loc0;
            }
         }

         #ifdef BLOCKSUM
         if (carry = (sum >> 24) & 255)
         {
            sum += carry;
            sum &= 0x00FFFFFF;
         }
         #endif

         if  ((flag == 1) && (p))
         {
            data[0] = p >> 16;
            data[1] = p >>  8;
            data[2] = p;

            #ifdef BLOCKSUM
            sum += p;

            if (carry = (sum >> 24) & 255)
            {
               sum += carry;
               sum &= 0x00FFFFFF;
            }

            sum ^= 0x00FFFFFF;

            data[3] = sum >> 16;
            data[4] = sum >>  8;
            data[5] = sum;
            #endif

            write(handle, data, index);
            loc0 += p;
            sum = (loc0 & 0x0FFFFFF) + ((loc0 > 24) & 0x00FFFFFF) ;
            index = 12;
            p = 0;
         }

         break;

      case S_RECORD:

         if ((loc) || (first != locator) || (flag == 7))
         {
            first = locator;
            if (p)
            {
               sum += p*3;
               sum += 4;
               sum ^= -1;
               data[0] = 'S';
               data[1] = '2';
               sprintf(plen, "%2.2X", p*3+4);
               data[2] = plen[0];
               data[3] = plen[1];
               index += sprintf(&data[index], "%2.2X\n", sum & 255);
               write(handle, data, index);
            }

            sum = (loc>>16)+(loc>>8)+loc;
      
            p = 0;
            loc0 = loc;
            index = 12;

            if (flag == 7)
            {
               sprintf(data, "S804%6.6llX", loc0);
               sum += 4;
               sum ^= -1;
               sum &= 255;
               index = 12+sprintf(&data[12], "%2.2X\n", sum);
               write(handle, data, index);
               return;
            }
      
            sprintf(data, "S200%6.6llX", loc0);
         }
   
         while (c--)
         {
            t1 = *b++;
            t2 = *b++;
            t3 = *b++;

            sum += t1 + t2 + t3;
            index += sprintf(&data[index], "%2.2X", t1);
            index += sprintf(&data[index], "%2.2X", t2);
            index += sprintf(&data[index], "%2.2X", t3);
            p++;

            if (p > columns) 
            {
               sum += p*3;
               sum += 4;
               sum ^= -1;
               data[0] = 'S';
               data[1] = '2';
               sprintf(plen, "%2.2X", p*3+4);
               data[2] = plen[0];
               data[3] = plen[1];
               index += sprintf(&data[index], "%2.2X\n", sum & 255);
               write(handle, data, index);
               loc0 += p;
               sum = (loc0>>16)+(loc0>>8)+loc0;
               sprintf(data, "S200%6.6llX", loc0);
               index = 12;
               p = 0;
            }
         }
   
         if  ((flag == 1) && (p))
         {
            sum += p*3;
            sum += 4;
            sum ^= -1;
            data[0] = 'S';
            data[1] = '2';
            sprintf(plen, "%2.2X", p*3+4);
            data[2] = plen[0];
            data[3] = plen[1];
            index += sprintf(&data[index], "%2.2X\n", sum & 255);
            write(handle, data, index);
            loc0 += p;
            sum = (loc0>>16)+(loc0>>8)+loc0;
            sprintf(data, "S200%6.6llX", loc0);
            index = 12;
            p = 0;
         }

         break;
   }
}

main(int argc, char *argv[])
{
  int			 i, j;
  char			 *filename[2] = { NULL, NULL } ;

  int c, d, locator, e, f, symbol, x, y = 0, interval;
  long loc, transfer, offset = 0, index;

  unsigned long long bank;

  char data[256];
  char internal[128];
  char *p;
  
  register char digit1, digit2;

  for (x = 1; x < argc; x++)
  {
      p = argv[x];

      symbol = *p;

      if (symbol == '-')
      {
         p++;

         while (symbol = *p++)
         {
            symbol |= 0x20;
            if ((symbol > 0x60) && (symbol < 0x7B)) flag[symbol - 'a'] = 1;
         }
      }
      else
      {
         if (y < 2) filename[y++] = p;
      }
  }
  
  if (y < 2)
  {
     printf("an input and an output filename are required\n");
     return 0;
  }

  y = 0;

  #ifdef DOS
  i = open(filename[0], O_RDONLY|O_TEXT);
  j = open(filename[1], O_WRONLY|O_CREAT|O_TRUNC|O_TEXT, S_IREAD|S_IWRITE);
  #else
  i = open(filename[0], O_RDONLY);
  j = open(filename[1], O_WRONLY | O_CREAT |O_TRUNC,
                        S_IREAD  | S_IWRITE|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
  #endif

  if (i < 1)
  {
     printf("input file %s unavailable\n", filename[0]);
     return i;
  }

  if (j < 1) 
  {
     printf("output file %s not written\n", filename[1]);
     return j;
  }

  if (flag['b' - 'a']) format = BLOCK_BOOT;

  switch (format)
  {
     case BLOCK_BOOT:
        columns = DEFAULT_LINE - 1;
        break;
     case S_RECORD:
        write(j, "S00600004844521B\n", 17);
        break;
     default:
        break;
  }
  
  for (;;)
  {
    c = 0;
    for (;;)
    {
      d = read(i, &data[c], 1);
      if (!d) break;
      if (data[c] == 0x0d) continue;
      if (data[c] == 0x0a) break;
      c++;
    }
    /*
    if (!d) break;
    */
    if (!c)
    {
      if (!d) break;
      continue;
    }
    if (data[0] == ':') continue;
    if (data[0] == '+') continue;
    if (data[0] == '.') continue;

    if (data[0] == '@')
    {
       if (sscanf(&data[1], ":%x:%llx", &x, &bank) < 2)
       {
          printf("bad bank pointer line\n");
          break;
       }

       if (flag['v'-'a']) printf("bankp %2.2x %12.12llx\n", x, bank);

       if ((x < 0) && (x > 71))
       {
          printf("location counter %d is not available, abandon\n", x);
          break;
       }

       first = 73;
       bankp[x] = bank;
       continue;
    }

    if (data[0] == '-')
    {
       p = data;
       while (symbol = *p++)
       {
          if (symbol == 0x0D) continue;
          if (symbol == 0x0A) break;
          putchar(symbol);
       }
       printf(" unresolved\n");
       continue;
    }
    
    data[c] = 0;
    if (data[0] == '$')
    {
       if (data[4] == '*')
       {
          printf("Relocatable Counter Not Used\n");
          continue;
       }
       offset = 0;
       x = sscanf(&data[1], "%x:%lx:%lx", &locator, &loc, &index);
       if (x < 2)
       {
          printf("bad location line\n");
          continue;
       }

       if (x > 2)
       {
          if (flag['v'-'a']) printf("$%2.2x:%6.6lx:%6.6lx\n", locator, loc, index); 
          bankp[locator] = (long long) loc << 12;
          loc = index;
       }

       exout(locator, loc, NULL, 0, j, 0); 
       continue;
    }
    if (data[0] == '>')
    {
       x = sscanf(&data[1], "%x:%lx", &locator, &transfer);
       if (x < 2)
       {
          printf("bad transfer line\n");
          continue;
       }
       y = 1;
       continue;
    }

    e = 0;
    interval = 0;
    p = data;
    while (digit1 = *p++)
    {
       if (digit1 == ':')
       {
          printf("$%2.2x:%6.6lx relocation information discarded %s\n",
                           locator, loc + offset + interval / 3, data);
          e = interval;
          continue;
       }

       if (digit1 == 32)
       {
          interval = e;
          continue;
       }

       digit1 -= 48;
       if (digit1>9)
       {
          digit1 &= 15;
          digit1 += 9;
       }
       digit1 <<= 4;
       digit2 = *p++;
       if (!digit2) break;
       digit2 -= 48;
       if (digit2>9)
       {
          digit2 &= 15;
          digit2 += 9;
       }
       digit1 |= digit2;
       internal[e++] = digit1;
    }

    offset += e / 3;
    exout(locator, 0, internal, e, j, 0);
    if (!d) break;
  }

  if (y) exout(locator, transfer, internal, e, j, 7);

  /* exout(0, 0, internal, c, j, 0); */
  exout(0, 0, NULL, 0, j, 1);
  close(i);
  close(j);
  return 0;
}
