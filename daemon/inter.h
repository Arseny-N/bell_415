#include <time.h>
#include <string.h>
#define __tm_zero(t,v) tm->tm_##t = v

char *__str_time(char *fmt, struct tm *tm);
char *str_time(char *fmt, time_t *t);
char *_stime(time_t *t);
time_t get_inter( time_t md );
time_t _get_inter( time_t md,time_t now );
time_t _only_hours(time_t t );
char *_stime(time_t *t);
time_t __get_midnight( time_t when );


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
