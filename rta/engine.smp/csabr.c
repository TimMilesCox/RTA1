/*********************************************************
	this is a separate file because it may need
	icc default optimisations suppressed
	which elsewhere are optimal
*********************************************************/

#include "emulate.h"
#include "smp.h"

extern device	 devices[];

extern int sabr(int value, int ea, int base_r[], int p_s_r, page *memory);

int csabr(int ea, smp *xcore)
{
   int		*register_set = xcore->register_set;
_register[280] = devices[0].pages;
   return sabr(a, ea, base, psr, devices[0].pages);
}
