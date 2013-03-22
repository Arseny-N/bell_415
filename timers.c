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
	last -> next = timer;
	last = last -> next;
	/* last -> next = NULL; */		
}
inline static void __remove_from_list(struct timers *timer)
{	
	struct timers *p;
	

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
		if( p->descr && descr )
			if( strncmp(p->descr, descr, MAX_DESCR) == 0) 	       
				return p;
		p = p->next;
	}
	dbg_print("Not found"); 
	return NULL;
}
struct timers *timer_find(char *descr)
{
	struct timers *timer;
	
	timer = __timer_find(descr);
	
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
		memset(timer->descr,0,MAX_DESCR);
		timer->timerid = 0;
		
		return;
	}
	__remove_from_list (timer);
	free (timer);		
}
struct timers *timer_make(char *descr)
{
	struct timers *timer;
	struct sigevent *sev;


	timer = get_free_timer();  
	if(timer == NULL || is_err(timer)) {
		wrn_print( "get_free_timer %s",descr);
		return NULL;
	}

	sev = &timer->sigevent;
	
	sev->sigev_notify = SIGEV_THREAD;
	sev->sigev_notify_function = restorer_thread;
	sev->sigev_notify_attributes = NULL;
	/* sev->sigev_value.sival_ptr = timer; 
	 * then need mutexes
	 */

	memcpy(timer->descr, descr, MAX_DESCR);
	timer->descr[MAX_DESCR-1] = '\0';
	
	if (timer_create( CLOCK_REALTIME ,
			  &timer->sigevent, &timer->timerid) == -1 ) {
		err_print( "timer_create" );
		t_err(timer);
		return NULL;
	}
	timer->state = TS_DISARMED;

	dbg_print("making %s %ld timer", descr,timer->timerid); 
	return timer;
}
int timer_destroy(struct timers *timer)
{
	char descr[MAX_DESCR];

	dbg_print("destroying %s %ld timer", descr,timer->timerid);
	memcpy(descr,timer->descr, MAX_DESCR);
	if(timer->state & TS_ARMED) {
		wrn_print("Destroying armed timer, operation cancelled");
		return -1;				
	}
	timer->state = TS_NONE;
	if(timer_delete(timer->timerid) == -1) {
		err_print("timer_delete %s",timer->descr);
		t_err(timer);
		return -1;
	}


	free_timer(timer);


}

/* inter-time from now to the desired time 
 *
 * arm the timer using interval(inter) as default.
 */
int timer_arm(struct timers *timer, time_t inter)
{
	struct itimerspec ts;	
	
	if ( inter <= (time_t) 0 ) {
		wrn_print ("Couldnt set zero or less to timer");
		return -1;
	}

	timer->inter = inter;
	ts_h24(ts.it_interval);	

		
	ts.it_value.tv_sec = inter;
	ts.it_value.tv_nsec = 0;

	dbg_print("arming %s %ld inter %d",timer->descr, timer->timerid,inter);
	
	if (timer_settime( timer->timerid, 0, &ts ,NULL) == -1 ) {

		err_print( "timer_settime %s %g", timer->descr, ts.it_value.tv_sec );
		t_err(timer);		
		return -1;
	}			       
	timer->state = TS_ARMED;

	return 0;
}
int timer_disarm(struct timers *timer)
{
	struct itimerspec ts;

	dbg_print("disarming %s %d",timer->descr,timer->timerid);
	ts_h0(ts.it_interval);
	ts_h0(ts.it_value);

	if (timer_settime( timer->timerid, 0, &ts ,NULL) == -1 ) {
		err_print( "timer_settime %s %d %d %d %d", timer->descr, ts.it_interval.tv_sec, ts.it_interval.tv_nsec,
			   ts.it_value.tv_sec,ts.it_value.tv_nsec);
		t_err(timer);		
		return -1;
		
	}
	timer->state = TS_DISARMED;


	return 0;
}

int mk_timer(char *descr, time_t inter)
{
	struct timers *timer;
	timer = timer_find(descr);
	if( timer != NULL) {
		wrn_print("timer %s already present", descr);
		return -1;
	}
	timer = timer_make(descr);		
	if( timer == NULL ) {
		wrn_print("timer_create %s",descr);
		return -1;
	}

	if ( timer_arm ( timer, inter ) == -1 ) {
		wrn_print("timer_arm %s", descr);
		timer_destroy (timer);
		return -1;
	}
	return 0;
}
int rm_timer(char *descr)
{
	struct timers *timer;		
	dbg_print("deliting %s",descr);
	timer = timer_find(descr);		
				
	if( timer == NULL ) {
		wrn_print("no such timer %s",descr);
		return -1;
	}
	dbg_print("disarming %s", timer->descr);
	if ( timer_disarm (timer) == -1 ) {
		wrn_print("timer_disarm %s", descr);
		return -1;
	}
	
	if ( timer_destroy (timer) == -1 ) {
		wrn_print("timer_destroy %s",descr);
		return -1;
	}		
	return 0;
}
