#include <errno.h>
#include <string.h>
#include <sys/types.h>


#ifndef __ERROR_H__
# define __ERROR_H__
# define MAX_PBUF 64

void terminate (void) __attribute__ ((noreturn));

#ifdef TERMINATE_ON_ERR
# warning TERMINATE_ON_ERR defined
static inline void terminate_if_need_err (void)
{
	terminate();
}
#else
static inline void terminate_if_need_err (void)
{
        return; 
}
#endif
#ifdef TERMINATE_ON_WRN
# warning TERMINATE_ON_WRN defined
static inline void terminate_if_need_wrn (void)
{
	terminate();
}
#else
static inline void terminate_if_need_wrn (void)
{
        return; 
}
#endif

void _err_print_no_exit(char *fmt, ... ) 
  __attribute__ ((format (printf, 1, 2)));   
void _nerr_print_no_exit(int err,char *fmt, ... ) 
  __attribute__ ((format (printf, 2, 3)));  
void _wrn_print_no_exit(char *fmt, ... ) 
  __attribute__ ((format (printf, 1, 2)));                

#ifdef _DEBUG
# define err_print_no_exit(fmt,...) _err_print_no_exit("[%s]: "fmt,__FUNCTION__,##__VA_ARGS__)
# define nerr_print_no_exit(err,fmt,...) _nerr_print_no_exit(err,"[%s]: "fmt,__FUNCTION__,##__VA_ARGS__)
# define wrn_print_no_exit(fmt,...) _wrn_print_no_exit("[%s]: "fmt,__FUNCTION__,##__VA_ARGS__)
#else
# define err_print_no_exit(fmt,...) _err_print_no_exit(fmt,##__VA_ARGS__)
# define nerr_print_no_exit(err,fmt,...) _nerr_print_no_exit(err,fmt,##__VA_ARGS__)
# define wrn_print_no_exit(fmt,...) _wrn_print_no_exit(fmt,##__VA_ARGS__)
#endif

/* err -if a function returns error & sets errno */

#define err_print(fmt, ... )						\
	do {								\
		err_print_no_exit(fmt,##__VA_ARGS__);	\
		terminate_if_need_err();				\
	} while(0);								


/* nerr -if a function returns errorcode */

#define nerr_print(err,fmt, ... )				\
	do {							\
		nerr_print_no_exit(err,fmt, ##__VA_ARGS__);	\
		terminate_if_need_err();			\
	} while(0);					


/* wrn - if a selfmade function returns  error but does  not set errno */

#define wrn_print(fmt, ... )					\
	do {							\
		wrn_print_no_exit(fmt, ##__VA_ARGS__);		\
		terminate_if_need_wrn();			\
	} while(0);					


void _dbg_print( char *fmt,...) __attribute__ ((format (printf, 1, 2)));
#define dbg_print(fmt,...) _dbg_print("[%.3ld] %s: "fmt,(long)__LINE__,__FUNCTION__, ##__VA_ARGS__)



#endif
