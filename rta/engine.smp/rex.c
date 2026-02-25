
#include "emulate.h"
#include "smp.h"
#include "rw.h"
#include "fpu.h"
#include "ii.h"
#include "operand.h"
#include "alu.h"

/***************************************************************
	these flag arrays are the number of
	internal stack registers needed between
	destination cursor q and stack root
	in order to carry out each operation

	lane b is destination cursor + 1
	so 2 registers must be available

	1-word lane a, 2-word and 4-word operations are per size

	operations which don't compute into
	internal stack registers need nothing on the stack

	non-repeat operations are marked < 0 and no-op
***************************************************************/

#if 0
extern word *apc;
#endif

#define	R_SA	1
#define	R_SB	2
#define	R_Z	0

#define	R_LA	1
#define	R_LB	2
#define	R_TZ	0
#define	R_TP	0

#define	R_OR	1
#define	R_ORB	2

#define	R_AND	1
#define	R_ANDB	2

#define	R_XOR	1
#define	R_XORB	2

#define	R_AA	1
#define	R_AB	2

#define	R_ANA	1
#define	R_ANB	2

#define	R_M	2
#define	R_MF	1
#define	R_D	2

#define	R_NOP	-1


static int registers1[32] = {	R_NOP, R_NOP, R_NOP, R_NOP, R_SA, R_SA, R_Z, R_NOP,
				R_NOP, R_NOP, R_NOP, R_NOP, R_LA, R_LB, R_TZ, R_TP,
				R_NOP, R_NOP, R_OR, R_ORB, R_AND, R_ANDB, R_XOR, R_XORB,
				R_AA, R_AB, R_ANA, R_ANB, R_M, R_MF, R_D, R_NOP		} ;

#define	R_SAR	1
#define	R_SBR	2
#define	R_DSR	2

#define	R_SAL	1
#define	R_SBL	2
#define	R_DSL	2

#define	R_RAR	1
#define	R_RBR	2
#define	R_DRR	2

#define	R_RAL	1
#define	R_RBL	2
#define	R_DRL	2

#define	R_SAA	1
#define	R_SBA	2
#define	R_DSA	2

static int registers6[32] = {	R_SAR, R_SBR, R_DSR, R_NOP, R_SAL, R_SBL, R_DSL, R_NOP,
				R_RAR, R_RBR, R_DRR, R_NOP, R_RAL, R_RBL, R_DRL, R_NOP,
				R_SAA, R_SBA, R_DSA, R_NOP, R_NOP, R_NOP, R_NOP, R_NOP,
				R_NOP, R_NOP, R_NOP, R_NOP, R_NOP, R_NOP, R_NOP, R_NOP	} ;
#define	R_QS	4
#define	R_QL	4
#define	R_DTE	0

#define	R_FA	4
#define	R_FAN	4
#define	R_FM	4
#define	R_FD	4

#define	R_MTA	0
	
#define	R_DS	2
#define	R_DL	2
#define	R_DA	2
#define	R_DAN	2

static int registers7[32] = {	R_NOP, R_NOP, R_NOP, R_NOP, R_NOP, R_NOP, R_NOP, R_NOP,
				R_QS, R_QL, R_DTE, R_NOP, R_FA, R_FAN, R_FM, R_FD,
				R_NOP, R_NOP, R_NOP, R_NOP, R_NOP, R_MTA, R_NOP, R_NOP, 
				R_DS, R_DL, R_DA, R_DAN, R_NOP, R_NOP, R_NOP, R_NOP	} ;

