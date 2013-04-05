#include <sys/types.h>
#include <string.h>

#include "head.h"
#include "timers.h"
#include "threads.h"
#include "error.h"
#include "inter.h"

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

/* Low ord Funcs */
inline static void __append_to_list(struct timers *timer)
{		
	if(last == NULL) {
		last = timer;
		return;
	}
	
	last->next = timer;
	last = last->next;
	timer->next = NULL;
	/* last -> next = NULL; */		
}
inline static void __remove_from_list(struct timers *timer)
{	
	if(timers == timer) {
		timers = timers->next;		
		if(timer == last) 
			last = NULL;		
		return;		
	}
	
	struct timers *p;	
	p = timers;		

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
void print_timer(struct timers *p)
{
	printf("%s %s    %s    %lld    %p\n",
	       (p->state == TS_NONE)?     "none    ":
	       (p->state == TS_ARMED)?    "armed   ":
	       (p->state == TS_DISARMED)? "disarmed":
	       "error   ",
	       p->state & TS_ERROR ? "y": "n", p->descr, 
	       (long long)p->md_inter, p->next);
}
void print_timers(void) 
{
	struct timers *p;
	p = timers;
	printf("state   err  descr md_inter next\n");

	while(p) {
		print_timer(p);
		p = p->next;
	}
}
/* Linked List Funcs */
static inline struct timers *__timer_find(char *descr)
{	
	struct timers *p;
	p = timers;
	if(!descr) {
		wrn_print("NULL description ?!");
		return NULL;
	}
	while(p != NULL) {		
		/* dbg_print("(%s) (%s)",descr, p->descr); */
		if( p->descr )
			if( strncmp(p->descr, descr, MAX_DESCR) == 0) 	       
				return p;
		p = p->next;
	}
	//dbg_print("Not found"); 
	return NULL;
}
struct timers *timer_find(char *descr)
{
	struct timers *timer;
	
	timer = __timer_find(descr);
	
	return timer;
}
struct timers *get_free_timer(void)
{
	struct timers *timer;
	if ( timers == NULL ) {
		timers = malloc( sizeof(struct timers) );
		if ( timers == NULL )
			return NULL;
		timer = timers;
	} else {
		timer = malloc( sizeof(struct timers) );
		if ( timer == NULL )
			return NULL;	
		
	}
	timer->next = NULL;
	__append_to_list (timer);
	timer->state = TS_NONE;	

	return timer;
}
static void free_timer(struct timers *timer)
{
	
	if(timer->state != TS_NONE) {
		wrn_print("freeng in use timer, operation refused");
		return;
	}

	__remove_from_list (timer);

	free (timer);		
}
/* End List Functions */



void __sigev_init(struct sigevent *sev)
{       
 	sev->sigev_notify = SIGEV_THREAD;
	sev->sigev_notify_function = restorer_thread;
	sev->sigev_notify_attributes = NULL;  
}
struct timers *__timer_make(char *descr, struct timers *timer, time_t md_inter)
{
	__sigev_init(&timer->sigevent);
	
	memcpy(timer->descr, descr, MAX_DESCR);
	timer->descr[MAX_DESCR-1] = '\0';
	
	if (timer_create( CLOCK_REALTIME ,
			  &timer->sigevent, &timer->timerid) == -1 ) {
		err_print( "timer_create" );
		t_err(timer);
		return NULL;
	}
	timer->state = TS_DISARMED;
	timer->md_inter = md_inter;
	dbg_print("making %s (id %ld) timer", descr,(long)timer->timerid); 
	return timer;	
}
struct timers *timer_make(char *descr,time_t md_inter)
{
	struct timers *timer;

	timer = get_free_timer();  
	if(timer == NULL || is_err(timer)) {
		wrn_print( "get_free_timer %s",descr);
		return NULL;
	}
	return __timer_make(descr,timer,md_inter);

}
int timer_destroy(struct timers *timer)
{
	char descr[MAX_DESCR];

	dbg_print("destroying %s %ld timer", timer->descr,(long)timer->timerid);
	
	memcpy(descr,timer->descr, MAX_DESCR);

	if(timer->state & TS_ARMED) {
		wrn_print("Destroying armed timer, operation cancelled");
		return -1;				
	}
	timer->state = TS_NONE;
	if(timer_delete(timer->timerid) == -1) {
		err_print("timer_delete %s (%ld)",descr,(long)timer->timerid);
		t_err(timer);
		return -1;
	}


	free_timer(timer);

	return 0;

}


int timer_disarm(struct timers *timer)
{
	struct itimerspec ts;
	if(timer->state != TS_ARMED) {
		wrn_print("Disarming non armed timer %s", timer->descr);
		return -1;
	}
	dbg_print("disarming \"%s\" %ld",timer->descr,(long)timer->timerid);
	ts_h0(ts.it_interval);
	ts_h0(ts.it_value);

	if (timer_settime( timer->timerid, 0, &ts ,NULL) == -1 ) {
		err_print( "timer_settime %s %ld %ld %ld %ld", timer->descr, (long)ts.it_interval.tv_sec, (long)ts.it_interval.tv_nsec,
			  (long) ts.it_value.tv_sec,(long)ts.it_value.tv_nsec);
		t_err(timer);		
		return -1;
		
	}
	timer->state = TS_DISARMED;


	return 0;
}

/* inter-time from now to the desired time 
 *
 * arm the timer using interval(inter) as default.
 */
int timer_arm(struct timers *timer, time_t inter)
{
	struct itimerspec ts;	
	
	if ( inter <= (time_t) 0 ) {
		wrn_print ("Couldnt set zero or less to timer (%s to %lld)",timer->descr,(long long) inter);
		return -1;
	}
	
	ts_h24(ts.it_interval);	

		
	ts.it_value.tv_sec = inter;
	ts.it_value.tv_nsec = 0;

	dbg_print("arming %s %ld inter %ld",timer->descr, (long)timer->timerid,(long)inter);
	
	if (timer_settime( timer->timerid, 0, &ts ,NULL) == -1 ) {

		err_print( "timer_settime %s %ld", timer->descr, (long)ts.it_value.tv_sec );
		t_err(timer);		
		return -1;
	}			       
	timer->state = TS_ARMED;

	return 0;
}

int mk_timer(char *descr, time_t inter,time_t md_inter)
{
	struct timers *timer;
	timer = timer_find(descr);
	if( timer != NULL) {
		wrn_print("timer %s already present", descr);
		return -1;
	}
	timer = timer_make(descr,md_inter);		
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
int timer_mk_md(char *descr, time_t md_inter)
{
	time_t inter = get_inter(md_inter);
	
	return mk_timer(descr,inter,md_inter);
}

static int _rm_timer(struct timers *timer)
{


	//dbg_print("removing %s", timer->descr);

	if ( timer_disarm (timer) == -1 ) {
		wrn_print("timer_disarm %s",  timer->descr);		
	}

	if ( timer_destroy (timer) == -1 ) {
		wrn_print("timer_destroy %s", timer->descr);
	}		

	return 0;
}

int rm_all_timers(void) 
{
	struct timers *next,*p = timers;
		
        while(p) {
		
		
		next = p->next;

		if(_rm_timer(p) == -1)
			return -1;
		p = next;
	} 
	return 0;
}

int mk_timers_set_rq(struct timer_set_rq *s_rq )
{
	int i, num = s_rq -> num;

	for(i = 0; i < num; ++i) 
		if (timer_mk_md(s_rq->sub[i]->descr,s_rq->sub[i]->md_inter) == -1) 
			return -1;						
	return 0;
}


int rm_timer_descr(char *descr)
{
	struct timers *timer;		
	dbg_print("deliting %s",descr);
	timer = timer_find(descr);		
				
	if( timer == NULL ) {
		wrn_print("no such timer %s",descr);
		return -1;
	}
	return 	_rm_timer(timer);

}
