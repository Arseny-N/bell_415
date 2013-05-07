#ifndef __CMD_H
#define __CMD_H
#include <stdbool.h>
#include <stdio.h>
#include "creds.h"

struct cmdline {

	bool no_dump_config;
	bool exit_after_dump;
	
	bool logs_enable;
	bool kill_running;	      
	bool rexec_running;	      
		
	bool need_daemon;	      
	char *pid_file;

	char *host;
	char *dbname;
	char *user;
	char *pass;
	
	char *ring_prog;
	char **ring_prog_args;

	char *log_out;
	char *log_err;
	
	FILE *err_fp;
	FILE *out_fp;

	char *short_ring;
	char *long_ring;

	char **args; 
	char *path;
	
	struct cmd_creds creds;

};

extern struct cmdline cmd;

void cmd_process(char argc, char **argv, struct cmdline *cmd );
void dump_cmd(struct cmdline *cmd);
#endif
