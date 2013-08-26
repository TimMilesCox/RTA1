


#ifdef	INTEL
#define	PORT(X)	((unsigned short) X >> 8) | (X << 8)
#else
#define	PORT(X)	X
#endif

