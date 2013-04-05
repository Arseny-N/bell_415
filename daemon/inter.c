#include "inter.h"
#include "head.h"
#include "error.h"

char *str_time(char *fmt, time_t *t)
{
        struct tm tm;	
	if( localtime_r( t, &tm) == NULL ) {
		err_print("gmtime_r()");
		return NULL;
	}	       
	return __str_time(fmt, &tm);
}
char *__str_time(char *fmt, struct tm *tm)
{
	static char buf[BUF_SIZE];
	if(strftime(buf,BUF_SIZE,fmt,tm) == -1) {
		err_print("gmtime_r()");
		return NULL;
	}
	return buf;
}
char *_stime(time_t *t)
{
	return str_time("%T %d-%m-%Y",t);
}
char buf[BUF_SIZE];

char *__stime(time_t *t)
{
	//sprintf("years months days hours min sec");
	sprintf(buf,"   %ld     %ld   %ld    %ld  %ld  %ld",
		(long) *t/365*24*60*60,
		(long) *t/31*24*60*60,
		(long) *t/24*60*60,
		(long) *t/60*60,
		(long) *t/60,
		(long) *t);
	return &buf;
}
/* md - time from midnight to the bell ring 
 * returns time from now to the bell ring 
 */

time_t __get_midnight( time_t when )
{
        struct tm tm;	
	if( localtime_r( &when, &tm) == NULL ) {
		err_print("gmtime_r()");
		return (time_t) -1;
	}	

	tm.tm_sec = 0;
	tm.tm_min = 0;
	tm.tm_hour = 0;
	
	return mktime(&tm) - h24;		
}
time_t _only_hours(time_t t )
{

	struct tm tm;	
	if( localtime_r( &t, &tm) == NULL ) {
		err_print("gmtime_r()");
		return (time_t) -1;
	}	
	__zero_out_unneed(&tm);
	return mktime(&tm);
}
time_t get_inter( time_t md)
{
	time_t now;
	time (&now);
	if(now == -1) {
		err_print("time()");
		return -1;
	}
	return _get_inter(md,now);
}

/* Used for debugging reasons */
time_t _get_inter( time_t inter_md,time_t curr )
{       
	curr = _only_hours(curr);
	inter_md = _only_hours(inter_md);	
	if(curr > inter_md)  
		inter_md += h24; 
	return difftime(inter_md, curr);
}
