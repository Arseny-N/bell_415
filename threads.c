#include <string.h>
#include <signal.h> /* sigval */

#include "threads.h"
#include "timers.h"
#include "generic.h"
#include "bell_ring.h"
#include "error.h"

#include <string.h>

static inline void thread_sigmask_to_proc ( void )
{
	sigset_t set;
	if(pthread_sigmask( SIG_SETMASK, NULL, &set ) == -1 ) 
		err_print( "thread_sigmask" );
	if(sigprocmask( SIG_SETMASK, &set, NULL ) == -1 ) 
		err_print( "sigprocmask" );
}

void *sig_thread(void *not_used)
{
	sigset_t nempty;
	siginfo_t info;
	int sig;
	if(sigfillset(&nempty) == -1 ) {
		err_print ("sigwaitinfo");
	}
	for(;;) {
		switch ( sig = sigwaitinfo(&nempty, &info) ) {
		case -1:
			err_print ("sigwaitinfo");
			/* set the old mask */
			thread_sigmask_to_proc();
			_dbg_print("sig_thread exiting");
			return;
		default:
			dbg_print("Recived %s signal", strsignal(sig));
			terminate();
			continue;
		}
	}
}

void * _server_thread(void *vrq)
{
	struct request *rq = (struct request*) vrq;
	struct timer_add_rq *a_rq;
	struct timer_del_rq *r_rq;
	struct timers *timer;
	void *rt = NULL;
	
	big_lock();
	dbg_print("unleashed");

	print_timers();
	
	/* More RT's could be added if needed */
	switch(rq->type) {
	case RT_ADD_TIMER:
		
		a_rq = rq->stuff;
		timer = timer_make(a_rq->descr);
		
		if( timer == NULL ) {
			wrn_print("timer_create %s", a_rq->descr);
			goto err;
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
	rt = NULL;
lock:
	big_unlock();
	dbg_print("terminated");
	return rt;
}

#ifndef NO_MUTEX
static inline void serve_rq(struct request *rq)
{
	void *p = malloc(sizeof(*rq));
	pthread_t dummy;
	if(p == NULL) {
		err_print( "malloc" );
		return;
	}
	memcpy (p, rq, sizeof(*rq));	
	int e = pthread_create( &dummy, NULL, _server_thread, p);
	if(e) {
		nerr_print( e, "pthread_create" );
		return;
	}
	

}
#else
static inline void serve_rq(struct request *rq)
{
	_server_thread(rq);
}
#endif
void *reader_thread(void *non_used) 
{	
	
	for( ;; ) {				
		void * buf = generic_read();
		if ( buf == NULL ) {
		        wrn_print( "generic_read returned NULL" );
			continue;
		}
		struct request *rq = decode_buf(buf);
		if ( rq == NULL ) {
			wrn_print( "decode_string returned NULL" );
			continue;			
		}
		serve_rq (rq);
		destroy_rq (rq);
	}
	
}

void restorer_thread(union sigval sigval_self)
{
	struct timers *self = (struct timers *) sigval_self.sival_ptr;	
	ring_the_bell();
	
	
}
