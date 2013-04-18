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



/* Thanks to Mikael Kerrisk */
static inline int lockReg ( int fd, int cmd, int type, int whence, 
		     int start, off_t len )
{
	struct flock fl;
	
	fl.l_type = type;
	fl.l_whence = whence;
	fl.l_start = start;
	fl.l_len = len;
	
	return fcntl ( fd, cmd, &fl );
}


static int lock_region ( int fd, int type, int whence, int start, off_t len )
{
	return lockReg ( fd, F_SETLK, type, whence, start, len );
}			  
int create_pid_file (const char * pidFile)
{
	char buf [BUF_SIZE];
  
	int fd = open ( pidFile, O_RDWR | O_CREAT | O_CLOEXEC, 
			S_IRUSR | S_IWUSR );
	if ( fd == -1 ) {
		err_print("open");
		return -1;
	} 
		
	if ( lock_region ( fd, F_WRLCK, SEEK_SET, 0, 0 ) == -1 ) {
		if ( errno == EAGAIN || errno == EACCES )
			return -2;		
		else {
		        err_print("lock_region");
			return -1;		
		}
	}
	dbg_print("All ok? %d",errno);
	if ( ftruncate ( fd, 0 ) == -1 ) {
		err_print("ftrucate");
		return -1;
	}
	snprintf ( buf, BUF_SIZE, "%ld\n", (long) getpid () );
	if ( write ( fd, buf, strlen (buf) ) != strlen (buf) ) {
		err_print("write");
		return -1;
	}
  
	return fd;
}
static inline int reopen_files(void)
{			       
	FILE *sp;
	if(truncate(cmd.log_err, 0) == -1) {
		if(errno != ENOENT)
			err_print("truncate %s", cmd.log_err);
	}
	
	sp = fopen(cmd.log_err,"a");	
	if ( !sp ) {		
		err_print("fopen");
		return -1;
	}
	cmd.err_fp = sp;
	
	if(truncate(cmd.log_out, 0) == -1) {
		if(errno != ENOENT)
			err_print("truncate %s", cmd.log_out);
	}

	sp = fopen(cmd.log_out,"a");		
	if ( !sp ) {		
		err_print("fopen");
		return -1;
	}		
	cmd.out_fp = sp;
	return 0;
}
int become_daemon ( int flags )
{
	int maxfd, fd;
	
	switch ( fork () ) {		
	case -1: err_print("fork");return -1;
	case  0: break;
	default: _exit (EXIT_SUCCESS);
	}
	if ( setsid () == -1 ) {
		err_print("setsid");
		return -1;
	}
	
	switch ( fork () ) {		
	case -1: err_print("fork");return -1;
	case  0: break;
	default: _exit (EXIT_SUCCESS);
	}
	
	if ( !( flags & BD_NO_UMASK0 ) )
		umask (0);

	if ( !( flags & BD_NO_CHDIR ) ) 
		if( chdir ( "/" ) == -1 ) {
			err_print("chdir");
			return -1;
		}
	
	if ( !( flags & BD_NO_CLOSE_FILES ) ) {
		maxfd = sysconf (_SC_OPEN_MAX );
		if ( maxfd == -1 )
			maxfd = BD_MAX_CLOSE;
		
		for ( fd = 0; fd < maxfd; ++fd )
			close (fd);
	}
	if ( !(flags & BD_NO_REOPEN_STD_FDS ) )
	{
		if(reopen_files() == -1) {
			wrn_print("Reoprn files returned fault");
			return -1;
		}	
		
	}
	
	return 0;
}

void rexec(void)
{
	dbg_print("execv(path:\'%s\',args:%p)",cmd.path,cmd.args);
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


