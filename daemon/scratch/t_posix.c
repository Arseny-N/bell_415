#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <time.h>
#include <string.h>

#include "head.h"
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
	if(strptime( p, "%H:%M:%S", &tm) == -1) {
		perror("strptime");
		return -1;
	}
	t = mktime(&tm);

	if( t == -1 ) {
		perror("mktime()");
		return -1;
	}

	return t;
	
}

static inline int is_expired(struct timespec *exp)
{
	time_t now;
	time(&now);
	return (exp->tv_sec < now);	
}

int push_timer(struct sigevent *se, struct timespec *exp, 
	       struct timespec *in, int flags, bool verify)
{
	struct itimerspec ts;	
	timer_t timerid;
 
	if(verify && is_expired(exp)) {	
		printf("Expired\n");
		return 1;

	}

	ts_h0(ts.it_interval);
	ts_h0(ts.it_value);

	printf("Pushing %ld\n", (long)exp->tv_sec);
	
	if(in)
		ts.it_interval = *in;
	if(exp)
		ts.it_value = *exp;

	if (timer_create( CLOCK_REALTIME, se, &timerid) == -1 ) {
		perror( "timer_create" );		
		return -1;
	}
	if( timer_settime(timerid, flags, &ts, NULL) == -1 ) {		
		perror( "timer_settime");		
		return -1;
	}
	return 0;
}
int push_timer_from_string(struct sigevent *se,  char *s_exp,  
			   char *s_in, int flags, bool verify)
{
	struct timespec exp, in;
	
	exp.tv_sec = 2;
	exp.tv_nsec = 0;

	in.tv_sec = 0;
	in.tv_nsec = 0;
	
	return push_timer(se, &exp, &in, flags, verify);
	
}
void SIGUSR_handler(int sig)
{
	printf("Recived %s signal\n",strsignal(sig));
	
}
int main(int argc, char *argv[])
{
	struct sigevent se;
	struct sigaction sa;
	
	setbuf(stdout,NULL);
	setbuf(stderr,NULL);
	
	sa.sa_handler = SIGUSR_handler;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGUSR1,&sa,NULL) == -1) {
		perror("sigaction1");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR2,&sa,NULL) == -1) {
		perror("sigaction2");
		exit(EXIT_FAILURE);
	}

	se.sigev_notify = SIGEV_SIGNAL;

	se.sigev_signo = SIGUSR1;
	push_timer_from_string( &se, argv[1], NULL, 0, 0);
	
	se.sigev_signo = SIGUSR2;
	push_timer_from_string( &se, argv[1], NULL, 0,0);
	
	for(;;)
		pause();
	
	return 0;
}
