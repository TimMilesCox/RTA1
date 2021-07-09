/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is part of the memory-and-device-subsection
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



extern int operand_read(unsigned ea, int designator);
extern int burst_read2(int *list, unsigned ea);
extern int burst_read4(int *list, unsigned ea);

extern int operand_write(int v, unsigned ea, int designator);
extern int burst_write2(int *list, unsigned ea);
extern int burst_write4(int *list, unsigned ea);

extern word memory_read(unsigned ea);

#ifdef ABSOTS
extern word *memory_hold(unsigned ea);
#endif

int device_read(int device_index, int relocation_base, unsigned offset, int designator);
