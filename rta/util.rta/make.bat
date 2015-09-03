rem #
rem #    Copyright Tim Cox, 2012
rem #    TimMilesCox@gmx.ch
rem #
rem #    This file is part of the software emulation of the freeware
rem #    processor architecture
rem #
rem #                RTA1
rem #
rem #    RTA1 is a free processor architecture design.
rem #
rem #    The executable emulation of RTA1 is free software.
rem #
rem #    Instruction code for the target RTA1 architecture is free software
rem #    if it is delivered with this software
rem #
rem #    Software programs delivered with this software to connect the
rem #    emulated RTA1 with real network interfaces in the emulator host
rem #    are free software
rem #
rem #    Scripts and programs delivered with this software for running
rem #    on other computers and interacting with the RTA1 are free software
rem #
rem #    Scripts and utility programs for constructing RTA1 target
rem #    executable software are free software
rem #
rem #    You can redistribute and/or modify RTA1 and its materials
rem #    under the terms of the GNU General Public License as published by
rem #    the Free Software Foundation, either version 3 of the License, or
rem #    (at your option) any later version.
rem #
rem #    RTA1 is distributed in the hope that it will be useful,
rem #    but WITHOUT ANY WARRANTY; without even the implied warranty of
rem #    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem #    GNU General Public License for more details.
rem #
rem #    You should have received a copy of the GNU General Public License
rem #    along with RTA1.  If not, see <http://www.gnu.org/licenses/>.
rem #
rem #
rem 
rem if	[ -z "$RTA_BINARY" ]
rem then
rem echo	export RTA_BINARY=directory-for-new-binaries \
rem 	in your shell profile
rem echo	and add \$RTA_BINARY to \$PATH
rem exit	0
rem fi
rem 
rem if      [ ! -d "$RTA_BINARY" ]
rem then
rem mkdir   "$RTA_BINARY"
rem fi

pushd ..\netifx
cl /J /c /Fo..\emulator.rel\argue.obj argue.c
popd

cl /J /DDOS /Fe..\binary.rta\win32\fds24 ..\emulator.rel\argue.obj fds24.c
cl /J /DDOS /Fe..\binary.rta\win32\fsformat ..\emulator.rel\argue.obj fsformat.c
cl /J /DDOS /Fe..\binary.rta\win32\lstring lstring.c
cl /J /DDOS /Fe..\binary.rta\win32\slab ..\emulator.rel\argue.obj slab.c
cl /J /DDOS /Fe..\binary.rta\win32\4kformat ..\emulator.rel\argue.obj 4kformat.c

echo	..\binary.rta\win32
dir	..\binary.rta\win32

