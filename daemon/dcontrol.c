#include <libgen.h>
#include <string.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "head.h"
#include "pid_file.c"



int main(int argc, char *argv[])
{	
	pid_t pid;		
	int r;
	
	r = read_pid_file(PID_FILE, &pid);
	if( r == -1) {
		fprintf(stderr,"read_pid_file: %s ", PID_FILE);
		perror("read_pid_file");
		return 1;
	}
	
	bool running = (r == -2);	
		
	if(strcmp(argv[1],"pid") == 0)  {
		if(running)
			printf("%ld\n", (long) pid);
		else
			printf("down\n");
		return 0;
	}
		
	if(strcmp(argv[1],"state") == 0) {			
	
		if(running)
			printf("up\n");
		else
			printf("down\n");
		return 0;
	}
		
	if(strcmp(argv[1],"down") == 0 && running)  {
		errno = 0;
		
		if(kill(pid,SIGTERM) == -1) 
			if(errno != ESRCH) {
				perror("kill");
				return 1;
			}
		return 0;
	}
	
	if(strcmp(argv[1],"up") == 0 && !running)  {
		argv[-1] = BELL_PROG;
		execvp(BELL_PROG, &argv[-1]);	
		perror("execvp");
		return 1;
	}
		
	if(strcmp(argv[1],"rexec") == 0 && running)  {
		errno = 0;
		
		if(kill(pid, SIG_REXEC) == -1) 
			if(errno != ESRCH) {
				perror("kill");
				return 1;
			}
		return 0;	
	}
	fprintf(stderr, "Illegal argument %s\n", argv[0] );
	
	return 0;
}
