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


#include <stdio.h>

#ifdef	X86_MSW
#include <windows.h>
#define	INTEL
//	typedef char TCHAR;
#else
#include <unistd.h>
#include <sys/shm.h>
#endif

#include <errno.h>
#include "../engine.rta/emulate.h"
#include "../rta.run/settings.h"
#include "../include.rta/address.h"
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

   #ifdef X86_MSW
   TCHAR	 device_logo[] = "Global\\device01";
   HANDLE	 mhandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, device_logo);

   if (mhandle == NULL)
   {
      printf("device array 1 unavailable\n"
             "enter '.' to stop the emulator\n");
      printf("run portal first\n");
      printf("code %d\n", GetLastError());
      return;
   }

   dpointer = (mm_netbuffer *) MapViewOfFile(mhandle,
                                             FILE_MAP_ALL_ACCESS, 0, 0,
                                             DEVICE_PAGE * DEVICE_PAGES);

   devices[2].s.dev16 = (void *) dpointer;
   if (dpointer == NULL) printf("device array not in scope %d\n", GetLastError());

   #else  X86_MSW
   int		 mhandle = shmget('aaaa', DEVICE_PAGE * DEVICE_PAGES, 0);

   printf("netbank %d\n", mhandle);

   if (mhandle < 0)
   {
      printf("no network bank is available\n"
             "enter '.' to stop the emulator\n");
      printf("run portal first\n");
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
   #endif SIGALERT
   #endif X86_MSW
}

