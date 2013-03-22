#include "head.h"
#include <unistd.h>
/* Acid Test */
struct request {
	int type;
	void *stuff;
};
#define RT_ADD_TIMER 0x1
#define RT_DEL_TIMER 0x2

struct timer_add_rq {
	char descr[MAX_DESCR]; 
	time_t inter;
};
struct timer_del_rq {
	char descr[MAX_DESCR];
};

static struct timer_add_rq timer_add_rq = {
  .descr = {'t','r','0','\0'},
  .inter = 4,
};
static struct timer_del_rq timer_del_rq = {
  .descr = {'t','r','0','\0'},
};
static struct request add_rq = {
  .type = RT_ADD_TIMER,
  .stuff = &timer_add_rq,
};
static struct request del_rq = {
  .type = RT_DEL_TIMER,
  .stuff = &timer_del_rq,
};
#include <ctype.h>
#define SLOW_RANDOM_
char rand_char(void)
{
	char c = '0';
#ifdef SLOW_RANDOM_
	do {		
		c = random();
	} while ( !isalnum(c) );
#endif
	return c;	
}
#define is_good_inter(t) ( t > 0 && t <= 10 )
time_t rand_inter(void)
{
	time_t t = 4;
#ifdef SLOW_RANDOM_
	t = (char) random();
	do {
	t +=  random();
} while ( !is_good_inter((char)t) );
#endif
	return (char)t;
}

static bool cnt = 0;
void *void_read(void) 
{
	void *on_null="+non_null+";
	return on_null; 
}

struct request *decode_buf(void *p) 
{ 	
	if(cnt)
		return &del_rq;
	return &add_rq; 
}
void destroy_rq ( struct request *rq ) 
{
	if(!cnt) {		
		cnt = 1;		
		timer_del_rq.descr[2] = rand_char();
	} else {
		timer_add_rq.descr[2] = rand_char();
		timer_add_rq.inter = rand_inter();	
		sleep(4);
		cnt = 0;
	}
}
void dummy(void)
{
	return;
}
struct method {
	void *(*read) (void);                               /* Read a non NULL buf  */
	struct request *(*decode)(void *p);                 /* Make from this buf a rq structure */
	void (*destroy) ( struct request *rq );             /* Destroy this structure */
	void (*init) (void);                                /* Called on init */
	void (*on_exit) (void);                             /* Called on exit */
} method = { 
	.read = void_read,
	.destroy = destroy_rq,
	.decode = decode_buf,

	.on_exit = dummy,
	.init = dummy,
};

