#ifdef	FORM_16_32
#define	NORMALISED	0x000080
#define	MANTISSA	32	
#define	MANTISSA_U	255	/* mask 1st word part mantissa */
#define	PROFILE_S_XPO	65535	/* mask characteristic field */
#define	FIELD_S_XPO	16
#define	MIDPOINT	0x00004000
#define	EXPONENT_BOUNDARY 0xFFFF8000
#endif

#ifdef	FORM_12_36
#define	NORMALISED	0x000800
#define	MANTISSA	36
#define	MANTISSA_U	4095	/* mask 1st word part mantissa	*/
#define	PROFILE_S_XPO	4095	/* mask characteristic field	*/
#define	FIELD_S_XPO	12
#define	MIDPOINT	0x00000400
#define	EXPONENT_BOUNDARY 0xFFFFF800
#endif

extern int ones_add48(int ea, int target[], int direction, smp *xcore);

