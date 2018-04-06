
#define	APDATA	44

typedef	struct { unsigned long		  option,
					sequence;
		 unsigned char	  data[APDATA-8]; } apdata;
