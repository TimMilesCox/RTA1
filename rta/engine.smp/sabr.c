/*********************************************************
	this is a separate file from rta.c because
	it may need icc default optimisations suppressed
	which elsewhere are optimal
*********************************************************/

#include "emulate.h"
#include "smp.h"

extern device	 devices[];

int csabr(int ea, smp *xcore)
{
   int		*register_set = xcore->register_set;
   int		 v = a;
   int		 target = v & 0x003FFFFF;
   int		 range;
   int		 device_index;
   int		 descriptor;
   int		 isr;

   page		*tcbp;
   word		*reloadp;

   if (ea < 2) return -1;
   if (ea > 63) return -1;

   if ((v & 0x00400000) && ((device_index = v & 63)))
   {
      isr = psr & 0x00800000;

      if (v & 0x00800000)
      {
         if (isr == 0) return -1;

         /*******************************************
		illegal device name
		used to close memory windows
		fall thru and apply
         *******************************************/
      }
      else
      {
         descriptor = base[device_index+128];
         target &= 0x003FFFC0;

         if (descriptor & 0x00400000)
         {
            /****************************************
		network trunk or file system
            ****************************************/

            if (isr)
            {
               /*************************************
		you are an ISR
               *************************************/
            }
            else return -1;

            if ((target >> 6)  > (descriptor & 65535))
            {
               /*************************************
		block out of range
                replace with non-block
               *************************************/

               v = 0x00C00001;
            }
            
         }
         else if (descriptor & 0x00800000)
         {
            /****************************************
		memory array device
            ****************************************/

            if (target > (descriptor & 0x003FFFFF))
            {
               /*************************************
		block beyond highest page
		replace
               *************************************/

               v = 0x00C00001;
            }
         }
         else return -1;

         /*******************************************
		no device
         *******************************************/
      }
   }
   else
   {
      if (v & 0x00800000)
      {
         /*******************************************
		not a peripheral bit 23 not yet used
		reserved for gate-driven selections
         *******************************************/

         v = 0x00C00001;
      }

      /*********************************************
		device is executable space
                all pages allowed
		except ISR space in low memory
      *********************************************/

      range = base[128] & 0x003FFFFF;

      if (target < base[72]) v = 0x00C00001;
      if (target > range)   v = 0x00C00001; 
   }

   base[ea] = v;

   tcbp = devices[0].pages + base[65];
   reloadp = tcbp->w + BASE_TABLE + ea;

   reloadp->t3 = v;
   reloadp->t2 = v >> 8;
   reloadp->t1 = v >> 16;
   reloadp->parity = 0;

   return 0;
}

