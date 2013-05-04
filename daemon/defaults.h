#include "head.h"
static inline void cmd_defaults(struct cmdline *cmd,char *self)
{
	cmd->need_daemon = 1;

#if 0	
	cmd->ring_prog = "/sbin/ringer";
	cmd->host = "localhost";
	cmd->dbname = "test_3";
	cmd->user = "arseni";
	cmd->pass = "1";
#else
	cmd->ring_prog = RING_PROG;
	
	cmd->host = DEFAULT_HOST;
	cmd->dbname = DEFAULT_DB;
	
	cmd->user = DEFAULT_USER;
	cmd->pass = DEFAULT_PASS;
#endif		
	
	cmd->log_out = DEBUG_LOG_FILE;
	cmd->log_err = ERROR_LOG_FILE;
	cmd->log_truncate = 0;
	cmd->kill_running = 0;
	cmd->rexec_running = 0;
	
	cmd->err_fp = stderr;
	cmd->out_fp = stdout;
	cmd->logs_enable = 1;
	
	cmd->short_ring = "00:00:07";
	cmd->long_ring = "00:00:06";


	cmd->pid_file = PID_FILE;
	
	cmd->no_dump_config = 1;
	cmd->exit_after_dump = 0;
	cmd->rexec_sig_time = NULL;
	
	cmd->creds.uid = (long)DEFAULT_UID;
	cmd->creds.gid = (long)DEFAULT_GID;
	
	cmd->creds.user = NULL;	
	cmd->creds.group = NULL;
	
	cmd->path = self;		

}
