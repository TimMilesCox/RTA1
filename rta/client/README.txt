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

./prun needs to run before any client program can
reach the emulated RTA1 on the emulator host's
loopback. ./prun tests its own success by pinging

RTA1 has servers for telnet, http and snmp, and for
this supplied floating point client program

fp
__

The floating point interactor client ./fp in this
directory can use argument 2 as a come-from address


	$ ./fp	172.29.7.7	172.29.7.6

	1234567890 + 123456789012345678900000000000

	4.5e600000 / 5e-600000

	3.7e+1262610 / 10

	4.9e-1262610 * 10

	.

Script ./prun also writes a configuration file for
./fp containing these following lines. So you don't
have to say any target or source  address overrides

	fp-server	172.29.7.7
	fp-client	172.29.7.6

If you don't like these addresses, change the shell
echo commands in ./prun

If you don't want defaults at all, change the shell
echo commands in ./prun to

	echo	fp_server	0.0.0.0  > config.fp
	echo	fp_client	0.0.0.0	>> config.fp

Otherwise tou still have an old config.fp

If you are driving an PowerPC OSX Mac you must
export RTA_BINARY in your shell profile

	export	RTA_BINARY=../binary.rta/osx.ppc

Add to your path as well. You will get the supplied
OSX PowerPC binaries. Or copy the PowerPC binary
to here if you're using ./fp from this wd

	cp ../binary.rta/osx.ppc/fp .

Type export and not just RTA_BINARY=etc

Otherwise you may not ovveride some script defaults

You have to compile fp if you are not driving an
Intel OSX at all

	./make_fp

You must in the same way export RTA_BINARY command
in your shell profile, for example

	export	RTA_BINARY=../binary.rta/linux.x86

If you are building on a big-endian platform

	./make_fp	sparc

or

	./make_fp	powerpc

It makes no difference which. There is no
cross-compilation. You are doing a native compile
on the platform which you have. There is just
one point in the code where compilation has to
know your platform bus architecture. Default
is little-endian

If your platform OS does not have a length-byte
on the front of struct sockaddr_in, you have to
tell the compilation that this is not Mac Darwin
Unix. You do this with argument 2 linux

You can't get an argument 2 without an argument 1
so for little endian you type something like

	../make_fp 	intel	linux

	../make_fp	arm	linux

intel and arm don't mean anything except that they
are not sparc or powerpc, so little-endian is
understood

If you have a big-endian platform which also has
sockaddr-in like ubuntu and not like Darwin
(i.e no length-byte on the front), one of these
does it

	../make_fp	sparc	linux

	../make_fp	powerpc	linux



telnet
______

telnet client on your platform has  option -s
or something similar for a come-from address

	telnet	-s 172.29.7.6	172.29.7.7


snmp
____

netsnmp snmp manager standalone binaries, for example

	snmpget

	snmpwalk

can have a come-from address in .snmp/snmp.conf of your
home directory

	clientaddr	172.29.7.6

browsers
________

Browsing the web server of the emulated RTA1 over
the emulator host loopback, you send to

	http://172.29.7.6:8080

and the firewall gives your traffic to the emulator
RTA1's responses come to 172.29.7.6, which is you

