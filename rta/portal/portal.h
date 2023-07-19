
#define HOSTS   	6
#define	ADDRESS_STRING	160
#define	REVISION1

#ifdef	LINUX
#define	RAMDEV	"/dynamic/rta/"
#else
#define	RAMDEV	"/Volumes/rta/"
#endif

#define	NET	RAMDEV "net/"		/*	../temp.ifname	*/
#define	ANET	RAMDEV "anet/"		/*	../atemp.ifname	*/

unsigned char *peel(unsigned char *to, unsigned char *from);
int portal(int iftype, unsigned char *ifname, unsigned char *addresses);

