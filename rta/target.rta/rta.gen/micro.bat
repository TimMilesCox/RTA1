@echo	off

rem 
rem     Copyright Tim Cox, 2012
rem     TimMilesCox@gmx.ch
rem 
rem     This file is part of the software emulation of the freeware
rem     processor architecture
rem 
rem                 RTA1
rem 
rem     RTA1 is a free processor architecture design.
rem 
rem     The executable emulation of RTA1 is free software.
rem 
rem     Instruction code for the target RTA1 architecture is free software
rem     if it is delivered with this software
rem 
rem     Software programs delivered with this software to connect the
rem     emulated RTA1 with real network interfaces in the emulator host
rem     are free software
rem 
rem     Scripts and programs delivered with this software for running
rem     on other computers and interacting with the RTA1 are free software
rem 
rem     Scripts and utility programs for constructing RTA1 target
rem     executable software are free software
rem 
rem     You can redistribute and/or modify RTA1 and its materials
rem     under the terms of the GNU General Public License as published by
rem     the Free Software Foundation, either version 3 of the License, or
rem     (at your option) any later version.
rem 
rem     RTA1 is distributed in the hope that it will be useful,
rem     but WITHOUT ANY WARRANTY; without even the implied warranty of
rem     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem     GNU General Public License for more details.
rem 
rem     You should have received a copy of the GNU General Public License
rem     along with RTA1.  If not, see <http://www.gnu.org/licenses/>.
rem 
rem 


del system.txo
del ..\drivers\drivers.txo
del ..\fs\fs.txo
del corefs.txo
del minimal.txo
del minimal
del minimal.rom

rem 	%1 is for the absolute listing option -d 
rem 	which is also the .txo absolute summary option
rem 	passed forward in links

masmx image -lnxkzy system %1 > mlist.txt
pushd ..\lib
call .\make %1
cd ..\fs
masmx fs fs -lnxkzyF %1 >list.txt
cd ..\drivers
masmx drivers drivers -lnxkzy %1 >list.txt
rem 	cd ..\async
rem 	masmx async2 async2 -lnxkzyd >list.txt
rem 	.\make
popd

masmx minimal -lnbxk %1 >> mlist.txt
call .\mcorefs >> mlist.txt
masmx adcorefs minimal -ulnkx >> mlist.txt
%RTA_BINARY%\lstring -b minimal.txo minimal
%RTA_BINARY%\slab -v minimal minimal.rom 40 -k


seeif -as system.zak system.txo
seeif -as minimal.zak minimal.txo
seeif -s minimals.zak minimal
seeif -s minimalr.zak minimal.rom

if [%1]==[] goto talk
if [%1]==[-d] goto end
:talk
echo	option ./micro -d for optimal view of memory map in mlist.txt
echo	-d option also writes expected summaries in output binary minimal.txo
:end

@echo	on
