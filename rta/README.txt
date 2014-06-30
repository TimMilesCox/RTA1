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


_________________________________________________________________________
This is the freeware code tree
RTA1 downloads and documentation are at http://timmilescox.github.io/RTA/
_________________________________________________________________________


RTA1 is a computer processor architecture which has run in emulation
on a Unix host computer since January 2012

Scalability is claimed for the RTA1 architecture. It can run simple dedicated
microcontroller applications in a few dozen words of storage, and it can run
as a major system maintaining some millions of TCP connections simultaneously

RTA1 is also designed from the start for symmetric multiprocessing,
a second plane of scalability

The emulated prototype is interfaced to Internet Protocol networks and
communicates with other computers with the Telnet, SNMP, HTTP and ICMP-Echo
(Ping) protocols. A floating-point server application also runs over UDP
to demonstrate the power and range of the RTA1 96-bit floating architecture

RTA1 is a 24-bit word-addressing architecture with 24- 48- and 96-bit arithmetic

Excellent code density is claimed for RTA1. The emulated prototype runs
IP, UDP and TCP in 3000 instructions which each take one storage location.

Another 4000 instructions run Telnet, MIB-II + snmpv1, a simple HTTP server,
and an online floating point server program

RTA1 is intended to work very fast without having operand cache

The large array of scratch registers is organised as an internal stack

RTA1 SMP scalability has a vast lateral dimension, without expensive
electronic co-ordination. The internal stack minimises storage reads
and writes without any operand cache


