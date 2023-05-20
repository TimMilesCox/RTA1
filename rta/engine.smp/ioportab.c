#define	ISR_ONLY	8
#define	MEMTYPE		1
#define	MEMZERONLY	2
#define	EXTERNAL_IO	4
#define	BROADCAST	16

#define	B_1	MEMTYPE
#define	B_TCB	MEMTYPE+ISR_ONLY
#define	B_KERN	MEMTYPE+ISR_ONLY
#define	A_THREP	MEMZERONLY+ISR_ONLY
#define	I_PORT	EXTERNAL_IO
#define	O_PORT	EXTERNAL_IO
#define	O_INDICA	EXTERNAL_IO
#define	O_CHILL	EXTERNAL_IO
#define	I_TZONE	EXTERNAL_IO
#define	RAM_THRESH	ISR_ONLY+MEMZERONLY
#define	XI_ACK	ISR_ONLY+EXTERNAL_IO
#define	O_LEDS	EXTERNAL_IO
#define	O_INC	ISR_ONLY+EXTERNAL_IO
#define	O_MCAST	ISR_ONLY+BROADCAST


unsigned char	mask_port_outAB[192]
	= {	0, B_1, 0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0,
		    0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0,

		    0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0,
		    0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0,

		0,  B_TCB,B_KERN,B_KERN,B_KERN,B_KERN,B_KERN,B_KERN,
		A_THREP, 0, 0, XI_ACK,  O_INC, 0,ISR_ONLY, O_MCAST,

		I_PORT,I_PORT,I_PORT,I_PORT,I_PORT,I_PORT,I_PORT,I_PORT,
		O_PORT,O_PORT,O_PORT,O_PORT,O_PORT,O_PORT,O_PORT,O_PORT,

		0,0,0,0, 0, I_TZONE, O_INDICA, O_CHILL,
		0,0,0,0,  0,0,0,0,
		0,0,0,0,  RAM_THRESH, 0,0, O_LEDS,

		0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0,
		0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0,
		0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0,
		0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0 } ;

/*****************************************************
	 outA|B rules flags per I/O port
*****************************************************/
 
