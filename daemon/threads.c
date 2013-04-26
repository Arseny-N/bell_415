#include <string.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "threads.h"
#include "timers.h"
#include "time.h"  /* for Debugging */

#include "mysql.h"
#include "error.h"
#include "cmd.h"
#include "head.h"
#include "pid_file.c"


int reopen_files(void)
{			       
	FILE *sp;
	
	/*
	if(cmd.log_truncate) {		
		if(truncate(cmd.log_err, 0) == -1) {
			if(errno != ENOENT)
				err_print("truncate %s", cmd.log_err);
		}
		if(truncate(cmd.log_out, 0) == -1) {
			if(errno != ENOENT)
				err_print("truncate %s", cmd.log_out);
		}
	}
	*/
	sp = fopen(cmd.log_err,"a");	
	if ( !sp ) {		
		err_print("fopen");
		return -1;
	}
	cmd.err_fp = sp;			
	
      	if ( dup2 ( fileno(sp), STDERR_FILENO ) != STDERR_FILENO ) {
      		err_print("Failed to reopen err");
      		return -1;			
      	}
      	
      	
      	sp = fopen(cmd.log_out,"a");	
	if ( !sp ) {		
		err_print("fopen");
		return -1;
	}		
	
	
	cmd.out_fp = sp;
	
      	if ( dup2 ( fileno(sp), STDOUT_FILENO ) != STDOUT_FILENO ) {
      		err_print("Failed to reopen out");

	}
	
	return 0;
}
	

int become_daemon ( int flags )
{
	
	switch ( fork () ) {		
	case -1: 
		err_print("fork");
		return -1;
	case  0: 
		break;
	default: 
		_exit (EXIT_SUCCESS);
	}
	if ( setsid () == -1 ) {
		err_print("setsid");
		return -1;
	}
	
	switch ( fork () ) {		
	case -1: 
		err_print("fork");
		return -1;
	case  0: 
		break;
	default: 
		_exit (EXIT_SUCCESS);
	}
	
	umask (0);
	
	if( chdir ( "/" ) == -1 ) {
		err_print("chdir");
		return -1;
	}
	return 0;
}

void rexec(void)
{
	dbg_print("execv(path:\'%s\')",cmd.path);
	execv(cmd.path, cmd.args);
	
	err_print("execv");
	terminate();
} 
void fork_exec(char *cmd)
{
	int ret;		

	switch(fork()) {
	case -1:
		err_print("fork");
		return;
	case 0:
		ret = system(cmd);		
		if(ret == -1) {
			err_print("system");
			terminate();
		}
		exit(EXIT_SUCCESS);
	}
}
void ring(char *d)
{
	static char buf[BUF_SIZE];	
	snprintf(buf, BUF_SIZE, "%s %s", cmd.ring_prog, d);	
	fork_exec(buf);
}
static inline void zombie_assasin(void)
{
	if(waitpid(-1, NULL, WNOHANG) == -1) {
		err_print("waitpid");
	}
}

int main_loop() 
{	
	if( arm_main_timers() == -1) {
		wrn_print("failed to arm ring timers");
		terminate();
	}

	if(arm_24h_sig() == -1) {                                  
		wrn_print("failed to arm 24h \"killer timer\"");
		terminate();
	}	
	sigset_t set;
	
	if(sigfillset(&set) == -1 ) {
		err_print ("sigwaitinfo");
		return -1;
	}
	siginfo_t info;
	char *custom_dur;

	for( ;; ) {	
		int sig = sigwaitinfo(&set, &info);

		dbg_print("FETCHED %s SIGNAL(%ld)",strsignal(sig),(long)sig);

		if(IS_EXIT_SIG(sig)) {
			dbg_print("%s SIGNAL(%ld) - exit sig; exiting", strsignal(sig),(long)sig);
			exit(EXIT_SUCCESS);
		}
		
		if(sig == SIG_RING_CUSTOM) {
			custom_dur = get_duration(&info);
			ring(custom_dur);
			continue;
		}
		switch(sig) {
		case -1:
			if(errno != EAGAIN) {
				err_print("sigwaitinfo");		
				break;
			}
		case SIG_REXEC:
			rexec();
			break;	

		case SIG_RING_SHORT:
			ring(cmd.short_ring);
			break;
		case SIG_RING_LONG:
			ring(cmd.long_ring);
			break;


		case SIGCHLD:
			zombie_assasin();
			break;
		}
	}	
}


