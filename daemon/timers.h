#include <signal.h>
#include <time.h>
#include "generic.h"

//struct timers *timer_make(char *descr);
//int timer_destroy(struct timers *timer);

//int timer_arm(struct timers *timer, time_t inter, time_t corr);
//int timer_disarm(struct timers *timer);

int rm_timer_descr(char *descr);
int mk_timer_descr(char *descr, time_t inter);
int timer_mk_md(char *descr, time_t md_inter);
int rm_all_timers(void);

/* Need move it to some sensed place! */
int  mk_timers_set_rq(	struct timer_set_rq *s_rq ); 

void print_timers(void);
void print_timer(struct timers *p);
