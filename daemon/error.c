#include <stdarg.h>
//#include <sys/types.h>
#include <pthread.h>
#include "head.h"
#include "error.h"
#include "ename.h"

#define __valid_err(err) (err > 0 && err < MAX_ERR) ? err : 0; 


void terminate (void) 
{
	bool need_core = 0;
#ifdef NEED_COREDUMP
	need_core = 1;
#endif
	if( need_core || getenv("NEED_COREDUMP") != NULL )
		abort();
	exit(EXIT_FAILURE);
}

static bool __first_err = 1;
static inline char *__get_ename( int err )
{
	if(__first_err) {
		__ename_init();
		__first_err = 0;
	}
	err = __valid_err(err);
	return ename[err] != NULL ? ename[err] : ename[0] ;
}
static inline void __print(FILE *s,char *fmt, va_list *va)
{
	vfprintf(s, fmt, *va);
}
void _err_print_no_exit(char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];

	
	snprintf(fmt_buf, MAX_PBUF,"Error: %s(%s):%s\n",
		 strerror(errno),__get_ename(errno),fmt);
	

	va_start(va, fmt);	
	__print(stderr,fmt_buf, &va);
	va_end(va);
	fflush(stderr);
	fflush(stdout);
}

void _nerr_print_no_exit(int err, char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];


	snprintf(fmt_buf, MAX_PBUF,"Error: %s (%s):%s\n",
		 strerror(err),__get_ename(err),fmt);
	
	va_start(va, fmt);	
	__print(stderr,fmt_buf, &va);
	va_end(va);
	fflush(stderr);
	fflush(stdout);
}
void _wrn_print_no_exit(char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];


	snprintf(fmt_buf, MAX_PBUF,"Warning:%s\n",fmt);
	
	va_start(va, fmt);	
	__print(stderr,fmt_buf, &va);
	va_end(va);
	fflush(stderr);
	fflush(stdout);
	
}
#ifdef _DEBUG
//#warning Warning: debug mode on may cause excessive verbosity
void _dbg_print(char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];

	snprintf(fmt_buf, MAX_PBUF,"Debug %s\n",fmt);
	
	va_start(va, fmt);	
	__print(stdout,fmt_buf, &va);
	va_end(va);
	fflush(stderr);
	fflush(stdout);
}
#else
void _dbg_print(char *fmt, ... )
{
}
#endif
