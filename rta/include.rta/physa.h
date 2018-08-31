extern unsigned char * physa(unsigned char *name);

#ifdef	LINUX
extern void ifidxa(unsigned char *name, struct sockaddr_ll *);
#else
extern void ifidxa(unsigned char *name, struct sockaddr_dl *);
#endif

