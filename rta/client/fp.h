
#define	TEXT		160

#define	FP_SERVICE	4608

#ifdef	INTEL
#define	PORT(X)	((unsigned short) X >> 8) | (X << 8)
#else
#define	PORT(X)	X
#endif

#define MAXTRY          1200
#define TWARP           2400

#undef	LINUX
#define	OSX
