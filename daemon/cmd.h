#ifndef __CMD_H
#define __CMD_H
#include <stdbool.h>
#include <stdio.h>
struct cmdline {

	bool no_dump_config;
	bool exit_after_dump;
	
	int daemon_flags;	      
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

	char *rexec_sig_time;


	char **args; 
	char *path;


};

extern struct cmdline cmd;

void cmd_process(char argc, char **argv, struct cmdline *cmd );
void dump_cmd(struct cmdline *cmd);
#endif
