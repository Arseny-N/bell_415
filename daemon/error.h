#include <errno.h>
#include <string.h>
#include <sys/types.h>


#ifndef __ERROR_H__
# define __ERROR_H__
# define MAX_PBUF 64

#ifndef NO_MYSQL
# include "mysql.h"
#endif
# include "head.h"

void terminate (void) __attribute__ ((noreturn));

void _err_print_no_exit(char *fmt, ... )   __attribute__ ((format (printf, 1, 2)));   
void _nerr_print_no_exit(int err,char *fmt, ... )  __attribute__ ((format (printf, 2, 3)));  
void _wrn_print_no_exit(char *fmt, ... )   __attribute__ ((format (printf, 1, 2)));                

#ifndef NO_MYSQL
void _mysqle_print_no_exit(MYSQL *err,char *fmt, ... )  __attribute__ ((format (printf, 2, 3)));  
#endif

#ifdef _DEBUG
# define err_print_no_exit(fmt,...) _err_print_no_exit("[%s]: "fmt,__FUNCTION__,##__VA_ARGS__)
# define nerr_print_no_exit(err,fmt,...) _nerr_print_no_exit(err,"[%s]: "fmt,__FUNCTION__,##__VA_ARGS__)
# define wrn_print_no_exit(fmt,...) _wrn_print_no_exit("[%s]: "fmt,__FUNCTION__,##__VA_ARGS__)
# define mysqle_print_no_exit(err,fmt,...) _mysqle_print_no_exit(err,"[%s]: "fmt,__FUNCTION__,##__VA_ARGS__)
#else
# define err_print_no_exit(fmt,...) _err_print_no_exit(fmt,##__VA_ARGS__)
# define nerr_print_no_exit(err,fmt,...) _nerr_print_no_exit(err,fmt,##__VA_ARGS__)
# define wrn_print_no_exit(fmt,...) _wrn_print_no_exit(fmt,##__VA_ARGS__)
# define mysqle_print_no_exit(err,fmt,...) _mysqle_print_no_exit(err,fmt,##__VA_ARGS__)
#endif

/* err -if a function returns error & sets errno */
#ifdef TERMINATE_ON_ERR
# warning TERMINATE_ON_ERR defined
# define err_print(fmt, ... ) do { err_print_no_exit(fmt,##__VA_ARGS__); terminate(); } while(0)
#else
# define err_print(fmt, ... ) err_print_no_exit(fmt,##__VA_ARGS__)			
#endif


/* nerr -if a function returns errorcode */
#ifdef TERMINATE_ON_ERR
# define nerr_print(err,fmt, ... ) do { nerr_print_no_exit(err,fmt, ##__VA_ARGS__); terminate(); } while(0)
#else
# define nerr_print(err,fmt, ... ) nerr_print_no_exit(err,fmt, ##__VA_ARGS__);
#endif


#ifdef TERMINATE_ON_ERR 
# define mysqle_print(err,fmt, ... ) do { mysqle_print_no_exit(err,fmt, ##__VA_ARGS__); terminate(); } while(0)
#else
# define mysqle_print(err,fmt, ... ) mysqle_print_no_exit(err,fmt, ##__VA_ARGS__)
#endif


/* wrn - if a selfmade function returns  error but does  not set errno */
#ifdef TERMINATE_ON_WRN
# warning TERMINATE_ON_WRN defined
# define wrn_print(fmt, ... ) do { wrn_print_no_exit(fmt, ##__VA_ARGS__); terminate(); } while(0)
#else
# define wrn_print(fmt, ... ) wrn_print_no_exit(fmt, ##__VA_ARGS__)
#endif



void _dbg_print( char *fmt,...) __attribute__ ((format (printf, 1, 2)));
#define dbg_print(fmt,...) _dbg_print("[%s]: "fmt,__FUNCTION__, ##__VA_ARGS__)

typedef void (*err_print_func_t) (FILE *s, char *fmt, va_list va);
typedef void (*flush_error_streams_t) (void);

struct error_method
{
	err_print_func_t _print;
	flush_error_streams_t flush_streams;
	
	bool need_core;	
	FILE **error_stream;
	FILE **debug_stream;
};

extern struct error_method error_method;
void init_error(void);
#endif
