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
#include <string.h>
#include <io.h>
#include <dos.h>
#include <fcntl.h>
#include <sys\stat.h>
#else
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#endif

#include "../include.rta/argue.h"

#define	BLOCKSUM

#define DEFAULT_LINE	4096
#define S_RECORD	'S'
#define BLOCK_BOOT	'B'
#define S_FORMAT	'F'

static char include[84] = { 1,1,1,1,1,1,1,1,1,1,1,1, 
                            1,1,1,1,1,1,1,1,1,1,1,1,
                            1,1,1,1,1,1,1,1,1,1,1,1,
                            1,1,1,1,1,1,1,1,1,1,1,1,
                            1,1,1,1,1,1,1,1,1,1,1,1,
                            1,1,1,1,1,1,1,1,1,1,1,1,
                            0,0,1,1,1,1,1,1,1,1,1,1 } ;

static unsigned long long bankp[84];

static int				 format = S_RECORD;
static int				 columns = 8-1;
static int				 first = 73;

static void exout(int locator, long loc, char *b, int c, int handle, int xflag)
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

   if ((!include[locator]) && (xflag ^ 1)) return;

   switch (format)
   {
      case BLOCK_BOOT:

         if ((loc) || (first != locator) || (xflag == 7))
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

            if (xflag == 7)
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

         if  ((xflag == 1) && (p))
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

         if ((loc) || (first != locator) || (xflag == 7))
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

            if (xflag == 7)
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
   
         if  ((xflag == 1) && (p))
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

/**********************************************************************************
	inject a request to resolve storage page index[es] at later load

	a single code word designating a 4K-word storage page
	is marked for update with load location

        request applied to a single code word
	only one request like this per superword is actioned

	to do 2+ of these in a structure assembly must generate superwords
	which do not span > 1 of these requests

	assembly does multiword superwords for $form or for length suffix like :D

	word address relocations are resolved at earlier links
**********************************************************************************/

static void inject_physical_address_tuple(int handle, char *data, long long target)
{
   unsigned char request[12];

   int		 label_index;
   int		 field;
   int		 scale = 0;
   int		 superword = 0;
   int		 symbol;
   char		*rewind = data;

   int		 sum;

   if ((*data == '[') && (*(data + 5) == ']'))
   {
      sscanf(data + 1, "%x", &label_index);
      sscanf(data + 6, "%x", &field);

      data += 8;

      if (*data == '*')
      {
         if (data[1] == '/')
         {
            sscanf(data + 2, "%x", &scale);
            data += 4;
         }
         else
         {
            printf("load fixup scale encoding error: %s\n", rewind);
            return;
         }
      }

      if (*data++ == ':')
      {
         while (symbol = *data++)
         {
            if (symbol == ' ') break;
            if (symbol == '\n') break;
            if (symbol == '\r') break;

            if (symbol == ':')
            {
               printf("multiple load fixups must be "
                      "in separate assembly output superwords: %s\n", rewind);
               return;
            }

            superword += 4;
         }
      }
      else
      {
          printf("load fixup encoding dislocation: %s\n", rewind);
          return;
      }

      target += (superword - scale) / 24 - 1;

      label_index |= 0x00800000;
      sum = label_index + target + (target >> 24);
      sum ^= 0x00FFFFFF;
      
      request[0] = label_index >> 16;
      request[1] = label_index >>  8;
      request[2] = label_index;

      request[6]  = target >> 40;
      request[7]  = target >> 32;
      request[8]  = target >> 24;
      request[9]  = target >> 16;
      request[10] = target >> 8;
      request[11] = target;

      request[3] = sum >> 16;
      request[4] = sum >> 8;
      request[5] = sum;

      write(handle, request, 12);
   }
   else printf("%s only a load label index "
               "may be forwarded in binary strings\n", data);
}

/****************************************************
   a2zu(*, **)
   returns -1 if there are no digits before delimiter
   updates input pointer to address of delimiter
   only returns zero
   if the value of encountered digits is zero
****************************************************/

static int a2zu(char *p, char **q)
{
   int           symbol,
                 x = -1;

   while (symbol = *p++)
   {
      if (symbol == ' ') continue;

      if ((symbol > '0' - 1) && (symbol < '9' + 1))
      {
         x = symbol - '0';
         while (symbol = *p++)
         {
            if (symbol < '0') break;
            if (symbol > '9') break;
            x = x * 10 + symbol - '0';
         }
      }

      break;
   }
   p--;
   *q = p;
   return x;
}

int main(int argc, char *argv[])
{
  int			 i, j;

  int c, d, locator, e, f, symbol, x, y = 0, interval;
  long loc, transfer, offset = 0, index, aside;

  unsigned long long bank;

  char data[256];
  char internal[128];
  char *p,
       *q;
  
  register char digit1, digit2;

  argue(argc, argv);
  
  if (arguments < 2)
  {
     printf("an input and an output filename are required\n");
     return 0;
  }

  if (arguments > 2)
  {
     p = argument[2];

     if (*p++ == '(')
     {
        memset(include, 0, 72);
        x = 0;
        
        for (;;)
        {
           /****************************************************
              a2zu(*, **)
              returns -1 if there are no digits before delimiter
              updates input pointer to address of delimiter
              only returns zero
              if the value of encountered digits is zero
           ****************************************************/

           y = a2zu(p, &p);

           if (y < 0)
           {
           }
           else if (y > 71) printf("locator %d skipped\n", y);
           else
           {
              include[y] = 1;
              if (flag['v'-'a']) printf("locator %d included\n", y);
              x++;
           }

           symbol = *p++;

           if (symbol == ',') continue;
           break;
        }

        if (symbol ^ ')')
        {
           printf("\n\tsyntax undetermined. Abandon for safety. No files changed\n\n");
           return 0;
        }

        if (flag['v'-'a']) printf("%d locators included\n", x);
     }
     else
     {
        printf("\n\t3rd argument has the format \'(locator_number[,locator_number,...])\'\n"
               "\tthe apostrophes or another escape are necessary\n"
               "\tby default all locators included\n\n");
        printf("\tabandon for safety. No files changed\n\n");
        return 0;
     }
  }

  y = 0;

  #ifdef DOS
  i = open(argument[0], O_RDONLY|O_BINARY);
  j = open(argument[1], O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, S_IREAD|S_IWRITE);
  #else
  i = open(argument[0], O_RDONLY);
  j = open(argument[1], O_WRONLY | O_CREAT |O_TRUNC,
                        S_IREAD  | S_IWRITE|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
  #endif

  if (i < 1)
  {
     printf("input file %s unavailable\n", argument[0]);
     return i;
  }

  if (j < 1) 
  {
     printf("output file %s not written\n", argument[1]);
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
       printf(" %s\n", (uflag['L'-'A']) ? "forwarded" : "unresolved");
       continue;
    }
    
    data[c] = 0;

    if (data[0] == '$')
    {
       if (data[4] == '*')
       {
          locator = atoi(data + 1);
          if (include[locator]) printf("Relocatable Counter Not Used\n");
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
    q = p;

    while (digit1 = *p++)
    {
       if (digit1 == ':')
       {
          if (uflag['L'-'A'])
          {
             if (include[locator]) inject_physical_address_tuple(j, q,
             (unsigned long long) loc + offset + interval / 3);
             e = interval;
             continue;
          }             
          else
          {
             if (include[locator])
             printf("$%2.2x:%6.6lx relocation information discarded %s\n",
                              locator, loc + offset + interval / 3, data);
             e = interval;
             continue;
          }
       }

       if (digit1 == 32)
       {
          q = p;
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
