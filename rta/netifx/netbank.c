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

    The masmx target-independent meta-asembler delivered here
    is free software whether it is used for constructing RTA1 code
    or any other code. masmx is a separate free program by the
    same author and included with this software to save searching for it

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


#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include "../engine.rta/emulate.h"
#include "../rta.run/settings.h"
#include "sifr_mm.h"

#define	DEVICE_PAGE	2048
#define	DEVICE_PAGES	256

extern device		 devices[];

void netbank()
{
   mm_netbuffer	*dpointer;

   #ifdef SIGALERT
   int		*wrpid;
   #endif

   int		 mhandle = shmget('aaaa', DEVICE_PAGE * DEVICE_PAGES, 0);

   printf("netbank %d\n", mhandle);
   if (mhandle < 0)
   {
      printf("code %d\n", errno);
      return;
   }

   dpointer = (mm_netbuffer *) (void *) shmat(mhandle, NULL, 0);
   devices[2].s.dev16 = (void *) dpointer;
   printf("netbase %p\n", dpointer);

   if ((long) dpointer < 0) printf("E %d\n", errno);

   #ifdef SIGALERT
   else
   {
      /***************************************************

	one-time microprotocol write to the interface
	process at startup

	hand it the pid of the RTA1 emulator for signals

	the interface task receives it in a microPDU in
	the first buffer of the output ring

      ***************************************************/

      dpointer += DEVICE_PAGES / 2;
      wrpid = (int *) dpointer + 1;
      *wrpid = getpid();
      dpointer->preamble.flag = OUT_OF_BAND_TARGET_PROTOCOL;
      printf("[ID %d @ %p]\n", *wrpid, wrpid);
   }
   #endif
}

