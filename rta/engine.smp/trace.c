#if	0
#include "smp.h"
#include "emulate.h"
#endif

void trace(int *list1, int *list2, int *list3, smp *xcore)
{
   if (list1)
   {
      _register[256] = list1[0];
      _register[257] = list1[1];
      _register[258] = list1[2];
      _register[259] = list1[3];
      _register[260] = list1[4];
      _register[261] = list1[5];
      _register[262] = list1[6];
      _register[263] = list1[7];
   }

   if (list2)
   {
      _register[264] = list2[0];
      _register[265] = list2[1];
      _register[266] = list2[2];
      _register[267] = list2[3];
      _register[268] = list2[4];
      _register[269] = list2[5];
      _register[270] = list2[6];
      _register[271] = list2[7];
   }

   if (list3)
   {
      _register[272] = list3[0];
      _register[273] = list3[1];
      _register[274] = list3[2];
      _register[275] = list3[3];
      _register[276] = list3[4];
      _register[277] = list3[5];
      _register[278] = list3[6];
      _register[279] = list3[7];
   }
}
