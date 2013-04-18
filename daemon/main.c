//#include <pthread.h>
//#include <time.h>
//#include <string.h>
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
	setbuf(stderr, NULL);
	setbuf(stdout, NULL);

	if(!is_path_absolute(argv[0])) {
		wrn_print("Require execution with absolute path(%s)",argv[0]);
		terminate();
	}	
	dbg_print("Hello");
	wrn_print("Hello");

	cmd_defaults(&cmd,argv[0]);
	cmd_process(argc,argv,&cmd);	
	
	if(!cmd.no_dump_config)
		dump_cmd(&cmd);
	
	if(cmd.daemon_flags)
		if(become_daemon(cmd.daemon_flags))
			wrn_print("Running not demonized because of error");
	if(cmd.pid_file) {
		int r = create_pid_file(cmd.pid_file);
		if(r == -2) {
			wrn_print("An instance of %s is already running",cmd.path);
			exit(EXIT_SUCCESS);
		}
		if(r == -1) 
			wrn_print("creat_pid_file failed");
	}
	dbg_print("Hello");
	wrn_print("Hello");
	
	if(cmd.exit_after_dump) 
		exit(EXIT_SUCCESS);
	
	block_all_sigs();
	
	main_loop();
	
	wrn_print("suspicious main_loop return...");
	exit(EXIT_FAILURE);
}
