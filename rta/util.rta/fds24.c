/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file fds24.c is free software.

    fds24.c is a utility program for examining instruction
    code or data files and file system images which have been
    generated for the freeware processor architecture

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


#include <stdio.h>
#include <fcntl.h>
#include <string.h>

typedef struct { char		b[16]; } frame;

typedef struct { frame   	 f[4]; } window;

typedef struct { unsigned short     u;
                 char              *p; } argum;
                 
typedef struct { argum          s[12]; } argil;

static char option[26];
static argil s;
static window data;

char *rline(int x)
{
   static char data[384];

   int y = 0;

   while (y < 383)
   {
      if (read(x, &data[y], 1) != 1) break;
      if (data[y++] == 10) break;
   }
   if (y == 0) return NULL;
   data[y] = 0;
   return data;
}

char *fline(int x)
{
   static char data[384];

   int y = 0;

   while (y < 383)
   {
      if (read(x, &data[y], 1) != 1) break;
      if (data[y] == 10) break;
      y++;
   }
   if (y == 0) return NULL;
   data[y] = 0;
   return data;
}

int main(int argc, char *argv[])
{
   int v, x, y = 0, print, maybe, length, symbol;
   char *p, *q, *r = argv[1], *arg;
   int f = 0;

   off_t	cursor = 0;
   
   if (argc > 1)
   {
      q = argv[1];
      symbol = *q++;
      if (symbol == '-')
      {
         while (symbol = *q++)
         {
            if ((symbol > 0x60) && (symbol < 0x7B)) symbol &= 0x5F;
            if ((symbol > 0x40) && (symbol < 0x5B)) option[symbol-'A'] = 1;
         }
      }
   }
   
   if (argc > 2)
   {
      q = argv[2];
      symbol = *q++;
      if (symbol == '-')
      {   
         while (symbol = *q++)
         {
            if ((symbol > 0x60) && (symbol < 0x7B)) symbol &= 0x5F;
            if ((symbol > 0x40) && (symbol < 0x5B)) option[symbol-'A'] = 1;
         }
      }
   }
   

   for (x = 3; x < argc; x++)
   {
      if (x > 11) break;
      q = argv[x];
      s.s[y].u = strlen(q);
      s.s[y].p = q;
      y++;
   }

   if ((argc) && (*r != '-')) f = open(r, O_RDONLY);
   
   while (f > -1)
   {
      if (!f)
      {
         r = fline(0);
         if (!r) break;
         f = open(r, O_RDONLY);
         if (f < 0)
         {
            printf("%s not opened\n", r);
            f = 0;
            continue;
         } 
      }
      
      cursor = 0;

      if ((argc > 2) && (*argv[2] > 0x2A) && (*argv[2] < 0x3A))
      {
         if (*argv[2] == '0') sscanf(argv[2], "%llx", &cursor);
         else                 sscanf(argv[2], "%lld", &cursor);
         cursor *= 3;
         lseek(f, cursor, SEEK_SET);
      }

      for(;;)
      {
         data.f[0] = data.f[1];
         data.f[1] = data.f[2];
         data.f[2] = data.f[3];
         
         v = read(f, data.f[3].b, 12);

         print = 3;
         if (y)
         {
            print = 4;
            for (x = 0; x < y; x++)
            {
               maybe = 4;
               arg = s.s[x].p;
               length = s.s[x].u;
               q = data.f[2].b;
               if (length > 33) q = data.f[1].b;
               if (length > 49) q = data.f[0].b;
               while (q + length < data.f[3].b + 17)
               {
                  if (memcmp(q, arg, length) == 0)
                  {
                     if (q >= data.f[3].b) maybe = 3;
                     if ((q < data.f[3].b) && (q + length > data.f[3].b))
                        maybe = 2; 
                     if ((q < data.f[2].b) && (q + length > data.f[3].b))
                        maybe = 1; 
                     if ((q < data.f[1].b) && (q + length > data.f[3].b))
                        maybe = 0; 
                  }
                  q++;
               }
               if (maybe < print) print = maybe;  
            }
         }
         
         while (print < 4)
         {
            if (option['p'-'a']) printf("%s ", r);
         
            printf("%8.8llx:", (cursor - 12 * (3 - print))/3);
            for (x = 0; x < 12; x++)
            {
               symbol = data.f[print].b[x];
               if (!(x % 3)) printf(" ");
               if ((print < 3) || (x < v)) printf("%2.2x", data.f[print].b[x]);
               else       printf("  ");
            }
            printf("  \"");
            for (x = 0; x < 12; x++)
            {
               if ((print == 3) && (x == v)) break;
               symbol = data.f[print].b[x];
               if ((symbol > 32) && (symbol < 127)) printf("%c", symbol);
               else                                 printf(" ");
            }
            printf("\"");
            print++;
         }

         cursor += 12;
         if (v < 12) break;
         if (option['g'-'a'])
         {
            printf("\n");
            continue;
         }
         else
         {
             fflush(stdout);
         }

         p = rline(0);
         if (!p) continue;
         if (*p == '.') break;
         if ((*p > 0x2F) && (*p < 0x3A))
         {
            if (*p == '0') sscanf(p, "%llx", &cursor);
            else           sscanf(p, "%lld", &cursor);
            cursor *= 3;
            lseek(f, cursor, SEEK_SET);
         }
      }
      close(f);
      f = 0;
      if ((argv[1]) && (*argv[1] != '-')) break;
   }
   printf("\n");
   return 0;
}
