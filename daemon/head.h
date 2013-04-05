#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* #ifdef __HEAD_H__ */
/* # warning Warning: case of double inclusion, or accidental macro predefinition */
/* #endif */

#ifndef __HEAD_H__
#define __HEAD_H__
#define _XOPEN_SOURCE


#define TERMINATE_ON_ERR
//#define _DEBUG


#define TS_NONE       0x0   /* Bug if found in initialized function */
#define TS_DISARMED   0x1   /* Timer set but not armed*/   
#define TS_ARMED      0x2   /* Timer set but armed*/   

#define TS_ERROR      0x8   /* Errors occured */

#define _DEBUG
#define t_err(t)  t->state &= TS_ERROR; 
#define is_err(t) (t->state & TS_ERROR)
#define h0 (time_t) 0.0       
#define h24 (time_t) 24*60*60  /* A day */
//#define h24 (time_t) 20  /* Not A day */
#define ts_h0(ts) do{ ts.tv_sec = h0; ts.tv_nsec = 0L; } while(0)
#define ts_h24(ts) do{ ts.tv_sec = h24; ts.tv_nsec = 0L; } while(0) /* A timespec day */

#ifndef likely
# define likely(op) __builtin_expect(1,op)              /* doesnt work why ? */
#endif
#ifndef unlikely
# define unlikely(op) (op)
#endif

/* TODO:
 *     Journal
 *     Log
 */
#include <signal.h>
struct timers {
	int state;	

  //	time_t inter;           /* Informative */
        time_t md_inter;        /* Mid Night interval */

#define MAX_DESCR 16
	char descr[MAX_DESCR];  /* char. description */
	timer_t timerid;

	struct sigevent sigevent;
	struct timers *next;
	
	/* struct timers *prev; */
	
};
#define for_each_timer(p) for ( p = timers; p ; p = p->next )
extern struct timers *timers;
extern struct timers *last; /* = &timers; */

#define min(a,b) (a>b?b:a)
#define __timezone_adj(t) (t -= timezone)
#define BUF_SIZE 256

#endif