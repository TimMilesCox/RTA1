/*	dummy test and set with no lock			*/
/*	not for cores+ implemented as platform threads	*/
/*	for single cores running FIFO scheduling	*/
/*	that makes all emulated instructions atomic	*/
/*	otherwise implement a real memory lock		*/
/*	per platform architecture on the model		*/
/*	rta/assembly.x64/memlock/ts.msm			*/

int _ts(unsigned short *target)
{
   #ifdef INTEL		// meaning only little endian
   int	result = *target;
   *target = 0x8000;
   #else
   int  result = ((*target) & 255) << 8;
   *target = 0x0080;
   #endif

   return result; 
}

