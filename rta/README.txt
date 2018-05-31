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


_________________________________________________________________________
This is the freeware code tree
RTA1 downloads and documentation are at http://timmilescox.github.io/RTA/
_________________________________________________________________________


RTA1 is a computer processor architecture which has run in emulation
on a Unix host computer since January 2012

Scalability is claimed for the RTA1 architecture. It can run simple dedicated
microcontroller applications in a few dozen words of storage, and it can run
as a major system maintaining some millions of TCP connections simultaneously

RTA1 is designed for symmetric multiprocessing,
an important plane of scalability

Excellent code density is claimed for RTA1. The emulated prototype runs
IP, UDP and TCP in 3250 instructions. One instruction takes one storage location.

Another 4000 instructions run Telnet, MIB-II and snmp v1,
a simple HTTP server, and a floating point server program
for online interaction with RTA1 96-bit floating architecture

IP in the  emulated prototype is interfaced to real networks.
RTA1 Telnet, SNMP, HTTP and ICMP interact with other computers

RTA1 is a 24-bit word-addressing architecture with 24- 48- and 96-bit arithmetic

RTA1 minimises storage reads and writes with an internal stack and
runs without operand cache. SMP operation needs no cache coherency
electronics

RTA I/O space is a further plane of scalability

I/O cycles broadcast interrupts between SMP instruction processors for true
synchronisation and exact realtime response

I/O ports are used to organise memory space
