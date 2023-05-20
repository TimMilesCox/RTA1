#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../include.rta/argue.h"
#include "../include.rta/address.h"
#include "../netifx/sifr_mm.h"
#include "config.h"

#define REQUEST 72


extern int			 tpexit;
extern void restart(void);

void procedure(void)
{
   char			 request[REQUEST];
   char			 text2u[REQUEST];

   char			*p;

   int			 offset,
			 port,
			 ports,
			 times;

   int			 x,
			 y;

   int			 triangle;

   int			 slab,
			 cursor;

   unsigned char	*qq;


   struct { unsigned		 bucket[14]; } tcb_states;

   #ifdef STAGED_VOLUME
   source_net.low = LOCAL_NET_RESTART;
   remote_start_port = SOURCE_PORT_RESTART;
   #endif

   for (;;)
   {
      printf("to_port/ports/connections_port, repeat, text>");
      fflush(stdout);
      memset(request, 0, REQUEST);
      p = fgets(request, REQUEST, stdin);
      if (p == NULL) break;
      if (*p == '.') break;

      if (*p == '-')
      {
         reflag(p);
         continue;
      }

      if (*p == 'r')
      {
         restart();
         continue;
      }

   }

   printf("end test procedure\n");
   tpexit = -1;
}

