			/*********************************************
				The IO ports are given the name
				base[] because the first 72 of them
				are the relocation registers

				base 0 addresses the current instruction segment
				and words 256..4095 of this segment are
				in the operand address space, mostly so
				that instruction code can contain data constants
					
				Application data is otherwise
				based in bases 1..63

				Base 65 addresses the task control block
				and bases 66..71 address kernel data

				data based with 65..71 is not visible
				when data based with 1..7 is visible

				psr flag SEE$1 selects TCB page 65 instead of
				application automatic data page 1, ADP

				psr flags SEE$2..SEE$7 select the kernel
				data buffer bases 66..71 instead of application
				data spaces in base 2..7

                                IO ports base[128..191] contain information
				about the possible 64 Devices (essentially
				memory arrays)

				the first of these, Device 0, is system memory
				or the executable space

				System memory and any other device can be an
				array of up to 16 gigawords

				Therefore system memory plus the maximum
				63 other data arrays may be one teraword.

				Practically at least one device will be other
				than an array of 24-bit words. Network
				interfaces may populate a device with
				dual-ported 16-bit memory on the controller
				chip.


			*********************************************/


int	 base[IO_PORTS] = { 	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,

			/**********************************************
				above 72 base registers
				below 54 spare, then sys$switch
	                 	and sys$leds
			**********************************************/

							0, 0, 0, 0, 0, 0, 1, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,

			/**********************************************

				sys$switch = 1 means try to boot
				from array device 1, which should be
				a 24-bit file system with a boot file

				if the external switches are implemented
				they can select any device 1..63 for
				boot

				the next 64 IO-Ports are the device
				control flags, where bit 23 says,
				here is a device, bit 22 says it is an
				array of 24-bit words which can be
				used as a filestore, bit 21 may hint
				that the filestore has a boot file

				device zero at port 128 is always
				system memory in pages. Device flag
				bit 23 is not examined. Emulator startup
				sets the memory size

				bit 18 says this isn't a 24-bit array,
				but the data words in it are 16-bits
				network interfaces are expected to
				be 16 bit memories

				The lower 16 bits may later contain
				an indication of the highest bank index
				in the device, maximum 65535

				There are three devices in this
				emulator: system memory (device 0);
				an array of 24-bit words containing
				a filestore and a boot file (device 1);
				and an array of 16-bit words which
				will buffer a network interface (device 2)

			**********************************************/

				SYSMEM_FLAG,
				FSYS24_FLAG,
				DATA16_FLAG | (BANKS_IN_DEVICE16-1),

                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } ;

