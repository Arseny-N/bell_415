#include <string.h>
#include <signal.h> /* sigval */

#include "threads.h"
#include "timers.h"
#include "generic.h"
#include "bell_ring.h"
#include "error.h"

#include <string.h>
extern sigset_t old_sigset;
static inline void set_sigmask( sigset_t *s )
{

	if(pthread_sigmask( SIG_SETMASK, s, NULL ) == -1 ) 
		err_print( "thread_sigmask" );	
}

void *sig_thread(void *not_used)
{
	sigset_t set;
	siginfo_t info;
	int sig;

	if(sigfillset(&set) == -1 ) {
		err_print ("sigwaitinfo");
	}

	for(;;) {
	
		switch ( sig = sigwaitinfo(&set, &info) ) {
		case -1:
			err_print ("sigwaitinfo");
			/* set the old mask */
			set_sigmask(&old_sigset);
			_dbg_print("sig_thread exiting");
			return;
		default:
			printf("Recived %s signal(%d)", strsignal(sig), sig);
fflush(stdout);
			set_sigmask(&old_sigset);
			raise(sig);
			set_sigmask(&set);
			
			continue;
		}
	}
}
static inline void serve_rq(struct request *rq)
{
	struct timer_add_rq *a_rq;
	struct timer_del_rq *r_rq;
	struct timers *timer;

	

	dbg_print("unleashed");

	print_timers();
	
	/* More RT's could be added if needed */
	switch(rq->type) {
	case RT_ADD_TIMER:
		
		a_rq = rq->stuff;
		timer = timer_make(a_rq->descr);
		
		if( timer == NULL ) {
			wrn_print("timer_create %s", a_rq->descr);
			goto lock;
		}
		if ( timer_arm ( timer, a_rq->inter,a_rq->corr ) == -1 ) {
			wrn_print("timer_arm %s", a_rq->descr);
			goto err_disarm;
		}
		goto lock;

	case RT_DEL_TIMER:		
		r_rq = rq->stuff;
		
		dbg_print("deliting %s",r_rq->descr);
		timer = timer_find(r_rq->descr);		
		
		
		if( timer == NULL ) {
			wrn_print("no such timer %s",r_rq->descr);
			goto lock;
		}
		dbg_print("found %s", timer->descr);
		dbg_print("disarming %s", timer->descr);
		if ( timer_disarm (timer) == -1 ) {
			wrn_print("timer_disarm %s", r_rq->descr);
			goto lock;
		}
		dbg_print("destroying %s", timer->descr);
		if ( timer_destroy (timer) == -1 ) {
			wrn_print("timer_destroy %s", r_rq->descr);
			goto lock;
		}			
		goto lock;		
	default:
		goto lock;		
	}

err_disarm:
	timer_disarm (timer);
err:
	timer_destroy (timer);       
lock:

	dbg_print("terminated");

}

void reader(int f) 
{	

	for( ;; ) {				
		void * buf = method.read();
		if ( buf == NULL ) {
		        wrn_print( "generic_read returned NULL" );
			continue;
		}
		struct request *rq = method.decode(buf);
		if ( rq == NULL ) {
			wrn_print( "decode_string returned NULL" );
			continue;			
		}
		serve_rq (rq);
		method.destroy (rq);
	}
	
}

void restorer_thread(union sigval void_sigval)
{	
	ring_the_bell();       	
}
