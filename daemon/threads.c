#include <string.h>
#include <string.h>
#include <signal.h>

#include "threads.h"
#include "timers.h"
#include "generic.h"
#include "bell_ring.h"
#include "error.h"
#include "journal.h"


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
			return NULL;
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
  //	struct timer_set_rq *s_rq;

	dbg_print("unleashed");

	
	
	/* More RT's could be added if needed */

	switch(rq->type) {

	case RT_SET_TIMERS:
		rm_all_timers();
		mk_timers_set_rq((struct timer_set_rq *)rq->stuff);
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
		void *buf = method.read();
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

		journal_write();

		method.destroy (rq);
		print_timers();
	}
	method.on_exit();	
}

void restorer_thread(union sigval void_sigval)
{	
	ring_the_bell();       	
}
