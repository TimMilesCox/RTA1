#define	CONFIGURATION_MICROPROTOCOL	0x6969

#define	IFCONFIG_P ((i_f_string *) rxdata->frame)

typedef struct	{ unsigned char		 ifclass,
					   ll_hl,
					 oqif[2],
					 neta[4],
					 masklog,
					 physaol,
					physa[0]; } i_f_string;

#define ROUTE_CONFIGURATION_MICROPROTOCOL 0x6767

typedef struct { unsigned char	   indirect_flag,
                                             i_f,
                                     mask_log[2],
                                         neta[4],
				     next_hop[4]; } route_microstring;
