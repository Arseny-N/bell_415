#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <time.h>


#include "head.h"
#include "timers.h"
#include "error.h"
#include "time.h"
#include "cmd.h"

static inline int is_expired(struct timespec *exp)
{
	time_t now;
	time(&now);
	return (exp->tv_sec < now);	
}
int push_timer_from_string(struct sigevent *se,  char *s_exp,  
			   char *s_in, int flags, bool verify)
{
	struct timespec exp, in;
	
	exp.tv_sec = s_exp ? str_to_time(s_exp, 1) : 0;
	exp.tv_nsec = 0;

	in.tv_sec = s_in ? str_to_time(s_in, 0) : 0;
	in.tv_nsec = 0;
	
	return push_timer(se, &exp, &in, flags, verify);
	
}
int push_timer(struct sigevent *se, struct timespec *exp, 
	       struct timespec *in, int flags, bool verify)
{
	struct itimerspec ts;	
	timer_t timerid;
 
	if(verify && is_expired(exp)) {	
		return 0;
	}


	ts_h0(ts.it_interval);
	ts_h0(ts.it_value);

	if(in)
		ts.it_interval = *in;
	if(exp)
		ts.it_value = *exp;


	if (timer_create( CLOCK_REALTIME, se, &timerid) == -1 ) {
		err_print( "timer_create" );		
		return -1;
	}
	if( timer_settime(timerid, flags, &ts, NULL) == -1 ) {		
		err_print( "timer_settime");		
		return -1;
	}
	return 0;
}


int arm_24h_sig(void)
{	     
	struct sigevent se;
	struct timespec exp;	
	
	se.sigev_notify = SIGEV_SIGNAL;
	se.sigev_signo = SIG_REXEC;
	
	exp.tv_nsec = 0;

	exp.tv_sec = str_to_time("0:0:01", 1);
	exp.tv_sec += h24;		
	
	
	return push_timer(&se, &exp, NULL, TIMER_ABSTIME,0);
}


