#include <errno.h>
#include <string.h>
#include <sys/types.h>


#ifndef __ERROR_H__
# define __ERROR_H__
# define MAX_PBUF 64

void err_print( char *fmt,...) __attribute__ ((format (printf, 1, 2)));                /* err -if a function returns error & sets errno */
void nerr_print( int err, char *fmt,...) __attribute__ ((format (printf, 2, 3)));      /* nerr -if a function returns errorcode */
void wrn_print( char *fmt,...) __attribute__ ((format (printf, 1, 2)));                /* wrn - if a selfmade function 
											  returns  error but does  not set errno */

void _dbg_print( char *fmt,...) __attribute__ ((format (printf, 1, 2)));
#define dbg_print(fmt,...) _dbg_print("[%.3ld] %s: "fmt,(long)__LINE__,__FUNCTION__, ##__VA_ARGS__)
void terminate (void) __attribute__ ((noreturn));

#endif
