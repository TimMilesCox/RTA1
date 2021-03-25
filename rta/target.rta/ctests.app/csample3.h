#define	TEST45	(-centre != -3000000000000) * centre / divisor * multiplier
#define	TEST46	centre / -divisor * multiplier * (-centre == -3000000000000)
#define	TEST47	(-centre >= -3000000000000) * centre / divisor * -multiplier
#define	TEST48	-centre / divisor * multiplier * (-centre <= -3000000000000)
#define	TEST49	(-centre > -3000000000000) * centre / divisor * multiplier
#define	TEST50	centre / -divisor * multiplier * (centre < -3000000000000)

#define	TEST51	-6000000000000 != centre / divisor * -multiplier
#define	TEST52	-centre / -divisor * multiplier == 6000000000000
#define	TEST53	-6000000000000 >= centre / -divisor * multiplier 
#define	TEST54	centre / divisor * -multiplier <= -6000000000000
#define	TEST55	6000000000000 > -centre / divisor * multiplier
#define	TEST56	centre / divisor * -multiplier < 6000000000000

