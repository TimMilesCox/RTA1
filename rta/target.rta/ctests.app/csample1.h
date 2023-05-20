#define	TEST21	((7000000000000000000 >> scale_right) - right) >> long_scale_right << long_scale_left
#define	TEST22	((-7000000000000000000 >> scale_right) + right) >> long_scale_right << long_scale_left
#define	TEST23	centre / divisor * multiplier & 0x7FFFFF
#define	TEST24	centre / divisor * multiplier & (unsigned int) 0x7FFFFF
#define	TEST25	left << scale_left
#define	TEST26	left << *indirect_scale_left
#define	TEST27	right >> scale_right
#define	TEST28	right >> *indirect_scale_right
#define	TEST29	centre / divisor * multiplier
#define	TEST30	

