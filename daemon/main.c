#include <pthread.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#include "journal.h"
#include "threads.h"
#include "timers.h"
#include "error.h"
#include "head.h"



struct timers *timers = NULL;
struct timers *last = NULL;

/*
  Scratch:


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

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Thanks to Mikael Kerrisk */
#define BD_MAX_CLOSE  256
#define BD_NO_UMASK0               0x1
#define BD_NO_CHDIR                0x2
#define BD_NO_CLOSE_FILES          0x4
#define BD_NO_REOPEN_STD_FDS       0x8

int become_daemon ( int flags )
{
	int maxfd, fd;
	
	switch ( fork () ) {		
	case -1: return -1;
	case  0: break;
	default: _exit (EXIT_SUCCESS);
	}
	if ( setsid () == -1 )
		return -1;
	
	switch ( fork () ) {		
	case -1: return -1;
	case  0: break;
	default: _exit (EXIT_SUCCESS);
	}
  
	if ( !( flags & BD_NO_UMASK0 ) )
		umask (0);
	if ( !( flags & BD_NO_CHDIR ) )
		chdir ( "/" );
	if ( !( flags & BD_NO_CLOSE_FILES ) ) {
		maxfd = sysconf (_SC_OPEN_MAX );
		if ( maxfd == -1 )
			maxfd = BD_MAX_CLOSE;
		
		for ( fd = 0; fd < maxfd; ++fd )
			close (fd);
	}
	if ( !(flags & BD_NO_REOPEN_STD_FDS ) )
	{
		close ( STDIN_FILENO );
		fd = open ( "/dev/null", O_RDWR );
		if ( fd != STDIN_FILENO ) 
			return -1;
		if ( dup2 ( STDIN_FILENO, STDOUT_FILENO ) != STDOUT_FILENO ) 
			return -1;
		if ( dup2 ( STDIN_FILENO, STDERR_FILENO ) != STDERR_FILENO ) 
			return -1;
	}
  return 0;
}



sigset_t old_sigset;

char *jfiles[] = { "jfiles/jfile0",
		   "jfiles/jfile1",
		   "jfiles/jfile2",
		   "jfiles/jfile3",
		   NULL,};
bool need_daemon = 0;
char *startup_file = "./jfiles/jfile3";
int main ( int argc, char * argv [] )
{
	sigset_t block_set;

        /* TODO: commad line options */

	journal_open(4,jfiles);
	
	if(startup_file)
		journal_read(startup_file);
	print_timers();
	if(need_daemon)
		if(become_daemon(0)) {
			err_print("become_daemon");
			wrn_print("Running not demonized");
		}
	if(sigfillset(&block_set) == -1) 
		err_print("sigfillset");	
	if(sigprocmask( SIG_SETMASK, &block_set, &old_sigset ) == -1 ) 
		err_print( "sigprocmask" );	

	if(start_sig_thread() == -1)  {			      
		/* TODO: On error must cancel sig_thread */
		wrn_print("start_sigthread");	
		wrn_print("no sig handleing");
	}	
		
	/* 
	 * if( start_reader_thread() == -1 ) { 
	 * 	err_print("start_reader_thread"); 
	 * 	exit(EXIT_FAILURE); 
	 * } 
	 */
	
	reader(0);
	journal_close();
	exit(EXIT_SUCCESS);
}
