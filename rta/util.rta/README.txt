	Utilities lstring slab fsformat fds24
	_____________________________________

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

fsformat loads a file system image which can be made
available to the emulated machine at startup. This enables
the emulated RTA1 to run a web server and send its
documentation to browsers on other computers

All those files are RTA1-readable

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


lstring and slab
________________

	lstring -b input[.txo] output[.bxl]

-b option selects the binary format which slab loads to
ROM image. Otherwise you get S-Records for quantum-24

The input file is named in full. masmx by default writes
extension .txo for text-encoded-binary

The output file is named in full. .bxl means something
like binary load string, is meant to help remember what
the file is, and doesn't clash with anything else

Here is a part of the RTA1 system image output from
lstring with -b option



	$ ../util.rta/fds24 srom.bxl
	00000000: 000001 49ffbe 000000 000000  "   I        "
	00000004: b60040 00000b d9fa7c 000000  "  @     |   "
	00000008: 000040 5500fc 10008f 10008e  "  @U        "
	0000000c: 04007c 200087 680084 dd0001  "  |   h     "
	00000010: de0049 3e004a f70100 350000  "  I> J   5  "
	00000014: 000001 fffebe 000000 000100  "            "
	00000018: 000040 000077 45324d 000000  "  @  wE2M   ".

	$ 

There is a four word descriptor: word-count, checksum,
48-bit address

The address may be relative to the start of executable space
or some other load address, depending how your loader is

A loader loading at a run-time-determined address would
need to plug a vector bank with the program's own entry
points and bank addresses. That's for loaders running on RTA1

See the OS web page for the structure and
use of the task vector bank

	../rta.doc/smaragd7.html

smaragd7.html is also available online from the emulated RTA1 web host

The .bxl print above is the front of the 28K-word system image running
on the emulated RTA1

The first descriptor says it has one word of code at absolute zero.
It's a jump instruction to absolute address 64

11 instructions at location 64 initialise the interrupt stack pointer
and read memory configuration from an I/O port. That assists decisions
on loadable vs ROM system image, and where data RAM starts. Then a GO
instruction jumps inter-bank. Its indirect operand is a constant at
address 256 (operand addresses < 256 are registers). The constant has
the value 64. That means go to the first location of a bank at absolute
address 4096

Banks are entered at a 64-word jump pad at their start. The GO target is

	(bank_absolute_start_address/64) OR (offset AND 63)

Very little happens in the 4K restart bank at address zero. It's
reserved for an optional loader and transfers to the second bank
at absolute 4096. The first interrupt vectors are absolute 4097..4159,
locations 1..63 of the second bank

You can see this in the assembly listing

	../rta.gen/list.txt

Here's how the image start looks after the next step

	../util.rta/slab srom.bxl srom.rom 28 -k

No descriptors, just a picture of memory. We read eight
words from zero, then tell fds24 to scan from word 64, and
finally display the constant at word 256


	$ ../util.rta/fds24 srom.rom
	00000000: b60040 000000 000000 000000  "  @         "64
	00000040: 5500fc 10008f 10008e 04007c  "U          |"
	00000044: 200087 680084 dd0001 de0049  "   h       I"
	00000048: 3e004a f70100 350000 000000  "> J   5     "256
	00000100: 000040 000000 000000 000000  "  @         ".

	$

If any load strings point outside the area described at
argument 3 of slab, slab won't finish the job

The default ROM size is 1024 words. The number you place at
argument 3 of slab is interpreted according these options. Options
can go anywhere on the line, but these options are reader-helpful
next to the size value at argument 3 of slab

	$ slab input output size -k	# kilowords
				 -p	# pages of 4K words
				 -b	# banks of 262144 words
				 -m	# megawords

					# none of the above: words

slab also has an -h option which tells you all this and exits,
and a -v option which comments on its progress

slab is not the only way of writing a rom image. You
may have a loader which writes from .bxl strings
onto an erased ROM

Or a loader running on RTA1 itself and reading .bxl
strings into executable space


fsformat
________

This is rta/mrand7, a script for constructing a file system image for 
RTA1 to read. That's how emulated RTA1 runs web server about itself

	util.rta/fsformat fsimages/rand7 < fsload

Here is fsload, a command list for fsformat. There is no indentation
in the list, that's just for here


	volume remington
	tree boot$
	tree left_boot
	.
	tree right_boot
	.
	.
	tree free$tree$
	.
	tree web
	file index.html rta.doc/index.html
	file rta.html rta.doc/rta.html
	file smaragd7.html rta.doc/smaragd7.txt
	file language.html rta.doc/language.txt
	file rtadef.html rta.doc/rtadef.txt
	file masmx.html ../masmx.7r2/work.txt
	file connect.html rta.doc/connect.txt
	file footnote.html rta.doc/footnote.txt
	file gnu_gpl.html rta.doc/gnu_gpl.txt
	.
	.

