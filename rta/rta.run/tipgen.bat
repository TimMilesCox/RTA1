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
rem     You can redistribute it and/or modify RTA1
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
rem 
rem 
rem 


if not exist %RTA_BINARY% echo	export %RTA_BINARY%=directory-for-new-binaries in your environment variables and add %RTA_BINARY% to PATH

rem if      [ ! -d "$RTA_BINARY" ]
rem mkdir   "$RTA_BINARY"


pushd   ..\netifx
cl /J /c /DX86_MSW /Fo..\win32.rel\netbank.obj netbank.c
popd

cl /J /c /Fo..\win32.rel\idisplay.obj idisplay.c

if not exist %RTA_MBANKS% set RTA_MBANKS=2

echo	emulated RTA executable space $RTA_MBANKS banks image tiptoe%RTA_MBANKS%

set	ARCHFLAG=/DX86_MSW

pushd	..\engine.rta
call genrel %ARCHFLAG%
popd

cl /J %ARCHFLAG% /DMBANKS=%RTA_MBANKS% /Fe%RTA_BINARY%\tiptoe%RTA_MBANKS%	^
					  ..\win32.rel\rta.obj			^
					  ..\win32.rel\ii.obj			^
					  ..\win32.rel\memory.obj		^
					  ..\win32.rel\netbank.obj		^
					  ..\win32.rel\rw.obj			^
					  ..\win32.rel\alu.obj			^
					  ..\win32.rel\fpu.obj			^
					  ..\win32.rel\sr.obj			^
					  ..\win32.rel\idisplay.obj tiptoe.c

echo	binaries in %RTA_BINARY%
dir %RTA_BINARY%

