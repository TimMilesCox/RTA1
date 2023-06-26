#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static int		 f1,
			 f2,
			 tuples;

static unsigned char	 instring[4];
static unsigned char	 tuple[12];
static char		 name2[240];

static off_t upointer(unsigned char *data)
{
   off_t		 offset = *data++;
   int			 down = 5;

   while (down--)
   {
      offset <<= 8;
      offset |= *data++;
   }

   return offset * 3;
} 

static off_t lpointer(unsigned char data[])
{
   int			 offset = (data[0] << 16)
				| (data[1] << 8)
				|  data[2];

   return offset * 3;
}

int main(int argc, char *argv[])
{
   off_t		 position,
			 iposition,
			 offset;

   int			 page_index,
			 page_update,
			 j,
			 x,
			 y;

   char			*p;


   if (argc < 3) printf("position start_page_index apps_name\n");
   else
   {
      page_index = atoi(argv[1]);

      f1 = open(argv[2], O_RDONLY);

      if (f1 < 0) printf("%s open failed %d\n", argv[2], errno);
      else
      {
         p = stpcpy((char *) name2, argv[2]);
         strcpy(p, ".rom");

         for(;;)
         {
            x = read(f1, tuple, 12);
            if (x < 12) break;

            if (tuple[0] & 128)
            {
               /****************************************************
			an update tuple
			point up in the ROM image file
               ****************************************************/

               offset = upointer(tuple + 6);

               if (f2 == 0)
               {
                  f2 = open(name2, O_RDWR);
                  if (f2 < 0) break;
               }

               tuples++;
               position = lseek(f2, offset, SEEK_SET);
               if (position < 0) break;

               j = read(f2, instring, 3);
               if (j < 3) break;

               page_update = (instring[0] << 16) | (instring[1] << 8) | instring[2];
               page_update += page_index;
               position = lseek(f2, offset, SEEK_SET);
               if (position < 0) break;

               instring[2] = page_update;
               instring[1] = page_update >> 8;
               instring[0] = page_update >> 16;
               
               y = write(f2, instring, 3);
               if (y < 3) break;              
            }
            else
            {
               /****************************************************
			a load string not an update tuple
			advance in the load strings file
               ****************************************************/

               iposition = lpointer(tuple);

               iposition = lseek(f1, iposition, SEEK_CUR);
               if (iposition < 0) break;
            }
         }
      }

      if (f1 < 0) printf("%s open failed %d\n", argv[2], errno);
      if (f2 < 0) printf("%s open failed %d\n", name2, errno);
      if (x < 0) printf("read failed %d %s\n", errno, argv[2]);
      if (position < 0) printf("update position %lld failed %d\n", offset, errno);
      if (j < 0) printf("pre_read ROM file failed %d\n", errno);
      if (iposition < 0) printf("input scan %lld failed %d\n", iposition, errno);
      if (y < 0) printf("update failed %d %s\n", errno, name2);

      close(f1);
      close(f2);
      printf("updates %d\n", tuples);
   }
}