volume gives a volume label. That acts as owner of the
top directory

tree means a directory

. means end of directory and end of the whole list

There must be a . per tree plus one at the end

If you forget a . anywhere, you get unintended nesting

If a tree command follows another tree before any .
the second tree is inside the earlier one

file has two arguments, the name inside the RTA1 file
system, and the PC file to get the data from

the main point is the files in web/ where RTA1's
http server looks for documents

There is a reason for free$tree$ which is explained later

The file system architecture loaded here is offered
as generic for any underlying sort of filestore
on the RTA1

There is one file system driver in the RTA1 target image
and it is just sufficient to retrieve these pages. This
generic FS architecture fits over that driver, which
is for the RTA1 filestore arrays known as "devices",
in reality a memory parallel to and outside of executable
space. That actual driver is discussed a little later


The Generic FS Architecture
___________________________

fsformat preconstructs RTA1 file system images organised
in granules of 64 words of 24 bits

The granule pointers used in internal descriptors are
48 bits in size, and therefore the architecture imposes
a limit of 16384 terawords per filesystem device

That's from 48 bits of granule-ID and 6 bits of word-offset
totalling 54 bits of file space address in the device

Here is a view of the FS image built with the fsformat text above

A volume label points to the next assignable granule in the device,
and shows the next writable word in block 1 of the top directory,
the number of remaining writable words in the block, and finally
its own name, "remington"

Directory descriptors point to the granule address
of their own directory first block, and show the
directory block size in granules, the next writable
word of the directory block (= words written so far),
remaining writable words of the directory block, and
the directory name

File descriptors include a first extent descriptor,
a count of octets written in the file, and the
filename

There are plenty of other things you can add to a
file descriptor, but they can be added a lot later

The extent descriptor shows the extent starting granule
address, and the extent size in granules  The extent
descriptor contains finally the granule-ID and word-offset
of the next extent descriptor

link "." in each directory points to the granule-ID
word-offset of the directory descriptor in the parent
directory

link ".." in each directory points to the granule-ID
word-offset of the parent directory's descriptor in
its own parent directory

"." and ".." in the top directory are different.

"." in the top dirctory points to the volume label
and ".." doesn't

	$ util.rta/fds24 fsimages/rand7
	00000000: 4c0004 00000a 000000 000000  "L           "
	00000004: 2e0000 4c0004 000000 ffffff  ".  L        "
	00000008: ffffff 2e2e00 560008 0fefa9  "   .. V     "
	0000000c: 000000 001057 00002c 0003d4  "     W  ,   "
	00000010: 72656d 696e67 746f6e 440007  "remingtonD  "
	00000014: 000010 000000 000010 00001d  "            "
	00000018: 0003e3 626f6f 742400 440009  "   boot$ D  "
	0000001c: 000010 000000 000040 00000a  "        @   "
	00000020: 0003f6 667265 652474 726565  "   free$tree"
	00000024: 240000 440006 000010 000000  "$  D        "
	00000028: 000050 000088 000378 776562  "  P     xweb"
	0000002c: 000000 000000 000000 000000  "            "
	00000030: 000000 000000 000000 000000  "            "01400
	00001400: 4c0004 000025 000000 000000  "L    %      "
	00001404: 2e0000 4c0004 00000a 000000  ".  L        "
	00001408: 000000 2e2e00 46000c 000011  "   .. F     "
	0000140c: 000000 000060 000000 000000  "     `      "
	00001410: 000000 000000 000cbb 696e64  "         ind"
	00001414: 65782e 68746d 6c0000 46000b  "ex.html  F  "
	00001418: 000387 000000 000071 000000  "        q   "
	0000141c: 000000 000000 000000 02a51d  "            "
	00001420: 727461 2e6874 6d6c00 46000d  "rta.html F  "
	00001424: 000068 000000 0003f8 000000  "  h         "
	00001428: 000000 000000 000000 004d4c  "          ML"
	0000142c: 736d61 726167 64372e 68746d  "smaragd7.htm"
	00001430: 6c0000 46000d 00004b 000000  "l  F    K   "
	00001434: 000460 000000 000000 000000  "  `         "
	00001438: 000000 0037a0 6c616e 677561  "    7 langua"
	0000143c: 67652e 68746d 6c0000 46000c  "ge.html  F  "
	00001440: 00002b 000000 0004ab 000000  "  +         "
	00001444: 000000 000000 000000 001f94  "            "
	00001448: 727461 646566 2e6874 6d6c00  "rtadef.html "
	0000144c: 46000c 000ab4 000000 0004d6  "F           "
	00001450: 000000 000000 000000 000000  "            "
	00001454: 08068e 6d6173 6d782e 68746d  "   masmx.htm"
	00001458: 6c0000 46000c 000001 000000  "l  F        "
	0000145c: 000f8a 000000 000000 000000  "            "
	00001460: 000000 0000a7 636f6e 6e6563  "      connec"
	00001464: 742e68 746d6c 46000d 000020  "t.htmlF     "
	00001468: 000000 000f8b 000000 000000  "            "
	0000146c: 000000 000000 00176f 666f6f  "        ofoo"
	00001470: 746e6f 74652e 68746d 6c0000  "tnote.html  "
	00001474: 46000c 000055 000000 000fab  "F    U      "
	00001478: 000000 000052 000001 000000  "     R      "
	0000147c: 008054 676e75 5f6770 6c2e68  "  Tgnu_gpl.h"
	00001480: 746d6c 580006 000057 000000  "tmlX    W   "
	00001484: 001000 000000 000000 000000  "            "
	00001488: 000000 000000 000000 000000  "            "

	$ 

