/*********************************************************************

    Copyright Tim Cox, 2012
    TimMilesCox@gmx.ch

    This file is part of the real-external-network-interaction
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

    You can redistribute and/or modify RTA1 and it materials
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



#define DEVICE_PAGE     2048
#define DEVICE_PAGES    256
#define	DEVICE_COMPARTMENTS 2

#define	CONTAINER_PAGES	256*8

#define	FORWARD(X)	PORT(X) & 65535

#define	FRAME				FORWARD(0x8000)
#define	OUT_OF_BAND_TARGET_PROTOCOL	PORT(0x4000)

#define	IP	PORT(0x0800)
#define	ARP	PORT(0x0806)
			

#define	FPL sizeof(mm_descriptor)


typedef	struct { unsigned short			    flag,
					    frame_length,
						   ll_hl,
						     i_f,
						protocol; } mm_descriptor;

typedef struct { mm_descriptor			preamble;
		 unsigned char	frame[DEVICE_PAGE - FPL]; } mm_netbuffer;

typedef struct { mm_netbuffer	       i[DEVICE_PAGES/2],
				       o[DEVICE_PAGES/2]; } mm_netdevice;

typedef struct { mm_netbuffer	    i[CONTAINER_PAGES/2],
				    o[CONTAINER_PAGES/2]; } mm_netcontainer;
