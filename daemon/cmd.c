#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <libgen.h>
#include "cmd.h"


static struct option long_options[] = {
	{"nodaemon",  no_argument, 0,  'D' },

	{"log",  required_argument, 0,  'l' },
	{"log_out",  required_argument, 0,  'o' },
	{"log_err",  required_argument, 0,  'e' },

	{"ring_prog",  required_argument, 0,  'r' },

	{"host",  required_argument, 0,  'h' },
	{"dbname",  required_argument, 0,  'd' },
	{"user",  required_argument, 0,  'u' },
	{"passwd",  required_argument, 0,  'p' },
	
	{"pid_file",  required_argument, 0,  'P' },
	{"help",  no_argument, 0,  'H' },
	{"exit_after_dump",  no_argument, 0,  'E' },
	{"dump_config",  no_argument, 0,  'c' },
	
	{0,         0,                 0,  0 },
};
static inline void print_help(char *prog)
{
	printf("Usage : %s\n"
	       "\t-D --nodeamon - running not demonized\n"
	       "\t-l --log - stdin/err pipeing\n",
	       basename(prog));
}

void cmd_process(char argc, char **argv, struct cmdline *cmd )
{	
	cmd->path = strdup(argv[0]);
	cmd->args = argv;
	
	for(;;) {		
		int option_index = 0;
		char c = getopt_long(argc, argv, "DlierhdupEcP",
				     long_options, &option_index);
		if(c == -1)
			break;
		switch(c) {		
		case 0:
		case 'H':
			print_help(argv[0]);
			exit(EXIT_SUCCESS);
		case 'D':
			cmd->daemon_flags = 0;
			break;
		case 'l':
			cmd->log_out = optarg;
			cmd->log_err = optarg;			
			break;
		case 'o':			
			cmd->log_out = optarg;
			break;
		case 'e':			
			cmd->log_err = optarg;			
			break;
		case 'r':
			cmd->ring_prog = optarg;
			break;
		case 'h':
			cmd->host = optarg;
			break;
		case 'd':
			cmd->dbname = optarg;
			break;
		case 'u':
			cmd->user = optarg;
			break;
		case 'p':
			cmd->pass = optarg;
			break;
		case 'E':
			cmd->exit_after_dump = 1;
		        break;
		case 'c':
			cmd->no_dump_config = 0;
		        break;
		case 'P':			
			cmd->pid_file = optarg;
			break;
		}
			
	}
}
void dump_cmd(struct cmdline *cmd)
{
	printf( "Mysql:\n"
		"\tHost: %s\n"
		"\tDb  : %s\n" 
		"\tuser: %s\n"
		"\tpass: %s\n",		
		cmd->host, cmd->dbname,
		cmd->user, cmd->pass );
	printf("Misc:\n"
	       "\terr log   : %s\n"
	       "\tout log   : %s\n"
	       "\tring prog : %s\n"
	       "\tshort ring: %s\n"
	       "\tlong ring : %s\n"
	       "\trexec_sig_time: %s\n"
	       "\tdemonized: %s\n"
	       "\texiting: %s\n",
	       cmd->log_err,cmd->log_out,
	       cmd->ring_prog,cmd->short_ring,
	       cmd->long_ring,cmd->rexec_sig_time,
	       cmd->daemon_flags ? "yes": "no",
	       cmd->exit_after_dump ?"yes": "no"
		);
}
