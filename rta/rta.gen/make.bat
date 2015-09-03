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


del system.txo

del ..\fs\fs.txo
del rtaimage.txo
del srom
del srom.rom

masmx image -lnxkzyd system >list.txt
pushd ..\lib
call make
cd ..\fs
masmx fs fs -lnxkzyFd >list.txt
cd ..\drivers
masmx drivers drivers -lnxkzyd >list.txt
cd ..\async
rem #	masmx async2 async2 -lnxkzyd >list.txt
call make
popd
masmx bank rtaimage -lnubxkd >>list.txt
..\binary.rta\win32\lstring -b rtaimage.txo srom
..\binary.rta\win32\slab -v srom srom.rom 48 -k
