
/**************************************************************************************
	for SMP [ emulate.h smp.h ] must be include ahead of this file to resolve psr
**************************************************************************************/


#define STACK(X)	_q = &sp;				\
			v = *_q;				\
			if (psr & 0x00800000)			\
			{					\
                           if ((v < (GUARD_RANGE_IL + X)) || (v > GUARD_RANGE_IU)	\
			   || ((v < (GUARD_RANGE_ILC + X)) && (v > GUARD_RANGE_UP)))	\ 
                           {					\
                              x_sp = GUARD_RANGE_IU;		\
                              RESTART				\
                              break;				\
                           }					\
                        }					\
                        else					\
                        {					\
                           if ((v < (GUARD_RANGE_SP + X)) || (v > GUARD_RANGE_UP))	\
                           {  					\
                              GUARD_AUTHORITY			\
                              break;				\
                           }					\
                        }


#define STACK_READ(X)	_q = &sp;				\
			v = *_q;				\
			if (psr & 0x00800000)			\
                        {					\
                           if (((unsigned) v) > (GUARD_RANGE_IU - X))			\
                           {					\
                              x_sp = GUARD_RANGE_IU;		\
                              RESTART				\
                              break;				\
                           }					\
                        }					\
                        else					\
                        {					\
                           if ((v < GUARD_RANGE_SP) || (v > (GUARD_RANGE_UP - X)))	\
                           {					\
                              GUARD_AUTHORITY			\
                              break;				\
                           }					\
                        }

#define STACK_RETURN(KEY)					\
								\
			_q = &sp;				\
			v = *_q;				\
			if (psr & 0x00800000)			\
                        {					\
                           if (((unsigned) v) > 		\
                              (GUARD_RANGE_IU - KEY))		\
                           {					\
                              x_sp = GUARD_RANGE_IU;		\
                              RESTART				\
                              break;				\
                           }					\
                        }					\
                        else					\
                        {					\
                           if (v == GUARD_RANGE_UP)		\
                           {					\
                              EXIT_INTERRUPT			\
                              break;				\
                           }					\
                           if ((v < GUARD_RANGE_SP)		\
                           ||  (v > (GUARD_RANGE_UP - KEY)))	\
                           {					\
                              GUARD_AUTHORITY			\
                              break;				\
                           }					\
                        }
