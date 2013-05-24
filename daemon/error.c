#include <stdarg.h>

#include "head.h"
#include "error.h"
#include "ename.h"
#include "cmd.h"
#include "time.h"

#define __valid_err(err) (err > 0 && err < MAX_ERR) ? err : 0; 

struct error_method error_method;

static inline char *__get_ename( int err )
{
	__ename_init();	
	err = __valid_err(err);
	return ename[err] != NULL ? ename[err] : ename[0] ;
}
static void __print(FILE *s,char *fmt, va_list va)
{
	if(!s)
		return;
	fprintf(s, "[%s] [%ld] ", curr_time("%Y-%m-%d %T"),(long)getpid());
	vfprintf(s, fmt, va);
	fflush(s);
}
static void flush_streams(void)
{       
  	fflush(*(error_method.error_stream));
  	fflush(*(error_method.debug_stream));
}



void init_error(void)
{
#ifdef NEED_COREDUMP
	error_method.need_core = 1;
#else
	error_method.need_core = 0;
#endif	
	error_method._print = __print;
	error_method.flush_streams = flush_streams;
	
	error_method.error_stream = &(cmd.err_fp);
	error_method.debug_stream = &(cmd.out_fp);
	__ename_init();	
	
}
void costum_error( err_print_func_t print,
		   flush_error_streams_t _flush_streams, 
		   FILE *err_stream, FILE *dbg_stream, 
		   bool need_core)
{
	error_method._print = print ? print : __print;
	error_method.flush_streams = _flush_streams ? _flush_streams : flush_streams;
	
	error_method.need_core = need_core;	
	error_method.error_stream = err_stream ? (&err_stream) : &stderr;
	error_method.debug_stream = dbg_stream ? (&dbg_stream) : &stdout;
	
	__ename_init();	
}

void terminate (void) 
{	
	wrn_print("Terminating.");
	if( error_method.need_core || getenv("NEED_COREDUMP") != NULL ) {
		wrn_print("ABORTING.");
		abort();
	}	
	exit(EXIT_FAILURE);
}



void _err_print_no_exit(char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];

	
	snprintf(fmt_buf, MAX_PBUF,"Error: %s (%s) %s\n",
		 fmt,__get_ename(errno), strerror(errno));
	

	va_start(va, fmt);	
	error_method._print( *(error_method.error_stream) ,fmt_buf, va);
	va_end(va);
	
	flush_streams();
}

void _nerr_print_no_exit(int err, char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];


	snprintf(fmt_buf, MAX_PBUF,"Error: %s (%s) %s\n",
		 fmt, __get_ename(errno),strerror(err));
	
	va_start(va, fmt);	
	error_method._print(*(error_method.error_stream),fmt_buf, va);
	va_end(va);

	flush_streams();

}
void _wrn_print_no_exit(char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];


	snprintf(fmt_buf, MAX_PBUF,"Warning: %s\n",fmt);
	
	va_start(va, fmt);	
	error_method._print(*(error_method.error_stream),fmt_buf, va);
	va_end(va);

	flush_streams();

	
}

#ifdef _DEBUG
//#warning Warning: debug mode on may cause excessive verbosity
void _dbg_print(char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];

	snprintf(fmt_buf, MAX_PBUF,"Log: %s\n",fmt);
	
	va_start(va, fmt);	
	error_method._print(*(error_method.debug_stream),fmt_buf, va);
	va_end(va);

	flush_streams();
}
#else
void _dbg_print(char *fmt, ... )
{
}
#endif

