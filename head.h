#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* #ifdef __HEAD_H__ */
/* # warning Warning: case of double inclusion, or accidental macro predefinition */
/* #endif */

#ifndef __HEAD_H__
#define __HEAD_H__

#define NO_MUTEX
#define TERMINATE_ON_ERR
#define _DEBUG


#define TS_NONE       0x0   /* Bug if found in initialized function */
#define TS_DISARMED   0x1   /* Timer set but not armed*/   
#define TS_ARMED      0x2   /* Timer set but armed*/   

#define TS_ERROR      0x8   /* Errors occured */

#define t_err(t)  t->state &= TS_ERROR; 
#define is_err(t) (t->state & TS_ERROR)
#define h0 (time_t) 0.0       
//#define h24 (time_t) 24*60*60  /* A day */
#define h24 (time_t) 20  /* Not A day */
#define ts_h0(ts) do{ ts.tv_sec = h0; ts.tv_nsec = 0L; } while(0)
#define ts_h24(ts) do{ ts.tv_sec = h24; ts.tv_nsec = 0L; } while(0) /* A timespec day */

#ifndef likely
# define likely(op) (op)              /* doesnt work why ? */
#endif
#ifndef unlikely
# define unlikely(op) (op)
#endif

/* TODO:
 */
#include <signal.h>
struct timers {
        int state;	
	time_t inter;           /* Informative */
#define MAX_DESCR 16
	char descr[MAX_DESCR];  /* char. description */
	
# ifdef PERTIMER_MUTEX
	pthread_mutex_t mutex;
# endif
	timer_t timerid;

	struct sigevent sigevent;
	struct timers *next;

	void *cache;	
	/* struct timers *prev; */
	
};
extern struct timers timers;
extern struct timers *last; /* = &timers; */
#include <sys/types.h>


#include <pthread.h>
#include "error.h"
# ifdef NO_MUTEX
inline static void big_lock(void) {return;}
inline static void big_unlock(void) {return;}
inline static void last_unlock(void) {return;}
inline static void last_lock(void) {return;}
inline static void tunlock(struct timers *timer) {return;}
inline static void tlock(struct timers *timer) {return;}
#endif
# ifdef PERTIMER_MUTEX

extern pthread_mutex_t last_mutex;

inline static void big_lock(void) {return;}
inline static void big_unlock(void) {return;}
inline static void tlock(struct timers *timer) 
{
	int e = pthread_mutex_lock ( &timer->mutex );
	if(e) {
		nerr_print( e, "pthread_mutex_lock %s", timer-> descr);
		t_err(timer);
	}
		
}
inline static void tunlock(struct timers *timer) 
{
	int e = pthread_mutex_unlock ( &timer->mutex );
	if(e) {
		nerr_print( e, "pthread_mutex_unlock %s", timer->descr);
		t_err(timer);
	}

}
inline static void last_lock(void) 
{
	int e = pthread_mutex_lock ( &last_mutex );
	if(e) {
		nerr_print( e, "pthread_mutex_lock last_mutex");
	}

}
inline static void last_unlock(void) 
{
	int e = pthread_mutex_unlock ( &last_mutex );
	if(e) {
		nerr_print( e, "pthread_mutex_unlock last_mutex");
	}

}


# endif

# ifdef BIG_MUTEX
extern pthread_mutex_t big_mutex;

inline static void last_unlock(void) {return;}
inline static void last_lock(void) {return;}
inline static void tunlock(struct timers *timer) {return;}
inline static void tlock(struct timers *timer) {return;}

inline static void big_lock(void) 
{
	int e = pthread_mutex_lock ( &big_mutex );
	if(e) {
		nerr_print( e, "pthread_mutex_lock big_mutex");
		t_err(timer);
	}
		
}
inline static void big_unlock(void) 
{
	int e = pthread_mutex_unlock ( &big_mutex );
	if(e) {
		nerr_print( e, "pthread_mutex_unlock big_mutex");
		t_err(timer);
	}

}
# endif
#endif
