#define	GRANULE		64
#define	DIRECTORY_BLOCK	1024
#define	CONTROL_WORDS	4
#define	VOLUME1_WORDS	3

typedef struct { msw                rfw,
                            write_point,
                              remainder;
                 dmsw granule_next_page;  } page_control;

typedef struct { msw                rfw;
                 msw             offset;
                 dmsw           granule;
                 msw            name[1];  } forward;

typedef struct { msw                rfw,
                               granules;
                 dmsw           granule;  } extent1;


typedef struct { extent1             ex;
                 msw    name[DIRECTORY_BLOCK
                - CONTROL_WORDS   - 1
                - 5 * 2
                - VOLUME1_WORDS   - 1] ; } volume;

typedef struct { page_control	   page;
		 forward	   this,
				     up;
		 volume		  label; } vpage;
