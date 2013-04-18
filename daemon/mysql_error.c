#include "mysql.h"
#include "error.h"

static inline void __print(FILE *s,char *fmt, va_list *va)
{
	vfprintf(s, fmt, *va);
}
static inline void flush_streams(void)
{       
	fflush(stderr);
	fflush(stdout);
}

void _mysqle_print_no_exit(MYSQL *p, char *fmt, ... )
{
	va_list va;
	char fmt_buf[MAX_PBUF];


	snprintf(fmt_buf, MAX_PBUF,"MySql Error:\'%s\' (%s)\n", mysql_error(p),fmt);
	
	va_start(va, fmt);	
	__print(stderr,fmt_buf, &va);
	va_end(va);

	flush_streams();
}
