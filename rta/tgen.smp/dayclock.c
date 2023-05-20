#undef	DAYCLOCK_TRACE

static void dayclock(int dring, smp *xcore)
{
   struct timeval           xronos;

   #ifdef	__X64
   unsigned long		 clockwise;
   #endif

   #ifdef DAYCLOCK_TRACE
   if (flag['i'-'a'])
   {
      putchar(':');
      fflush(stdout);
   }

   if (uflag['I'-'A']) dring = 1;
   #endif

   if (dring == 0)
   {
      if (psr & 0x00800000) return;
   }

   #ifdef DAYCLOCK_TRACE
   if (flag['i'-'a']) 
   {
      putchar('+');
      fflush(stdout);
   }
   #endif

   gettimeofday(&xronos, NULL);

   #ifdef __X64

   clockwise = (xronos.tv_sec  * 1000)
             + (xronos.tv_usec / 1000);

   clockr[1] = clockwise & 0x00FFFFFF;
   clockr[0] = clockwise >> 24;

   #ifdef RATIO
   _register[DAYCLOCK]   = clockr[1];
   _register[DAYCLOCK_U] = clockr[0];
   #endif

   #else	/*	32-bit second values	*/

   /******************************************************
	32-bit second values from platform
   ******************************************************/

   /******************************************************
	for 32-bit execution with unchanged library
	on platforms the same age as this emulator or newer

	it's not known if tv_sec will really wrap for ever
	because systems may be set to do diagnostics instead

	but if tv_sec does keep wrapping:
	
	this tweak casts tv_sec to unsigned and prepends
	32 more bits which are all zeros until year 2110 approx.

	the 64-bit seconds-from-1970 count is store on file
	every 70 years when tv_sec high-order bit flips. 

	On unsigned overflow every 140 years
	the 32-bit prepend is incremented

	The 2 words on file are read every emulator startup
	and written once per 70 years
   ******************************************************/

   if (uflag['Y'-'A'] == 0)
   {
      if ((xronos.tv_sec & 0x80000000) ^ (step_second.low & 0x80000000))
      {
         /**************************************************
            ms bit of tv_sec has changed
            change is 1 to 0 approximately every 140 years
            and must carry into the high number half
         ***************************************************/

         if (step_second.low & 0x80000000) step_second.high++;
         step_second.low = (unsigned) xronos.tv_sec;
         store_second(&step_second);
      }
   }

   clockr[1] = (unsigned) xronos.tv_usec / 1000
             + (xronos.tv_sec & 0xFFFF) * 1000;

   clockr[0] = ((unsigned) xronos.tv_sec >> 16) * 1000
             + ((step_second.high * 1000) << 16)
             + (clockr[1] >> 16);

   clockr[1] = (clockr[1] & 0xFFFF)
             | ((clockr[0] & 255) << 16);

   clockr[0] = clockr[0] >> 8;

   #ifdef RATIO
     
   _register[DAYCLOCK]   = clockr[1];
   _register[DAYCLOCK_U] = clockr[0];
     
   #endif
   #endif	/*		__X64 / 32-bit		*/	

   /****************************************************************
	update time zone + summertime offset per second
   *****************************************************************/

   if (xronos.tv_sec ^ interval_seconds_mask) tzone(&xronos);
   interval_seconds_mask = xronos.tv_sec;

   /****************************************************************
      let instructions emulation thread copy the dayclock update
   ****************************************************************/

   #if (RTA_SMP > 1)
   indication |= INCREMENTER_CARRY;
   #endif

   if (flag['h'-'a']) printf("[%p -> %x]\n", xcore, indication);
}
