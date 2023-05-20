/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is part of the emulated-instruction-execution-subsection
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



#if 1

void gshiftr(int positions, int words, int fill, int target[]);
void gshiftl(int positions, int words, int fill, int target[]);
void lsc(int ea, smp *xcore);
void dlsc(int ea, smp *xcore);
int __mm(int ea, int designator, int target[], smp *xcore);
int __m(int ea, int designator, int target[], smp *xcore);
int __ms(int ea, int designator, int *target, smp *xcore);
int __mf(int ea, int designator, int target[], smp *xcore);
int __d(int ea, int designator, int target[], smp *xcore);
int __dd(int ea, int designator, int target[], smp *xcore);

#else

extern void sar(int ea);	/*	shift A right	*/
extern void sbr(int ea);	/*	shift B right	*/
extern void dsr(int ea);	/*	double shift right	*/

extern void sal(int ea);	/*	shift A left		*/
extern void sbl(int ea);	/*	shift B left		*/
extern void dsl(int ea);	/*	double shift left	*/

extern void rar(int ea);	/*	rotate A right		*/
extern void rbr(int ea);	/*	rotate B right		*/
extern void drr(int ea);	/*	double rotate right	*/

extern void ral(int ea);	/*	rotate A left		*/
extern void rbl(int ea);	/*	rotate B left		*/
extern void drl(int ea);	/*	double rotate left	*/

extern void saa(int ea);	/*	shift A algebraic	*/
extern void sba(int ea);	/*	shift B algebraic	*/
extern void dsa(int ea);	/*	double shift algebraic	*/

extern void lsc(int ea);	/*	load shift and count	*/
extern void  da(int ea);	/*	double add		*/
extern void dan(int ea);	/*	double add negative	*/
extern void dlsc(int ea);	/* double load shift and count	*/

extern void  or(int operand);	/*	OR with register A	*/
extern void orb(int operand);	/*	OR with register B	*/
extern void and(int operand);	/*	AND with register A	*/
extern void andb(int operand);	/*	AND with register B	*/
extern void xor(int operand);	/*	XOR with register A	*/
extern void xorb(int operand);	/*	XOR with register B	*/
extern void  aa(int operand);	/*	add to A		*/
extern void  ab(int operand);	/*	add to B		*/
extern void ana(int operand);	/*	add negative to A	*/
extern void anb(int operand);	/*	add negative to B	*/
extern void   m(int operand);	/*	multiply		*/
extern void  mf(int operand);	/*	multiply fractional	*/
extern void   d(int operand);	/*	divide			*/

#endif