It's not the first objective of the RTA1 initiative to offer a
file system architecture, but RTA1 might as well have one, and
this one is giving necessary results to the web host service
running in the emulated machine

It could even be a very maintainable FS architecture
because it can be updated without heavy reorganisations

For example

	How would you carry out a move ?

		add a link in the new home to the descriptor
		of the moved object. Silence the original
		descriptor


	Why?

		the uplink ".." in each first-level subdirectory
		of a moved directory is still correct like that

		At the same time we've avoided having a file
		system constructed with down and along pointers at
		every branch


	How would you silence the moved descriptors?

		Me, I'd change the type field in the header words
		from 'F' to 'f' and from 'D' to 'd'

		(and 'L' to 'l' if you are moving a link) 

		Just a 1-bit OR, since I'm using UTF8 as the
		character set

		Then the directory searches will skip these


	How would you delete a file or even a tree?

		Move it to free$tree$. A light touch or what?

		If you remove a link which implements an earlier
		move, you are removing the target file.

		Copy the link to free$tree$. Unsilence the file
		descriptor and silence the link which is the remove
		target

		If you remove a link which is only a link, in POSIX
		you are only removing a link, so silence the link


	How would you re-use the forest of structures and free
	extents growing in free$tree$ ?

		Principally when assigning a new extent first
		hunt down a right-sized one in free$tree$
		in preference to a new extent at the next-available
		address from the volume label

		Mark the file- or extent- descriptor as not
		owning the granules any more

		Don't take the granules from a silenced descriptor.
		They are owned somewhere else

		This happens if the file is moved out of a directory
		and the directory is later removed. The move was
		implemented with a link from the new owner

		When no descriptor in the directory block owns
		any granules any longer, the directory block can
		go on a free list, to be preferred to new directory
		blocks



The Underlying FS24 File System Driver
______________________________________

A call to RTA1 file I/O necessarily goes to the FS24 file system driver
because RTA1 has no other file system driver yet

This driver organises a file system in a 24-bit memory array outside
system memory called a device

RTA1 devices are, in the initial model, a maximum of 16 Gigawords
organised in 65536 banks of 262144 words

The organisation of arrays into one executable space and 63 other device
arrays goes back to the compact layout of the relocation registers

The relocation register is flagged big or not

If not flagged big, the relocation register points to one of 4194304 pages
of 4K-words in system memory

If the relocation register is flagged big, 6 address bits select a device
instead of a page. The relocation register selects one of 65536 banks of
262144 words in any of 64 devices, totalling a teraword

Executable space or system memory is device number zero.

The task can place system memory in its address windows in either 4K-word
or 256K-word pieces

The task can place 256K-word banks of devices 1..63 in its address windows

Devices 1..63 may be simple arrays or buffered peripherals, for example
network interface dual port buffers or video RAMs

The emulated RTA1 has a 24-bit filestore at device number one. It's loaded
at emulator start with a file system image like rand7 which we have just
examined

devices can be placed in a task address window in banks of 262144 words.
File extents don't straddle banks on this sort of file system. An extent
fits in one address space window, starting and stopping somewhere in the window

You can see a big document right at the end, gnu_gpl.html, is in two extents.
That doesn't have an impact. RTA1 http server sends whole extents in one
socket send each. TCP does the rest

FS24 has approximately three calls in its service. They don't have a POSIX layer yet

	find a directory

	find a file in the directory and place its first data extent in
	an application address window

	place the file's next data extent in the application address window

