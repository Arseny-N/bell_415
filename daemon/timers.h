#include <stdbool.h>
#include <signal.h>
#include <time.h>

int arm_24h_sig(void);

int push_timer(struct sigevent *se, 
	       struct timespec *exp, struct timespec *in,
	       int flags,bool v);

int push_timer_from_string(struct sigevent *se,  
			   char *s_exp,  char *s_in ,
			   int flags,bool v);
