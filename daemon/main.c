#include <libgen.h>
#include <string.h>
#include <string.h>
#include <signal.h>

#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "error.h"
#include "head.h"
#include "cmd.h"
#include "timers.h"
#include "time.h"
#include "defaults.h"

#include "mysql.h"
#include "creds.h"
#include "caps.h"
#include "pid_file.c"

#define LOG_FILE_PERMS 0664

struct cmdline cmd;

int reopen_files(void)
{			       
	int fd;
	if( close(STDIN_FILENO) == -1) {
		err_print("close STDIN_FILENO");
		return -1;
	}
	if(open("/dev/null", O_RDONLY) != STDIN_FILENO) {
		err_print("close STDIN_FILENO");
		return -1;
	}
	if( close(STDOUT_FILENO) == -1 ) {
		err_print("close STDOUT_FILENO");
		return -1;
	}
	fd = open(cmd.log_out, O_CREAT | O_RDWR, LOG_FILE_PERMS);
	if(fd != STDOUT_FILENO) {
		err_print("open %s", cmd.log_out);
		return -1;
	}
	
	cmd.out_fp = fdopen(fd, "a");
	if(cmd.out_fp == NULL) {
		err_print("fdopen %s", cmd.log_out);
		return -1;
	}
	
	
	if( close(STDERR_FILENO) == -1 ) {
		dbg_print("Error while closing STDERR_FILENO");
		return -1;
	}
	fd = open(cmd.log_err, O_CREAT | O_RDWR, LOG_FILE_PERMS);
	if(fd != STDERR_FILENO) {
		dbg_print("Error while opening open %s", cmd.log_err);
		return -1;
	}
	cmd.err_fp = fdopen(fd, "a");
	if(!cmd.err_fp) {
		dbg_print("Error while opening stream fdopen");
		return -1;
	}
		
	return 0;
}

int become_daemon ()
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

__attribute__ ((noreturn)) void rexec(void)
{
	dbg_print("execv(path:\'%s\')",cmd.path);
	execv(cmd.path, cmd.args);
	
	err_print("execv");
	terminate();
};

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
	dbg_print("Ringing %s", buf);
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


static inline int is_path_absolute(char *path)
{
	char *safe = strdup(path);
	char c = *dirname(safe);
	free(safe);
	return c != '.';
}
static inline void block_all_sigs(void)
{
	sigset_t set;
	
	if( sigfillset(&set) == -1 ) {
		err_print("sigfillset");
		return;
	}
	
	if( sigprocmask(SIG_BLOCK,&set,NULL) == -1) {
		err_print("sigprocmask");
		return;
	}	
}

int main ( int argc, char * argv [] )
{
	
	cmd_defaults(&cmd,argv[0]);
	
	init_error();
		
	if(!is_path_absolute(argv[0])) {
		wrn_print("Require execution with absolute path(%s)",argv[0]);
		terminate();
	}		
	
	cmd_process(argc,argv,&cmd);	
	
	if(!cmd.no_dump_config)
		dump_cmd(&cmd);
	if( check_creds(&cmd.creds) == -1 )
		wrn_print("check_creds failed, program execution compromised");
		
	if( cmd.kill_running || cmd.rexec_running) {
		// dbg_print("Sending %s signal",strsignal(cmd.rexec_running ? SIGHUP : cmd.kill_running ? SIGTERM : 0));
		KILL_FROM_PID_ERRORS(kill_from_pid_file(cmd.pid_file, cmd.rexec_running ? SIGHUP : SIGTERM));
		exit(EXIT_SUCCESS);
	}

	
	if(cmd.logs_enable)
		if(reopen_files() == -1) {
			wrn_print("reopen_files failed");
			terminate();
		}
		
	dbg_print("%s started executinon", argv[0]);
	dbg_print("Running as uid:%ld gid:%ld", (long)getuid(), (long)getgid());
	
	if(cmd.need_daemon) {
		dbg_print("Demonizing..");
		if(become_daemon()) {
			wrn_print("Can't demonize because of error");
			terminate();
		}
	} 
	if(cmd.pid_file) {	
		switch (create_pid_file(cmd.pid_file)) {
	        case -1: 
			wrn_print("creat_pid_file failed");
			terminate();
		case -2:
			wrn_print("An instance of %s is already running",cmd.path);
			exit(EXIT_SUCCESS);		

		}
	}	
	
	if(cmd.exit_after_dump) 
		exit(EXIT_SUCCESS);
		
	block_all_sigs();
	
	main_loop();
	
	wrn_print("suspicious main_loop return...");
	exit(EXIT_FAILURE);
}






















