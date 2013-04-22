#include "head.h"
static inline void cmd_defaults(struct cmdline *cmd,char *self)
{
	cmd->daemon_flags = 1;

	
	cmd->ring_prog = "/sbin/ringer";
	cmd->host = "localhost";
	cmd->dbname = "test_3";
	cmd->user = "arseni";
	cmd->pass = "1";

	cmd->log_out = "/var/log/bell/""dbg.log";
	cmd->log_err = "/var/log/bell/""err.log";
	cmd->log_truncate = 0;
	
	cmd->err_fp = stderr;
	cmd->out_fp = stdout;
	cmd->logs_enable = 1;
	
	cmd->short_ring = "00:00:07";
	cmd->long_ring = "00:00:06";


	cmd->pid_file = "/var/run/bell/""bell_pid";
	cmd->no_dump_config = 1;
	cmd->exit_after_dump = 0;
	cmd->rexec_sig_time = NULL;
	cmd->path = self;		

	
}
