#include "head.h"
static inline void cmd_defaults(struct cmdline *cmd,char *self)
{
	cmd->daemon_flags = 1 | BD_NO_CHDIR | 0x8 | 1;

	
	cmd->ring_prog = "./ringer";
	
	cmd->host = "localhost";
	cmd->dbname = "test_3";
	cmd->user = "arseni";
	cmd->pass = "1";
	
	cmd->log_out = cmd->log_err = "/dev/null";
	
	cmd->err_fp = stderr;
	cmd->out_fp = stdout;
	  
	cmd->short_ring = "00:00:07";
	cmd->long_ring = "00:00:06";


	cmd->pid_file = "/tmp/pid_file";
	cmd->no_dump_config = 1;
	cmd->exit_after_dump = 0;
	cmd->rexec_sig_time = NULL;
	cmd->path = self;		

	
}
