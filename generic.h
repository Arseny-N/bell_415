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
#include <ctype.h>
char rand_char(void)
{
	char c;
	do {		
		 c = random();
	} while ( !isalnum(c) );
	return c;	
}
#define is_good_inter(t) ( t > 0 && t <= 10 )
time_t rand_inter(void)
{
	time_t t;
	do {
		t = random();
	} while ( !is_good_inter(t) );
	return t;
}

bool cnt = 0;
void *void_read(void) 
{
	return non_null; 
}

struct request *decode_buf(void *p) 
{ 
	if(cnt)
		return &non_null_del_rq;
	return &non_null_add_rq; 
}
void destroy_rq ( struct request *rq ) 
{
	if(!cnt) {		
		cnt = 1;		
		timer_del_rq.descr[2] = rand_char();
	} else {
		timer_add_rq.descr[2] = rand_char();
		timer_add_rq.inter = rand_inter();
		/* sleep(rand_inter());   */
		cnt = 0;
	}
}
struct method {
	void *(*read) (void);                               /* Read a non NULL buf  */
	struct request *(*decode)(void *p);                 /* Make from this buf a rq structure */
	void (*destroy) ( struct request *rq );             /* Destroy this structure */

} method = { 
	.read = void_read,
	.destroy = destroy_rq,
	.decode = decode_buf,
};

