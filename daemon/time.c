#include "head.h"
#include "error.h"

#define __tm_zero(t,v) tm->tm_##t = v

static inline void __zero_out_unneed( struct tm *tm )
{
	struct tm saved_tm;

	saved_tm.tm_sec = tm->tm_sec;
	saved_tm.tm_min = tm->tm_min;
	saved_tm.tm_hour = tm->tm_hour;
	
	memset(tm, 0, sizeof(*tm));
	
 	__tm_zero(sec,0);       /* Seconds.	[0-60] (1 leap second) */  
	__tm_zero(min,0);	/* Minutes.	[0-59] */		   
	__tm_zero(hour,0);	/* Hours.	[0-23] */		   
	__tm_zero(mday,1);	/* Day.		[1-31] */	   
	__tm_zero(mon,0);	/* Month.	[0-11] */		   
	__tm_zero(year,70);	/* Year	- 1900.	 */		   
	__tm_zero(wday,0);	/* Day of week.	[0-6] */		 
	__tm_zero(yday,0);	/* Days in year.[0-365]	*/		 
	__tm_zero(isdst,0);	/* DST.		[-1/0/1]*/							    
# ifdef	__USE_BSD			
	__tm_zero(gmtoff,0);          /* Seconds east of UTC.  */
# else
	tm->__tm_gmtoff = 0;            /* Seconds east of UTC.  */
# endif
	tm->tm_sec = saved_tm.tm_sec;
	tm->tm_min = saved_tm.tm_min;
	tm->tm_hour = saved_tm.tm_hour;
}


char *str_brtime(const char *fmt, struct tm *tm)
{
	static char buf[BUF_SIZE];
	if( strftime( buf, BUF_SIZE, (fmt ? fmt : "%c %Z"), tm) == 0) {
		err_print("gmtime_r()");
		return NULL;
	}
	return buf;
}
char *str_time(const char *fmt, time_t *t)
{
        struct tm tm;	
	if( localtime_r( t, &tm) == NULL ) {
		err_print("gmtime_r()");
		return NULL;
	}	       
	return str_brtime(fmt, &tm);
}
char * curr_time (const char *fmt)
{
	time_t t = time (NULL);	       	
	return str_time(fmt, &t);
}

char *_stime(time_t *t)
{
	return str_time("%T %d-%m-%Y",t);
}

time_t str_to_time(const char *p, bool abs)
{
	time_t t = time(NULL);
	struct tm tm;	

	if(abs) {
		localtime_r(&t,&tm);
	} else {
		__zero_out_unneed(&tm);
	}
	/* Relies on glibc  */
				
	if(strptime( p, "%H:%M:%S", &tm) == NULL) {
		err_print("strptime");
		return -1;
	}
	t = mktime(&tm);

	if( t == -1 ) {
		err_print("mktime()");
		return -1;
	}

	return t;
	
}
struct tm *str_to_tm(const char *p, bool abs)
{
	static struct tm tm;    	
	
	if(abs) {
		time_t t = time(NULL);
		localtime_r(&t,&tm);
	} else {		
		__zero_out_unneed(&tm);		
	}
	/* Relies on glibc  */
				
	if(strptime( p, "%H:%M:%S", &tm) == NULL) {
		err_print("strptime");
		return NULL;
	}

	return &tm;
	
}

char *ts_to_string(struct timespec *ts)
{
	static char buf[BUF_SIZE];
	snprintf(buf,BUF_SIZE,"sec:(%lld) nsec:(%lld)",(long long)ts->tv_sec,(long long)ts->tv_nsec);
	return buf;
	
}





