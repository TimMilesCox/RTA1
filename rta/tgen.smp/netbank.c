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
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#endif

#include <errno.h>
#include "../engine.smp/emulate.h"
#include "../engine.smp/smp.h"
#include "../rta.run/settings.h"
#include "../include.rta/address.h"
#include "../include.rta/argue.h"
#include "../netifx/sifr_mm.h"
#include "../netifx/ifconfig.h"

#ifdef	LINUX
#ifndef	GCC
#include "../tgen.smp/_mnames.h"
#endif
#endif

#define	DEVICE_PAGE	2048
#define	DEVICE_PAGES	256

extern smp		 core[];

extern device		 devices[];

void netbank()
{
   static int		 trunk_key = 'aaaa';

   mm_netbuffer		*dpointer;
   int			 scope; 

   #ifdef SIGALERT
   int		*wrpid;
   #endif

   #ifdef X86_MSW
   TCHAR	 device_logo[] = "Global\\device01";
   HANDLE	 mhandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, device_logo);

   if (mhandle == NULL)
   {
      printf("device array 2 unavailable\n"
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

   if (uflag['A'-'A']) trunk_key = 'aaaz';

   int		 mhandle = shmget(trunk_key, DEVICE_PAGE * DEVICE_PAGES, 0);

   printf("netbank %d\n", mhandle);

   if (mhandle < 0)
   {
      printf("default network space not available\n");
      printf("runL first\n");
      printf("code %d\n", errno);
      exit(0);
   }

   dpointer = (mm_netbuffer *) (void *) shmat(mhandle, NULL, 0);
   devices[2].dev16 = (void *) dpointer;
   printf("netbase %p\n", dpointer);

   if ((long) dpointer == -1)
   {
      /***********************************************************
         condition must be ( == -1) not ( < 0)
         because 32-bit machines with 2GByte+ memory
         can test negative with a genuine address
      ***********************************************************/

      printf("network space not mapped E %d\n", errno);
      exit(0);
   }

   if (uflag['R'-'A'])
   {
      scope = DEVICE_PAGES * ((core[0].BASE[128 + 2] & 0x003FFFFF) + 1);

      printf("device 2 network interface relay added @ %p:%x\n", dpointer, scope); 
      dpointer += scope;
      scope >>= 1;

      while (scope--)
      {
         dpointer--;
         dpointer->preamble.protocol = CONFIGURATION_MICROPROTOCOL;
         dpointer->preamble.flag = FRAME;
      }

      return;
   }

   if ((dpointer->preamble.flag & FRAME)
   &&  (dpointer->preamble.protocol == CONFIGURATION_MICROPROTOCOL))
   {
   }
   else
   {
      printf("default network space unsynchronised. Restart runL\n");
      exit(0);
   }

   #endif X86_MSW
}

static void broadcast_trunk(int device_id, int block)
{  
   int                   cores = RTA_SMP;
   smp                  *ascor = core;
   
   int                   device_attribute = DATA16_FLAG | block;
   int			 port_offset = 128 + device_id;
   
   while (cores--)
   {  
      ascor->BASE[port_offset] = device_attribute ;
      ascor++;
   }
}

/****************************************************
	assign_interface_relay()
	assigns an extra interface relay device array
	beyond the default one
****************************************************/

void assign_interface_relay(int device_id, char *text)
{  
   unsigned int          rkey = -1;
   int                   _x,
                         _y,
                         bytes,
                         blocks,
                         block;

   int			 request_size = 0,
			 request_flag = 0;
   
   bank16               *_p;
   device               *_q = devices + device_id;
   struct shmid_ds       info;

   mm_netbuffer		*dpointer;
   
   sscanf(text + 1, "%x", &rkey);

   if (rkey < 0x61000000)
   {
      /**************************************************
	this is for testing bus operations
	without doing writies in an active network buffer
      **************************************************/

      request_size = DEVICE_PAGE * DEVICE_PAGES * (rkey & 65535);
      request_flag = IPC_CREAT;
   }

   _x = shmget(rkey, request_size, request_flag);
   
   if (_x < 0)
   {  
      printf("net device %d %x space not available E %d\n", device_id, rkey, errno);
      return;
   }
   
   _y = shmctl(_x, IPC_STAT, &info);
   
   if (_y < 0)
   {  
      printf("net device %d space not staticised E %d\n", device_id, errno);
      return;
   }
   
   _p = shmat(_x, NULL, 0);

   if ((long) _p == -1)
   {
      /***********************************************************
         condition must be ( == -1) not ( < 0)
         because 32-bit machines with 2GByte+ memory
         can test negative with a genuine address
      ***********************************************************/

      printf("net device %d space not mapped E %d\n", device_id, errno);
      return;
   }

   dpointer = (mm_netbuffer *) _p;
   devices[device_id].dev16 = _p;
   bytes = info.shm_segsz;

   /**************************************************************
	the peripheral descriptor port contains a block high index
	not a size
	so zero represents 1 block
        and maximum size 65536 can be represented
   **************************************************************/

   blocks = bytes >> 19;
   block  = blocks - 1;

   if ((block < 0) || (block > 65535))
   {
      printf("net interface block out of range\n");
      return;
   }
   
   if (rkey < 0x61000000)
   {
      broadcast_trunk(device_id, block);

      dpointer += blocks * (DEVICE_PAGES / 2);
      dpointer->preamble.protocol = CONFIGURATION_MICROPROTOCOL;
      dpointer->preamble.flag = FRAME;
      printf("device %d network interface relay key %X added @ %p\n", device_id, rkey, _p);
   }
   else if ((uflag['A'-'A'])
   ||      ((dpointer->preamble.flag & FRAME) &&       
            (dpointer->preamble.protocol == CONFIGURATION_MICROPROTOCOL)))
   {
      broadcast_trunk(device_id, block);

      printf("device %d network interface relay key %X added @ %p\n", device_id, rkey, _p);
   }
   else
   {
      printf("net device %d space unsynchronised. Restart osserv / linuxnet process\n", device_id);
   }
}

