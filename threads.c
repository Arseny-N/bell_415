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
	

	dbg_print("unleashed");

//print_timers();
	
	/* More RT's could be added if needed */

	switch(rq->type) {
	case RT_ADD_TIMER:
		
		a_rq = rq->stuff;
		mk_timer (a_rq->descr, a_rq->inter);		
		break;
		
	case RT_DEL_TIMER:		

		r_rq = rq->stuff;
		rm_timer (r_rq->descr);		
		break;

	default:
		wrn_print("Unknown rq type %d",rq->type);
		break;
	}

	dbg_print("terminated");

}

void reader(int f) 
{	

	method.init();
	for( ;; ) {				
		void * buf = method.read();
		if ( buf == NULL ) {
		        wrn_print( "generic_read returned NULL" );
			break;
		}
		struct request *rq = method.decode(buf);
		if ( rq == NULL ) {
			wrn_print( "decode_string returned NULL" );
			break;
		}
		serve_rq (rq);
		method.destroy (rq);
	}
	method.on_exit();	
}

void restorer_thread(union sigval void_sigval)
{	
	ring_the_bell();       	
}
