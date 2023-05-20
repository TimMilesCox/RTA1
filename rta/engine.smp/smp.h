#define	SMP	8
#define	DEVICES	64

typedef struct	{	int			 REGISTER[288];

			word			*APC,
						*APCZ;
			page			*B0P;
			int			*register_set;

			word			 restart_vector;

			int			 INDICATION,
						 LATENT_PARAMETER,
						 ISELECT;

			int			 PSR,
						 B0_NAME,
						 B0_SCOPE;

			/****************************************
			array element smp gets compiler rounded
			in size and must also agree with
			asm structure rta_smp.def
			otherwise pointer arithmetic will give
			wrong pointers to asm code
			so force the 32-byte metrics block to
			16-byte alignment with a spare word
			in both c and asm
			efficiency in C preferred to packed array
			***************************************/

			int			spare;

			unsigned		 DELTA,
						 _METRIC;

			unsigned long long	 DELTA_BASE,
						 TOTAL_DELTA,
						 TOTAL_METRIC;

			int			 BASE[IO_PORTS];

			long			 DATAFRAME[0]; } smp ;
