#include <string.h>

#include "head.h"
#include "timers.h"
#include "threads.h"
#include "error.h"
#include <sys/types.h>
/* 
 * timers are mesaured from now to the desired time
 * for example:
 * 00:07:00 = 1363748400  (desired time)
 * 00:06:00 = 1363744800  (now)
 * interval = 0000003600
 * then they are set (automatically) to wake up after 
 * 24 hours
 *
 */
/*
 * ads a timer to the timer list
 *
 */
inline static void __append_to_list(struct timers *timer)
{	
	last_lock();
	
	last -> next = timer;
	last = last -> next;
	/* last -> next = NULL; */

	last_unlock();
	
}
inline static void __remove_from_list(struct timers *timer)
{	
	struct timers *p;
	last_lock();

	p = &timers;
	while(p) {		
		if (p->next == timer || p->next == last)
			break;
		p = p->next;
	}	
	
	if(p == NULL)
		return;
	
	p->next = timer->next;

	if(timer == last)
		last = p;

	last_unlock();
	
}
void print_timers(void) 
{
	struct timers *p;
	p = &timers;
	printf("state   err  descr\n");
                   /* "none   ""y    " */
	while(p) {
		printf("%s %s    %s\n",
			   (p->state == TS_NONE)?     "none    ":
			   (p->state == TS_ARMED)?    "armed   ":
			   (p->state == TS_DISARMED)? "disarmed":
			                              "error   ",
			   p->state & TS_ERROR ? "y": "n", p->descr);
		p = p->next;
	}
}
static inline struct timers *__timer_find(char *descr)
{	
	struct timers *p;
	p = &timers;
	while(p != NULL) {		
		/* dbg_print("(%s) (%s)",descr, p->descr); */
		if(p->descr)
			if( strncmp(p->descr, descr, MAX_DESCR) == 0) {			
				return p;
			}		
		p = p->next;
	}
	/* dbg_print("Not found"); */
	return NULL;
}
struct timers *timer_find(char *descr)
{
	struct timers *timer;
	last_lock();
	timer = __timer_find(descr);
	last_unlock();
	return timer;
}
static struct timers *get_free_timer(void)
{
	struct timers *timer;
	if ( timers.state == TS_NONE ) {
		timer = &timers;
	} else {
		timer = malloc( sizeof(struct timers) );
		if ( timer == NULL )
			return NULL;	
		timer->next = NULL;
		__append_to_list (timer);
	}
	timer->state = TS_NONE;	

	return timer;
}
static void free_timer(struct timers *timer)
{
	
	if(timer->state != TS_NONE) {
		wrn_print("freeng in use timer, operation refused");
		return;
	}
	if( timer == &timers ) {		
		return;
	}
	__remove_from_list (timer);
	free (timer);		
}
struct timers *timer_make(char *descr)
{
	struct timers *timer;
	struct sigevent *sev;
	dbg_print("making %s timer", descr);
	timer = timer_find(descr);
	if(timer) {
		wrn_print("timer %s already present", descr);
		return NULL;
	}
	timer = get_free_timer();  
	if(timer == NULL || is_err(timer)) {
		wrn_print( "get_free_timer %s",descr);
		return NULL;
	}
#ifdef PERTIMER_MUTEX
	if ( pthread_mutex_init ( &timer -> mutex, NULL ) == -1 ) {
		err_print ( "pthread_mutex_init %d", descr);
		timer->state = TS_ERROR;
	}
#endif
	tlock(timer);
	sev = &timer->sigevent;
	
	sev->sigev_notify = SIGEV_THREAD;
	sev->sigev_notify_function = restorer_thread;
	sev->sigev_notify_attributes = NULL;
	sev->sigev_value.sival_ptr = timer;

	memcpy(timer->descr, descr, MAX_DESCR);
	timer->descr[MAX_DESCR-1] = '\0';
	
	if (timer_create( CLOCK_REALTIME ,
			  &timer->sigevent, &timer->timerid) == -1 ) {
		err_print( "timer_create" );
		t_err(timer);
		tunlock(timer);
		return NULL;
	}
	timer->state = TS_DISARMED;
	tunlock(timer);
		/* dbg_print("making %s timer", descr); */
	return timer;
}
int timer_destroy(struct timers *timer)
{
	char descr[MAX_DESCR];
	tlock(timer);
	dbg_print("destroying %s timer", descr);
	memcpy(descr,timer->descr, MAX_DESCR);
	if(timer->state & TS_ARMED) {
		wrn_print("Destroying armed timer, operation cancelled");
		tunlock(timer);
		return -1;				
	}
	timer->state = TS_NONE;
	if(timer_delete(timer->timerid) == -1) {
		err_print("timer_delete %s",timer->descr);
		t_err(timer);
		tunlock(timer);
		return -1;
	}
#ifdef PERTIMER_MUTEX
	pthread_mutex_t *p = &timer->mutex;	
	int e = pthread_mutex_unlock(p);
	if(e) 
		nerr_print( e,"pthread_mutex_unlock %s", descr);		
#endif

	free_timer(timer);


}

/* inter-time from now to the desired time 
 *
 * arm the timer using interval(inter) as default.
 */
int timer_arm(struct timers *timer, time_t inter, time_t corr)
{
	struct itimerspec ts;	
	
	tlock(timer);       

	timer->inter = inter;
	ts_h24(ts.it_interval);	
	
	ts.it_value.tv_sec = inter - corr;
	ts.it_value.tv_nsec = 0;

	dbg_print("arming %s inter %f; corr %f",timer->descr, inter, corr);
	
	if (timer_settime( timer->timerid, 0, &ts ,NULL) == -1 ) {
		err_print( "timer_settime" );
		t_err(timer);		
		tunlock(timer);
		return -1;
	}			       
	timer->state = TS_ARMED;
	tunlock(timer);
	return 0;
}
int timer_disarm(struct timers *timer)
{
	struct itimerspec ts;
	tlock(timer);
	dbg_print("disarming %s",timer->descr);
	ts_h0(ts.it_interval);
	ts_h0(ts.it_value);

	if (timer_settime( timer->timerid, 0, &ts ,NULL) == -1 ) {
		err_print( "timer_settime" );
		t_err(timer);		
		tunlock(timer);
		return -1;
		
	}
	timer->state = TS_DISARMED;

	tunlock(timer);
	return 0;
}
