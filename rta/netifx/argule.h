#define ARGUMENTS	4

#define	ARGUMENT							                        	\
static	char			 flag[28];					                    \
										                                \
static	int			     arguments;					                    \
static	char			*argument[ARGUMENTS];


#define	ARGUE							                    		    \
										                                \
   for (x = 1; x < argc; x++)							                \
   {										                            \
      p = argv[x];								                        \
										                                \
      if (*p == '-')								                    \
      {										                            \
          p++;									                        \
										                                \
          while (y = *p++)							                    \
          {									                            \
             /**************************************************		\
										                                \
                single-byte option flags, case insignificant			\
									                                	\
                the alpha values are expressed as				        \
                single-quote-alpha and never as numbers:			    \
			                                							\
                so that this code works for any data code   			\
                not just ASCII, and not just data codes		    		\
                where lowercase has higher code assigns			    	\
                than uppercase              							\
										                                \
                it does assume that the upper/lower letters 			\
                are in parallel positions in a higher or	    		\
                lower range                 							\
	                                									\
                so it reverses what is hoped to be the  				\
                upper/lower bit(s) in the uppercase letters 			\
                to change them to lowercase         					\
		                                								\
                (in ASCII that's bit 5)	            					\
					                                 					\
                For 52 case-significant one-letter-options  			\
                instead of 26 anycase one-letter options:	    		\
                                										\
                just include argument.h                 				\
                instead of this file, which is argule.h                 \
						                                				\
             *************************************************/			\
                                										\
             if ((y < 'A') || (y > 'Z'))                                \
             {                                                          \
             }                                                          \
             else y ^= 'a' ^ 'A';                                       \
                                										\
             if ((y < 'a') || (y > 'z'))            					\
             {                      									\
             }						                        			\
             else flag[y - 'a'] = 1;               						\
          }	                            								\
      }									                            	\
      else                          									\
      {								                            		\
         if (arguments < ARGUMENTS) argument[arguments++] = p;			\
      }			                            							\
   }

