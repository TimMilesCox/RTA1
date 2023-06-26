
/*	_NAMES of ports as referenced in implementation or emulation 		*/

#define	_CORE_PIN		73	/* read-only port 1 */ _CORE_SOCKET	*/

#define	_XI_PENDING		74	/* mask of external interrupt sources	*/
#define	_XI_ACK			75	/* writeable ack to quench pending signals	*/
#define	_INCREMENTER_RESTART	76
#define	_CORE_SOCKET		77	/* socket index this core read only	*/

#define	_MCAST_PENDING		78	/* mask of cores which have multicast to here	*/
#define	_MCAST_TO		79	/* -> latent parameter			*/
					/* destinations mask in register k	*/
#define	_TZONE			101

#define	_APP_THRESHOLD		72	/* lowest application code page / limit ISRs	*/
#define	_INDICATE		102	/* control commands powersave + others	*/
#define	_CHILLDOWN		103	/* powersave microseconds		*/

#define	_SYS_SWITCH		126	/* if a switch block is connected	*/
#define	_SYS_LEDS		127	/* if a LEDs block is connected		*/

#define	_DEVICE			128	/* descriptors 64 devices starting executable space	*/

					/* guards for kernel storage / net buffers		*/
#define	_RAM_THRESHOLD		124	/* start of RAM &  kernel data pages may = 122		*/
#define	_IFRAME_U		64	/* highest page instruction frame B0 + iframe pages - 1	*/

#define	_I_DATA			80	/* external data read port		*/
#define	_O_DATA			88	/* external daza write port		*/

#define	_CORES_MASK		125	/* bit map cores present +(1*/RTA_SMP)-1	*/
