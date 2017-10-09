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

if not exist %RTA_BINARY% (

echo
echo    [*** PLEASE DO THESE SIMPLE SETUP STEPS ***]
echo
echo    export RTA_BINARY=directory in your shell profile
echo    and add RTA_BINARY to PATH
echo    supplied binaries are
echo            ../binary.rta/osx.ppc
echo            ../binary.rta/osx.x86
echo
echo    otherwise build for your platform
echo    echo export RTA_BINARY=new-directory-name \>\> \~/.bash_profile
echo    echo export PATH=\$PATH:\$RTA_BINARY      \>\> \~/.bash_profile
echo
echo    [NEW SHELL]
echo    cd ../util.rta
echo    ./make
echo    cd ../client
echo    ./make_fp [powerpc\|sparc\|intel\|arm]       [linux\|osx]
echo    cd ../netifx
echo    ./make  [powerpc\|sparc\|intel\|arm]       [linux\|osx]
echo    ./runL
echo
echo    [NEW SHELL]
echo    cd ../rta.run
echo    ./tipgen
echo    ./run srom
echo
echo    if you build say RTA_BINARY=a-new-directory, not a supplied one
echo    you only build native for this platform
echo    the make parameters are not for cross compiling \
        but for small code differences
echo    if you kill the supplied binaries you can always download again
echo
echo    [*** SIMPLE SETUP NEEDED. PLEASE SCROLL BACK TO SEE WHAT TO DO ***]
echo	)


if	not exist	%RTA_MBANKS% set RTA_MBANKS=2

echo	emulator $RTA_BINARY/tiptoe%RTA_MBANKS%

%RTA_BINARY%/tiptoe%RTA_MBANKS% -ls	..\target.rta\rta.gen\%1.rom	^
					..\fsimages\%2

