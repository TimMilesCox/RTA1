	Utilities lstring slab fsformat 4kformat fds24
	______________________________________________

	Freeware Licensed with GNU General Public
	Licence Version 3. Please see the Licence
	Note at the end of this README
	______________________________________________


This directory contains lstring and slab, utilities
which prepare loadable system images for RTA1

masmx assembles and links RTA1 software and is documented
in its separate file tree

lstring reads the text-encoded-binary output of masmx and
constructs a load string image for loaders

slab is a loader for the output of lstring. It constructs
a ROM image on file which the emulator reads at startup

masmx, lstring and slab are shown working in series in the
target image build script ../rta.gen/make

fsformat loads a file system image in RTA target format
which is made available to the emulated machine at startup. 
This enables the emulated RTA1 to run a web server and send
its documentation to browsers on other computers

4kformat constructs a core-resident file system image in
RTA target format which can be linked into a ROM image.
Core-resident file systems allow RTA1 to have read-only
files in the absence of filestore peripherals

Files are located without directory seach in core-resident
file systems. They are used to load process RAM segments
in realtime

File systems constructed by fsformat and 4kformat have
uniform organisation


fds24
_____

fds24 is a viewer on the PC which interprets files as
arrays of 24-bit quanta. This may help you see what you're
doing

Its command line is

	fds24	filename

You hold the enter key to keep scanning. fds24 stops if
you enter . or if you scan over the end of the file. It's
easy to start again if you do

The only other command is a new word address to scan from.
This is hex with leading zero and decimal otherwise

If your platform is not OSX Intel then compile lstring,
slab, fsformat and fds24 again with the script

	./make

No parameters, it's just a native compile on the platform
that you have


lstring and slab
________________



	lstring -b input[.txo] output[.bxl]

-b option selects the binary format which slab loads to
ROM image. Otherwise you get S-Records for quantum-24

The input and output files are named in full. masmx output
files have the default extension .txo 

Here is a part of a system image output from lstring with
-b option


	$ $RTA_BINARY/fds24 srom
	00000000: 000001 49ffbe 000000 000000  "   I        "
	00000004: b60040 00000b d9fa7c 000000  "  @     |   "
	00000008: 000040 5500fc 10008f 10008e  "  @U        "
	0000000c: 04007c 200087 680084 dd0001  "  |   h     "
	00000010: de0049 3e004a f70100 350000  "  I> J   5  "
	00000014: 000001 fffebe 000000 000100  "            "
	00000018: 000040 000077 45324d 000000  "  @  wE2M   ".

	$ 

There is a four word descriptor before each load string:
word-count, checksum, 48-bit address

The address may be relative to the start of executable space
in the case of a ROM image, or to a dynamic load address

A loader loading at a run-time-determined address would
need to plug a vector bank with the program's own entry
points and bank addresses

This fds24 readout is the start of the RTA1 smaragd7 system image

The first 4-word descriptor describes one word of code at absolute zero.
It's a jump instruction to absolute address 64

11 instructions at location 64 initialise the interrupt stack pointer
and read memory configuration from an I/O port. That assists decisions
on loadable vs ROM system image, and where data RAM starts. Then a GO
instruction jumps inter-bank. Its indirect operand is a constant at
address 256 (operand addresses < 256 are registers). The constant has
the value 64. That means go to the first location of the bank at absolute
address 4096

Very little happens in the 4K restart bank at address zero. It's
reserved for an optional loader and transfers to the second bank
at absolute 4096. The first interrupt vectors are absolute 4097..4159,
locations 1..63 of the second bank

You can see this in the assembly listing

	../rta.gen/list.txt

Here's how the image start looks after the next step

	$RTA_BINARY/slab srom srom.rom 28 -k

No descriptors, just a picture of memory. We read eight
words from zero, then tell fds24 to scan from word 64, and
finally display the constant at word 256


	$ $RTA_BINARY/fds24 srom.rom
	00000000: b60040 000000 000000 000000  "  @         "64
	00000040: 5500fc 10008f 10008e 04007c  "U          |"
	00000044: 200087 680084 dd0001 de0049  "   h       I"
	00000048: 3e004a f70100 350000 000000  "> J   5     "256
	00000100: 000040 000000 000000 000000  "  @         ".

	$

If any load strings point outside the area described at
argument 3 of slab, slab won't finish the job

slab -h prints complete command line syntax

$ slab -h

slab [-options] input[.bxl] output[.rom] [target-rom-words]

default ROM size is 1024 RTA1 words
target-rom-words is leading-zero for hex else decimal

slab ifile ofile -p          #  4096 RTA1 words
slab ifile ofile -b          #  262144 RTA1 words
slab ifile ofile -m          #  1048576 RTA1 words

