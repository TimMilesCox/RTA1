
/*********************************************************
	this application processes input from masmx
	or from the network
	packets are always in network byte order
	but application often runs on a machine with LE bus
	and compares backwards instead of swapping bytes
*********************************************************/


#ifdef	NETWORK_ORDER
#define	TCP_SYN	2
#define	TCP_ACK	16
#define	TCP_FIN	1
#define	TCP_RST	4
#else
#define	TCP_SYN	0x0200
#define	TCP_ACK 0x1000
#define	TCP_FIN	0x0100
#define	TCP_RST	0x0400
#endif

typedef struct { unsigned short		from_port,
					  to_port;
		 unsigned long		      seq,
					      ack;
		 unsigned short		  hl_code,
					   window,
					 checksum,
					   urgent; } segment;
 
