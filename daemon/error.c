#include <stdarg.h>

#include "head.h"
#include "error.h"
#include "ename.h"
#include "cmd.h"


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
	if(!s)
		return;
	vfprintf(s, fmt, *va);
}
static inline void flush_streams(void)
{       
  ////	fflush(cmd.err_fp);
  //	fflush(cmd.out_fp);
}
void _err_print_no_exit(char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];

	
	snprintf(fmt_buf, MAX_PBUF,"Error: %s(%s):%s\n",
		 strerror(errno),__get_ename(errno),fmt);
	

	va_start(va, fmt);	
	__print( cmd.err_fp ,fmt_buf, &va);
	va_end(va);
	
	flush_streams();
}

void _nerr_print_no_exit(int err, char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];


	snprintf(fmt_buf, MAX_PBUF,"Error: %s (%s):%s\n",
		 strerror(err),__get_ename(err),fmt);
	
	va_start(va, fmt);	
	__print(cmd.err_fp,fmt_buf, &va);
	va_end(va);

	flush_streams();

}
void _wrn_print_no_exit(char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];


	snprintf(fmt_buf, MAX_PBUF,"Warning:%s\n",fmt);
	
	va_start(va, fmt);	
	__print(cmd.err_fp,fmt_buf, &va);
	va_end(va);

	flush_streams();

	
}

#ifdef _DEBUG
//#warning Warning: debug mode on may cause excessive verbosity
void _dbg_print(char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];

	snprintf(fmt_buf, MAX_PBUF,"Debug %s\n",fmt);
	
	va_start(va, fmt);	
	__print(cmd.out_fp,fmt_buf, &va);
	va_end(va);

	flush_streams();
}
#else
void _dbg_print(char *fmt, ... )
{
}
#endif