slab ifile ofile number      #  number of RTA1 words
slab ifile ofile number -k   #  number * 1024 RTA1 words
slab ifile ofile number -p   #  number * 4096 RTA1 words
slab ifile ofile number -b   #  number * 262144 RTA1 words
slab ifile ofile number -m   #  number * 1048576 RTA1 words
options -v verbose -w very verbose


fsformat
________

This is rta/mrand7, a script for constructing a file system image for 
RTA1 to read. That's how emulated RTA1 runs web server about itself

	util.rta/fsformat fsimages/rand7 < fsload

Here is fsload, a file and directory list for fsformat


	volume remington
	tree boot$
	tree left_boot
	.
	tree right_boot
	.
	.
	tree free$tree$
	.
	tree etc
	file network.txt etc.rta/network.txt
	.
	tree dmsa
	tree schema1
	.
	tree schema2
	.
	.
	tree web
	file index.html rta.doc/index.html
	file rta.html rta.doc/rta.txt
	file smaragd7.html rta.doc/smaragd7.txt
	file language.html rta.doc/language.txt
	file masmx7r3.html rta.doc/masmx7r3.txt
	file connect.html rta.doc/connect.txt
	file q_a.html rta.doc/q_a.txt
	file footnote.html rta.doc/footnote.txt
	file gnu_gpl.html rta.doc/gnu_gpl.html
	file file4000.html rta.doc/file4000.txt
	.
	.

volume gives a volume label. The volume label is
the top directory

tree means a directory

. means end of directory and end of the whole list

There must be a . per tree plus one at the end

If you forget a . anywhere, you get unintended nesting

If a tree command follows another tree before any .
the second tree is inside the earlier one

If file has two arguments, the first is the name in
the RTA1 file system, and the the second PC file to
load

The FS initial purpose is directory web/ where RTA1's
http server looks for RTA1 documents

The file system architecture loaded here is offered
as generic for any underlying sort of filestore
on the RTA1

There is one file system driver in the RTA1 target image
and it is just sufficient to retrieve these pages. This
generic FS architecture fits over that driver, which
is for the RTA1 filestore arrays known as "devices",
in reality a memory parallel to and outside of executable
space. That actual driver is discussed a little later

fs24 file system has a maximum extent size of 262144 words

Analagous executable-space resident core file systems
are similarly organised, but with a maximum extent size
of 4096 words. Core-resident files are visible to linkers
and do need a directory search or open. They are read-only
and constructed with utility 4kformat instead of fsformat



The Generic FS Architecture
___________________________

fsformat preconstructs RTA1 file system images organised
in granules of 64 words of 24 bits

The granule pointers used in internal descriptors are
48 bits in size, and therefore the architecture imposes
a limit of 16384 terawords per filesystem device

48 bits of granule-ID and 6 bits of word-offset
total 54 bits of file space address in the device

FS image built with the fsformat text fsload listed above
is viewed below

Page descriptors record free space in a directory block
and point forward to the start granule of the next block

Directory block size is 16 granules or 1024 words,
but may be any number of granules

A volume label points to the next assignable granule
in the device and contains the volume name

Directory descriptors point to the granule address
of their first directory block, and show its size
in granules and the directory name

File descriptors include a first extent descriptor,
a count of octets written in the file, and the
filename.

The file extent descriptor contains extent
start granule, size in granules, and a next extent
descriptor granule and word offset

Directory entries are of variable size with a type
flag and data words count in the first word 

'P' is directory block space description and forward
pointer, 'L' link, 'V' volume, 'D' directory,
'F' file, 'X' extent, and 0x80 a space descriptor
corresponding to the 'P' page control record 

Names in ASCII are a variable number of data words
at the end of descriptors

link "." in each directory points to the granule-ID
word-offset of the directory descriptor in the parent
directory

link ".." in each directory points to the granule-ID
word-offset of the parent directory's descriptor in
its own parent directory

"." and ".." in the top directory are different.

