#include <pthread.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#include "head.h"
#include "error.h"
#include "threads.h"
struct timers timers = { 
  .state = TS_NONE, 
  .next = NULL, 
};

struct timers *last = &timers;

#ifdef PERTIMER_MUTEX

pthread_mutex_t last_mutex = PTHREAD_MUTEX_INITIALIZER;

#endif

#ifdef BIG_MUTEX

pthread_mutex_t big_mutex = PTHREAD_MUTEX_INITIALIZER; /* TODO: recursive */ 

#endif
/*
  Scratch:
time_t __get_midnight( time_t when )
{
        struct tm tm;	
	if( gmtime_r( &when, &tm) == NULL ) 
		return (time_t) -1;
	
	tm.tm_sec = 0;
	tm.tm_min = 0;
	tm.tm_hour = 0;
	return mktime(&tm);		
}

time_t get_interval( time_t when )
{
	time_t mn = __get_midnight(when);
	if ( mn == (time_t) -1 ) {
		err_print( "__get_midnight %le", (double) when );
		return mn;
	}
	return difftime(when, mn);
}
*/



int main ( int argc, char * argv [] )
{
	sigset_t block_set;

	if(sigfillset(&block_set) == -1) 
	  err_print("sigfillset");	
	if(sigprocmask( SIG_SETMASK, &block_set, NULL ) == -1 ) 
	  err_print( "sigprocmask" );
	
	if(start_sig_thread() == -1)  {			      
		/* TODO: On error must cancel sig_thread */
		err_print("start_sigthread");	
		wrn_print("no sig handleing");
	}
	
	/* 
	 * if( start_reader_thread() == -1 ) { 
	 * 	err_print("start_reader_thread"); 
	 * 	exit(EXIT_FAILURE); 
	 * } 
	 */
	
	reader_thread(NULL);
	exit(EXIT_SUCCESS);
}
