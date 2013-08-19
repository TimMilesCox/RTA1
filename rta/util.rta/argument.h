#define ARGUMENTS							\
									\
static unsigned short			 filenames;			\
static char				 flag[26];			\
static char				*filename[FILENAMES];

#define ARGUMENT							\
									\
									\
  for (x = 1; x < argc; x++)						\
  {									\
      p = argv[x];							\
									\
      symbol = *p;							\
									\
      if (symbol == '-')						\
      {									\
         p++;								\
									\
         while (symbol = *p++)						\
         {								\
            symbol |= 0x20;						\
            if ((symbol > 0x60) && (symbol < 0x7B)) flag[symbol - 'a'] = 1; \
         }								\
      }									\
      else								\
      {									\
         if (filenames < FILENAMES) filename[filenames++] = p;		\
      }									\
  }