void rex(int rex_ea, smp *xcore)
{
   word		 instruction;

   int		 icode;
   int		 designator;
   int		 scode;
   int		 signs;
   int		 registers;
   int		 xtag = 0;
   int		*xtagp;
   int		*targetp;
   int		 v;
   int		 temp[4];

   int		 ea;
   int		*register_set = xcore->register_set;

   if (psr & 0x00800000) return;
   instruction = memory_read(rex_ea, xcore);
   icode = instruction.t1;
   designator = icode & 7;
   scode = icode >> 3;

   if  	   (designator < 6) registers = registers1[scode];
   else if (designator > 6) registers = registers7[scode];
   else                     registers = registers6[scode];

   if (registers < 0) return;

   if (rdatac & 0x00800000)
   {
      if (registers) q = sp;
      rdatac = wdatac;      
   }

   EFFECTIVE_ADDRESS;
   xtagp = _register + xtag;

   for (;;)
   {
       
      if (registers)
      {      
         targetp = _register + (q);

         if (((q) + registers > 128)
         ||  ((q) < 24))
         {
            GUARD_AUTHORITY
            return; 
         }
      }

      switch (designator)
      {
         case 6:
            switch (icode)
            {
               case SAR:
                  *targetp = (*targetp >> ea) & 0x00FFFFFF;
                  break;

               case SBR:
                  *(targetp + 1) = (*(targetp + 1) >> ea) & 0x00FFFFFF;
                  break;

               case DSR:
                  gshiftr(ea, 2, 0, targetp);
                  break;

               case SAL:
                  *targetp = (*targetp << ea) & 0x00FFFFFF;
                  break;

               case SBL:
                  *(targetp + 1) = (*(targetp + 1) << ea) & 0x00FFFFFF;
                  break;

               case DSL:
                  gshiftl(ea, 2, 0, targetp);
                  break;

               case RAR:
                   gshiftr(ea, 1, *targetp, targetp);
                  break;

               case RBR:
                  gshiftr(ea, 1, *(targetp + 1), targetp + 1);
                  break;

               case DRR:
                  temp[0] = temp[2] = *targetp;
                  temp[1] = temp[3] = *(targetp + 1);
                  gshiftr(ea % 48, 4, 0, temp);
                  *targetp = temp[2];
                  *(targetp + 1) = temp[3];
                  break;

               case RAL:
                  gshiftl(ea, 1, *targetp, targetp);
                  break;

               case RBL:
		  gshiftl(ea, 1, *(targetp + 1), targetp + 1);
                  break;

               case DRL:
                  temp[0] = temp[2] = *targetp;
                  temp[1] = temp[3] = *(targetp + 1);
                  gshiftl(ea % 48, 4, 0, temp);
                  *targetp = temp[0];
                  *(targetp + 1) = temp[1];
                  break;

               case SAA:
                  signs = 0x00FFFFFF & (((int) *targetp  << 8) >> 31);
                  gshiftr(ea, 1, signs, targetp);
                  break;

               case SBA:
                  signs = 0x00FFFFFF & (((int) *(targetp + 1) << 8) >> 31);
                  gshiftr(ea, 1, signs, targetp + 1);
                  break;

               case DSA:
                  signs = 0x00FFFFFF & (((int) *targetp  << 8) >> 31);
                  gshiftr(ea, 2, signs, targetp);
                  break;
            }

            break;

         case 7:
            switch (icode)
            {
               case QS:
                  BURST_WRITE4(targetp, ea)
                  break;

               case QL:
                  BURST_READ4(targetp, ea)
                  break;

               case DTE:
                  BURST_READ2(temp, ea)

                  if ((a ^ temp[0]) | (b ^ temp[1]))
                  {
                  }
                  else
                  {
                     apc++;
                     return;
                  }

                  break;

                  #ifndef FLOATING_PACKED
               case FA:
                  if ((__fa(ea, targetp, xcore)) < 0) return;
                  break;

               case FAN:
                  if ((__fan(ea, targetp, xcore)) < 0) return;
                  break;

               case FM:
                  if ((__fm(ea, targetp, xcore)) < 0) return;
                  break;

               case FD:
                  if ((__fd(ea, targetp, xcore)) < 0) return;
                  break;

                  #else

               case FA:
                  if (psr & FLOATING_PACKED)
                  {
                     if ((__fa48(ea, targetp, xcore)) < 0) return;
                  }
                  else
                  {
                     if ((__fa(ea, targetp, xcore)) < 0) return;
                  }
                  break;

               case FAN:
                  if (psr & FLOATING_PACKED)
                  {
                     if ((__fan48(ea, targetp, xcore)) < 0) return;
                  }
                  else
                  {
                     if ((__fan(ea, targetp, xcore)) < 0) return;
                  }
                  break;

               case FM:
                  if (psr & FLOATING_PACKED)
                  {
                     if ((__fm48(ea, targetp, xcore)) < 0) return;
                  }
                  else
                  {
                     if ((__fm(ea, targetp, xcore)) < 0) return;
                  }
                  break;

               case FD:
                  if (psr & FLOATING_PACKED)
                  {
                     if ((__fd48(ea, targetp, xcore)) < 0) return;
                  }
                  else
                  {
                     if ((__fd(ea, targetp, xcore)) < 0) return;
                  }
                  break;

                  #endif	/* FLOATING_PACKED */

               case MTA:
                  OPERAND(v, ea, 7)

                  if ((a & k) ^ (v & k))
                  {
                  }
                  else
                  {
                     apc++;
                     return;
                  }

                  break;

               case DS:
		  BURST_WRITE2(targetp, ea)
                  break;

               case DL:
                  BURST_READ2(targetp, ea)
                  break;


               case DA:
                  BURST_READ2(temp, ea)

                  temp[1] += *(targetp + 1);
                  *(targetp + 1) = temp[1] & 0x00FFFFFF;
                  temp[0] = temp[0] + *targetp + ((temp[1] >> 24) & 1);
                  *targetp = temp[0] & 0x00FFFFFF;
                  psr = (psr & 0x00FFFFFE) | ((temp[0] >> 24) & 1);
                  break;


               case DAN:
                  BURST_READ2(temp, ea)
                  temp[1] ^= 0x00FFFFFF;
                  temp[0] ^= 0x00FFFFFF;

                  temp[1] = temp[1] + *(targetp + 1) + 1;
                  *(targetp + 1) = temp[1] & 0x00FFFFFF;
                  temp[0] = temp[0] + *targetp + ((temp[1] >> 24) & 1);
                  *targetp = temp[0] & 0x00FFFFFF;
                  psr = (psr & 0x00FFFFFE) | ((temp[0] >> 24) & 1);
                  break;
            }

            break;

         default:
            switch (icode & 248)
            {
               case SA:
                  OPERAND_WRITE(*targetp, ea, designator)
                  break;

               case SB:
                  OPERAND_WRITE(*(targetp + 1), ea, designator)
                  break;

               case Z:
                  OPERAND_WRITE(0, ea, designator)
                  break;

               case LA:
                  OPERAND(v, ea, designator)
                  *targetp = v;
                  break;

               case LB:
                  OPERAND(v, ea, designator)
                  *(targetp + 1) = v;
                  break;

               case TZ:
                  OPERAND(v, ea, designator)
                  if (v & 0x00FFFFFF) break;
                  apc++;
                  return;


               case TP:
                  OPERAND(v, ea, designator)
                  if (v & 0x00800000) break;
                  apc++;
                  return;


               case OR:
                  OPERAND(v, ea, designator)
                  *targetp |= v;
                  break;

               case ORB:
                  OPERAND(v, ea, designator)
                  *(targetp + 1) |= v;
                  break;

               case AND:
                  OPERAND(v, ea, designator)
                  *targetp &= v;
                  break;

               case ANDB:
                  OPERAND(v, ea, designator)
                  *(targetp + 1) &= v;
                  break;

               case XOR:
                  OPERAND(v, ea, designator)
                  *targetp ^= v;
                  break;

               case XORB:
                  OPERAND(v, ea, designator)
                  *(targetp + 1) ^= v;
                  break;

               case AA:
                  OPERAND(v, ea, designator)
                  v += *targetp;
                  *targetp = v & 0x00FFFFFF;
		  psr = (psr & 254) | ((v >> 24) & 1);
                  break;

               case AB:
                  OPERAND(v, ea, designator)
                  v += *(targetp + 1);
                  *(targetp + 1) = v & 0x00FFFFFF;
                  psr = (psr & 254) | ((v >> 24) & 1);
                  break;

               case ANA:
                  OPERAND(v, ea, designator)
                  v ^= 0x00FFFFFF;
                  v += *targetp + 1;
                  *targetp = v & 0x00FFFFFF;
                  psr = (psr & 254) | ((v >> 24) & 1);
                  break;

               case ANB:
                  OPERAND(v, ea, designator)
                  v ^= 0x00FFFFFF;
                  v += *(targetp + 1) + 1;
                  *(targetp + 1) = v & 0x00FFFFFF;
                  psr = (psr & 254) | ((v >> 24) & 1);
                  break;

               case M:
                  if ((__mm(ea, designator, targetp, xcore)) < 0) return;
                  break;

               case MF:
                  if ((__ms(ea, designator, targetp, xcore)) < 0) return;
                  break;

               case D:
                  if ((__dd(ea, designator, targetp, xcore)) < 0) return;
                  break;
            }

            break;

      }

      if ((registers) && (rdata)) q = (q + rdata) & 0x00FFFFFF;

      if ((xtag) && (wdata))
      {
         *xtagp = (*xtagp + wdata) & 0x00FFFFFF;
         ea = (ea + wdata) & 0x00FFFFFF;
      }

      rdatac = (rdatac - 1) & 0x00FFFFFF;
      if (rdatac & 0x00800000) break;

      if (indication & (EXTERNAL_INDICATIONS))
      {
         apc--;
         break;
      }
   }
}
