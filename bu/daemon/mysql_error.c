#include <stdarg.h>
#include "mysql.h"
#include "error.h"

void _mysqle_print_no_exit(MYSQL *p, char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];


	snprintf(fmt_buf, MAX_PBUF,"MySql-Error:\'%s\' (%s)\n", mysql_error(p),fmt);
	
	va_start(va, fmt);	
	error_method._print( *(error_method.error_stream) ,fmt_buf, va);
	va_end(va);

	error_method.flush_streams();
}
