#include "head.h"
#include <unistd.h>

#ifndef _GENERIC_H
#define _GENERIC_H

#define RT_SET_TIMERS 0x1


#define FIFO_NAME "/tmp/fifo"

struct request {
	int type;	
	void *stuff;
};
struct timer_set_rq {
	int num;
	struct timer_set_sub{
		char descr[MAX_DESCR]; 
		time_t inter;
		time_t md_inter;
	} **sub;
};


struct method {
	void *(*read) (void);                               /* Read a non NULL buf  */
	struct request *(*decode)(void *p);                 /* Make from this buf a rq structure */
	void (*destroy) ( struct request *rq );             /* Destroy this structure */
	void (*init) (void);                                /* Called on init */
	void (*on_exit) (void);                             /* Called on exit */
};
extern struct method method;
#endif
