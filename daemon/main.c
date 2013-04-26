#include <libgen.h>

#include "threads.h"
#include "error.h"
#include "head.h"
#include "cmd.h"

#include "defaults.h"

struct cmdline cmd;

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
	
	if(!is_path_absolute(argv[0])) {
		wrn_print("Require execution with absolute path(%s)",argv[0]);
		terminate();
	}		
	
	cmd_process(argc,argv,&cmd);	
	
	if(!cmd.no_dump_config)
		dump_cmd(&cmd);
	
	if(cmd.logs_enable)
		if(reopen_files() == -1) {
			wrn_print("reopen_files failed");
			terminate();
		}
	dbg_print("%s start executinon", argv[0]);
	
	if(cmd.daemon_flags) {
		dbg_print("Demonizing..");
		if(become_daemon(cmd.daemon_flags)) {
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






















