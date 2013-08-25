#
#    Copyright Tim Cox, 2012
#    TimMilesCox@gmx.ch
#
#    This file is part of the software emulation of the freeware
#    processor architecture
#
#                RTA1
#
#    RTA1 is a free processor architecture design.
#
#    The executable emulation of RTA1 is free software.
#
#    Instruction code for the target RTA1 architecture is free software
#    if it is delivered with this software
#
#    Software programs delivered with this software to connect the
#    emulated RTA1 with real network interfaces in the emulator host
#    are free software
#
#    Scripts and programs delivered with this software for running
#    on other computers and interacting with the RTA1 are free software
#
#    The masmx target-independent meta-asembler delivered here
#    is free software whether it is used for constructing RTA1 code
#    or any other code. masmx is a separate free program by the
#    same author and included with this software to save searching for it
#
#    Scripts and utility programs for constructing RTA1 target
#    executable software are free software
#
#    You can redistribute it and/or modify RTA1
#    under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    RTA1 is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with RTA1.  If not, see <http://www.gnu.org/licenses/>.
#
#
#


script ./prun sets up an address which you can come from
when you are on the same host as the emulator. By default
this come-from address is

	172.29.7.6

Applications talking to the emulated RTA1 must prevent
IP coming from the same address that it's going to

That causes RTA1 to reply to its own address...

Applications talking from another computer won't have
this problem, but must have a route to the RTA1,
usually 172.29.7.7, via an interface of the emulator host

The floating point interactor client ./fp in this
directory can use argument 2 as a come-from address


	$ ./fp	172.29.7.7	172.29.7.6

	1234567890 + 123456789012345678900000000000

	4.5e600000 / 5e-600000

	3.7e+1262610 / 10

	4.9e-1262610 * 10

	.

Script ./prun also writes a configuration file for
./fp, containing these lines, so you don't have to
say any override addresses on the ./fp command line

	fp-server	172.29.7.7
	fp-client	172.29.7.6

If you don't like these addresses, change the shell
echo commands in ./prun

You have to compile fp if you are not driving an
Intel OSX Mac

	gcc -m32 -funsigned-char [-DINTEL] -o fp fp.c

You must say -DINTEL if this host is little-endian

telnet can use an option -s come-from address

	telnet	-s 172.29.7.6	172.29.7.7

snmp can have a come-from address in .snmp/snmp.conf
of your home directory

	clientaddr	172.29.7.6

Browsing the web server of the emulated RTA1 over
the emulator host loopback, you send to

	http://172.29.7.6:8080

and the firewall gives your traffic to the emulator
RTA1's responses come to 172.29.7.6, which is you

