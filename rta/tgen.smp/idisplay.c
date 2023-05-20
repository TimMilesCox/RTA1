/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is part of the user-interactive-subsystem
    of the software-emulation of the freeware processor architecture

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

    You can redistribute it and/or modify RTA1
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
#include "../engine.smp/emulate.h"
#include "../engine.smp/smp.h"
#include "idisplay.h"

#ifdef  LINUX
#ifndef	GCC
#include "../tgen.smp/_mnames.h"
#endif
#endif

#ifdef	RTA_SMP
extern	device		devices[];
#define	memory		devices[0].pages
#else
extern int		 _register[256];
extern int		 iselect;
extern system_memory	 memory;

extern int		 psr;
extern int		 b0_name;
extern page		*b0p;
extern word		*apc;
#endif

static char *ifile_1_name[32] = { "sr","sk","sx","sy",
                                  "sa","sb","z","pop",
                                  "lr","lk","lx","ly",
                                  "la","lb","tz","tp",
                                  "ax","ay","or","orB",
                                  "and","andB","xor","xorB",
                                  "aa","ab","ana","anb",
                                  "m","mf","d","push" } ;


static char *jname[16]         = { "",",t1",",t2",",t3",
                                  ",i",",xi",",unlikely",",unlikely",
                                   "", ",?t1", ",h1", ",h2",
                                  ",i",",xi",",unlikely",",unlikely" } ;


static char *i_o_name[16] = { "inA", "inB", "outA",       "outB",
                "reload",  "unassigned", "sabr",       "unassigned",
                "impossible", "impossible", "impossible", "impossible",
                "impossible", "impossible", "rex", "jdz"      } ;
   

static char *x_i_name[16] = { "on", "off", "rsr",        "wsr",
               "unassigned", "unassigned", "lret",       "fret",
               "impossible", "impossible", "impossible", "impossible", 
               "impossible", "impossible", "ii",         "ir" } ;


static char *shift_j_name[32] = { "sar","sbr","dsr","jdr",
                                  "sal","sbl","dsl","lcal",
                                  "rar","rbr","drr","jnc",
                                  "ral","rbl","drl","jc",
                                  "saa","sba","dsa","jao",
                                  "jpa","jpb","j",  "jpo",
                                  "jza","jzb","jnza","jnzb",
                                  "jna","jnb","jxge","jyge" } ;


static char *ifile_3_name[32] = { "ts","n","inc","dec",
                                  "sim","popA","src","slc",
                                  "qs","ql","dte","dpop",
                                  "fa","fan","fm","fd",
                                  "qpop","qpush","ex","dpush",
                                  "lsc","mta","sc","mlb",
                                  "ds","dl","da","dan",
                                  "dlsc","unassigned","go","call" } ;

static char *xname[8] = { ",impossible", ",impossible", ",x",	",y",
                          ",a",          ",b",		",fp",	",sp" } ;

void instruction_display(word *pc, int howmany, int linefeed, smp *xcore)
{
   int			 function,
   			 designator;

   char			*f,
			*j,
                        *itag,
                        *xtag;

   unsigned short	 address, index_tag;

   int			 net_pc = pc - b0p->w;


   if (net_pc < 0) return;

   while (howmany--)
   {
      designator = pc->t1 &  7;
      function   = pc->t1 >> 3;

      switch (designator)
      {
         case 6:
            f = shift_j_name[function];
            j = "";
            break;
         case 7:
            f = ifile_3_name[function];
            j = "";
            break;
         default:
            if ((function < 8) || (function == 14) || (function == 15))
            {
               if (designator == 4)
               {
                  f = i_o_name[function];
                  j = "";
                  break;
               }

               if (designator == 5)
               {
                  f = x_i_name[function];
                  j = "";
                  break;
               }
            }

            if (psr & HALFWORD) designator |= 8;

            f = ifile_1_name[function];
            j = jname[designator];
            break;
      }

      address = (pc->t2 << 8) | pc->t3;

      if ((designator & 7) == 5)
      {
            printf("[%6.6x:%6.6x]%2.2x%2.2x%2.2x\t%s%s\t%4.4x%s",
                    b0_name,
                    net_pc,
                    pc->t1, pc->t2, pc->t3,
                    f,
                    j,
                    address,
                    (linefeed) ? "\n" : " | ");
      }
      else
      {
         if (address & 32768)
         {
            itag = "";
            xtag = "";

            index_tag = address >> 12;
            index_tag &= 7;
            address &= 4095;

            if (index_tag < 2)
            {
               if ((address < 256) && (index_tag == 0))
               {
                  itag = "*";
               }
               else
               {
                  itag = (index_tag) ? "*B1+" : "*B0+";
               }
            }
            else
            {
               xtag = xname[index_tag];
            }
           

            printf("[%6.6x:%6.6x]%2.2x%2.2x%2.2x\t%s%s\t%s%3.3x%s%s",
                    b0_name,
                    net_pc,
                    pc->t1, pc->t2, pc->t3,
                    f,
                    j,
                    itag,
                    address,
                    xtag,
                    (linefeed) ? "\n" : " | ");
         }
         else
         {
            printf("[%6.6x:%6.6x]%2.2x%2.2x%2.2x\t%s%s\t%4.4x%s",
                    b0_name,
                    net_pc,
                    pc->t1, pc->t2, pc->t3,
                    f,
                    j,
                    address,
                    (linefeed) ? "\n" : " | ");
         }
      }

      net_pc++;
      pc++;
   }
}

