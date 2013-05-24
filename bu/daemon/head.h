#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#ifndef __HEAD_H__
# define __HEAD_H__

//# define TERMINATE_ON_ERR

# define _DEBUG

# define h0 (time_t) 0       
# define h24 (time_t) 24*60*60  /* A day */
//#define h24 (time_t) 20  /* Not A day */

# define ts_h0(ts) do{ ts.tv_sec = h0; ts.tv_nsec = 0L; } while(0)
# define ts_h24(ts) do{ ts.tv_sec = h24; ts.tv_nsec = 0L; } while(0)

# define __timezone_adj(t) (t -= timezone)
# define IS_EXIT_SIG(sig) (sig == SIGTERM || sig == SIGQUIT || sig == SIGINT)

# define BUF_SIZE   256
# define MAX_QUERY  1024
# define MAX_PROFID 16

# define PERMS 0777
# define SIG_REXEC SIGHUP
# define SIG_RING_SHORT  SIGUSR1
# define SIG_RING_LONG  SIGUSR2
# define SIG_RING_CUSTOM  SIGRTMIN

# define BD_MAX_CLOSE  256
# define BD_NEED                     0x1	 
# define BD_NO_UMASK0		    0x2	 
# define BD_NO_CHDIR		    0x4	 
# define BD_NO_CLOSE_FILES	    0x8	 
# define BD_NO_REOPEN_STD_FDS	    0x10 
#include <stdarg.h>
#include "cmd.h"


static int vsystem(char *cmds, ... )
{
	char cmd_buf[BUF_SIZE];
	va_list va;
	
	va_start(va, cmds);	
	vsnprintf( cmd_buf, BUF_SIZE, cmds, va);
	va_end(va);
	printf("%s\n",cmd_buf);
	FILE *p = popen(cmd_buf, "r");
	if(p == NULL) {
		perror("popen");
		return -1;
	}		
	while(fgets(cmd_buf, BUF_SIZE, p)) {
		if(fputs(cmd_buf,cmd.out_fp) == -1) 
			return -1;
	}
	int r = 0;
	if(ferror(p)) {
		perror("fgets");
		r = -1;
	}
	if(pclose(p) == -1) {
		perror("pclose");
		r = -1;
	}
	return r;
	//return system(cmd_buf);
}
#include <sys/types.h>
#include <unistd.h>
static inline void print_locks( void )
{	
	printf("print_locks\n");
	vsystem("grep %ld /proc/locks"/*" >> /home/arseni/Documents/p/bell/locks"*/, (long)getpid());
}
/*
#ifndef likely 
#  define likely(x)      __builtin_expect(!!(x), 1)
#  define unlikely(x)    __builtin_expect(!!(x), 0)
# endif
*/
#endif
