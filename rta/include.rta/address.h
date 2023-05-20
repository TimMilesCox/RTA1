


#ifdef	INTEL
#define	PORT(X)	((X >> 8) | (X << 8) & 0xFFFF)
#else
#define	PORT(X)	X
#endif


#ifdef	INTEL
#define	USTDOUT	0x1200
#define	LOCAL	0x0100007F
#define	CONSOLE	0x05071DAC
#define	PLATFORM 0x06071DAC
#else
#define	USTDOUT	18
#define	LOCAL	0x7F000001
#define	CONSOLE	0xAC1D0705
#define	PLATFORM 0xAC1D0706
#endif

