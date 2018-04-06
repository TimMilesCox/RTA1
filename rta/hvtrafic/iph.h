
#define	IPPROTO_TCP	6

typedef struct { unsigned short		 iphl_tos,
					    bytes,
					       id,
					 fragment,
					ttl_proto,
					 checksum;
		 unsigned long		 ipsource,
					   ipdest; } datagram;

