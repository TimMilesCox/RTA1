#define	CONFIGURATION_MICROPROTOCOL	0x6969

#define	IFCONFIG_P ((i_f_string *) rxdata->frame)

typedef struct	{ unsigned char		ifclass,
					  ll_hl,
					oqif[2],
					neta[4],
					masklog,
					physaol,
					physa[0]; } i_f_string;