"." in the top directory points to the volume label
and ".." doesn't

	$ fds24 fsimages/rand7
	000000000000: 500004 000034 0003cb 000000  "P    4      "
	000000000004: 000000 4c0004 00000f 000000  "   L        "
	000000000008: 000000 2e0000 4c0004 000000  "   .  L     "
	00000000000c: ffffff ffffff 2e2e00 560006  "      .. V  "
	000000000010: 000018 000000 000d04 72656d  "         rem"
	000000000014: 696e67 746f6e 440005 000010  "ingtonD     "
	000000000018: 000000 000010 626f6f 742400  "      boot$ "
	00000000001c: 440007 000010 000000 000040  "D          @"
	000000000020: 667265 652474 726565 240000  "free$tree$  "
	000000000024: 440004 000010 000000 000050  "D          P"
	000000000028: 657463 440005 000010 000000  "etcD        "
	00000000002c: 000061 646d73 610000 440004  "  admsa  D  "
	000000000030: 000010 000000 000091 776562  "         web"
	000000000034: 8003ca 000000 000000 000000  "            "
	000000000038: 000000 000000 000000 000000  "            "
	00000000003c: 000000 000000 000000 000000  "            "
	000000000040: 000000 000000 000000 000000  "            "02440
	000000002440: 500004 000094 00036b 000000  "P       k   "
	000000002444: 000000 4c0004 00002f 000000  "   L    /   "
	000000002448: 000000 2e0000 4c0004 00000f  "   .  L     "
	00000000244c: 000000 000000 2e2e00 46000c  "      .. F  "
	000000002450: 000016 000000 0000a1 000000  "            "
	000000002454: 000000 000000 000000 001027  "           '"
	000000002458: 696e64 65782e 68746d 6c0000  "index.html  "
	00000000245c: 46000b 00041d 000000 0000b7  "F           "
	000000002460: 000000 000000 000000 000000  "            "
	000000002464: 031555 727461 2e6874 6d6c00  "  Urta.html "
	000000002468: 46000d 00016c 000000 0004d4  "F    l      "
	00000000246c: 000000 000000 000000 000000  "            "
	000000002470: 011062 736d61 726167 64372e  "  bsmaragd7."
	000000002474: 68746d 6c0000 46000d 0000ce  "html  F     "
	000000002478: 000000 000640 000000 000000  "     @      "
	00000000247c: 000000 000000 009a30 6c616e  "        0lan"
	000000002480: 677561 67652e 68746d 6c0000  "guage.html  "
	000000002484: 46000d 000417 000000 00070e  "F           "
	000000002488: 000000 000000 000000 000000  "            "
	00000000248c: 03108f 6d6173 6d7837 72332e  "   masmx7r3."
	000000002490: 68746d 6c0000 46000c 00006f  "html  F    o"
	000000002494: 000000 000b25 000000 000000  "     %      "
	000000002498: 000000 000000 005315 636f6e  "       S con"
	00000000249c: 6e6563 742e68 746d6c 46000b  "nect.htmlF  "
	0000000024a0: 000093 000000 000b94 000000  "            "
	0000000024a4: 000000 000000 000000 006e25  "          n%"
	0000000024a8: 715f61 2e6874 6d6c00 46000d  "q_a.html F  "
	0000000024ac: 00001c 000000 000c27 000000  "        '   "
	0000000024b0: 000000 000000 000000 001476  "           v"
	0000000024b4: 666f6f 746e6f 74652e 68746d  "footnote.htm"
	0000000024b8: 6c0000 46000c 0000ac 000000  "l  F        "
	0000000024bc: 000c43 000000 000000 000000  "  C         "
	0000000024c0: 000000 008054 676e75 5f6770  "     Tgnu_gp"
	0000000024c4: 6c2e68 746d6c 46000d 000015  "l.htmlF     "
	0000000024c8: 000000 000cef 000000 000000  "            ".
	0000000024cc: 000000 000000 000fa0 66696c  "         fil"
	0000000024d0: 653430 30302e 68746d 6c0000  "e4000.html  "
	0000000024d4: 80036a 000000 000000 000000  "  j         "
	
	$

FS24 File System Driver
_______________________

The file system is in a 24-bit peripheral memory outside system memory
called a device array

RTA1 device arrays are a maximum of 16 Gigawords organised in 65536
banks of 262144 words and may number 64 including system memory


	architectural reminder
	----------------------

	The relocation register is flagged big or not

	If not flagged big, the relocation register points to a 4K-word page
	in system memory. There may be 4194304 executable pages or 16 gigawords
	in system memory

	If the relocation register is flagged big, its six low-order bits select 
	a device array instead of a page. sixteen bits select a bank of 262144
	words. There may be 64 devices of 65536 banks, totalling a teraword

	Executable space or system memory is device number zero.

	The task can place system memory in its address windows in either 4K-word
	or 256K-word pieces

	The task can place 256K-word banks of devices 1..63 in its address windows

	Devices 1..63 may be simple arrays or buffered peripherals, for example
	network interface dual port buffers or video RAMs


The emulated RTA1 has a 24-bit filestore at device number one. It's loaded
at emulator start with a file system image like the example rand7 above 

device arrays can be placed in a task address window in banks of 262144 words.
File extents don't straddle banks on this sort of file system. An extent
fits in one address space window, starting and stopping somewhere in the window

RTA1 http server sends whole extents in one
socket send each. TCP does the rest

FS24 has approximately three calls in its service. They don't have a POSIX layer yet

	find a directory

	find a file in the directory and place its first data extent in
	an application address window

	place the file's next data extent in the application address window




___________________________________________________________________________


LICENCE NOTE


#
#    Copyright Tim Cox, 2012
#    TimMilesCox@gmx.ch
#
#    This file is part of the software emulation of the freeware
#    processor architecture
#
#                RTA1
#
#    RTA1 is a free programmable processor architecture specification.
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
#    You can redistribute and/or modify RTA1 and its materials
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

