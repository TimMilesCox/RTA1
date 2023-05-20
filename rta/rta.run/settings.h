/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is part of the processor-architecture-subsystem
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

#define YIELD		65	/*	interrupt vectors	*/
#define	SPAWN		66
#define	BANK		67
#define	ARRAY		68
#define	BANK_FREE	69
#define	EXIT		70

#define	REALTIME_CLOCK	148	/*	interrupt registers	*/
#define	DAYCLOCK_U	150
#define	DAYCLOCK	151

#undef	TIMER		/*	127	*/
#undef	SIGALERT
#define	NET_ATTENTION_COUNT 121
#define	TIMER_IMASK	4
#define	INTERVAL	3000
#define	REGISTERS	288

#define	PLATFORM_INTERVAL 12000		/* default usleep emulator interactive thread		*/
#define	DAYCLOCK_REVISION 750		/* default usleep emulator dayclock revision 		*/

#define	ACTIVITY_SPACE	1200000		/* microseconds idle before osserv / linuxnet usleeps	*/
#define	REST_GRANULE	50000		/* osserv / linuxnet usleep step repeated on inactivity	*/
#define	NET_GRANULE	500
