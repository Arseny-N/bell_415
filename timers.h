#include <signal.h>
#include <time.h>


//struct timers *timer_make(char *descr);
//int timer_destroy(struct timers *timer);

//int timer_arm(struct timers *timer, time_t inter, time_t corr);
//int timer_disarm(struct timers *timer);

int rm_timer(char *descr);
int mk_timer(char *descr, time_t inter);

void print_timers(void);
//struct timers *timer_find(char *descr);
