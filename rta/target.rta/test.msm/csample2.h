#define	TEST31	(centre != 3000000000000) * centre / divisor * multiplier
#define	TEST32	centre / divisor * multiplier * (centre == 3000000000000)
#define	TEST33	(centre >= 3000000000000) * centre / divisor * multiplier
#define	TEST34	centre / divisor * multiplier * (centre <= 3000000000000)
#define	TEST35	(centre > 3000000000000) * centre / divisor * multiplier
#define	TEST36	centre / divisor * multiplier * (centre < 3000000000000)

#define	TEST37	6000000000000 != centre / divisor * multiplier
#define	TEST38	centre / divisor * multiplier == 6000000000000
#define	TEST39	6000000000000 >= centre / divisor * multiplier 
#define	TEST40	centre / divisor * multiplier <= 6000000000000
#define	TEST41	6000000000000 > centre / divisor * multiplier
#define	TEST42	centre / divisor * multiplier < 6000000000000

