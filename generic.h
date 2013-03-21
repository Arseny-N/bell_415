#include "head.h"
#include <unistd.h>
struct request {
	int type;
	void *stuff;
};
#define RT_ADD_TIMER 0x1
#define RT_DEL_TIMER 0x2

struct timer_add_rq {
	char descr[MAX_DESCR];
        time_t corr;
	time_t inter;
};
struct timer_del_rq {
	char descr[MAX_DESCR];
};

void *non_null="non_null";
struct timer_add_rq timer_add_rq = {
  .descr = {'t','r','0','\0'},
  .corr = 0,
  .inter = 4,
};
struct timer_del_rq timer_del_rq = {
  .descr = {'t','r','0','\0'},
};
struct request non_null_add_rq = {
  .type = RT_ADD_TIMER,
  .stuff = &timer_add_rq,
};
struct request non_null_del_rq = {
  .type = RT_DEL_TIMER,
  .stuff = &timer_del_rq,
};
int cnt = 0;
void *generic_read(void) { dbg_print("dummy");return non_null; }
struct request *decode_buf(void *p) 
{ 
	dbg_print("dummy"); 
	if(cnt)
		return &non_null_del_rq;
	return &non_null_add_rq; 
}
void destroy_rq ( struct request *rq ) 
{
	if(!cnt) {		
		cnt++;		
		timer_del_rq.descr[2] ++;
	} else {
		timer_add_rq.descr[2] ++;
		/* sleep(1); */
		cnt --;
	}
}
