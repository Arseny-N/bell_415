#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>



#include "error.h"
#include "timers.h"
#include "journal.h"
#include "head.h"
time_t _read_time(int fd)
{
	static time_t t;
   	if( read( fd, &t, sizeof(t) ) != sizeof(t))
		wrn_print("error while reading");	
	return t;
}
static long _descr_hash(char *s)
{
	long h = 0;
	while(*s) {
		h += *s;
		s++;
	}
	return h;
}
static long timer_hash(struct timers *timer)
{
	long h = 0;
	h += timer->state;	
	h += timer->md_inter;
	h += _descr_hash(timer->descr);
	return h;	
}
bool broken(struct timer_jentry *je)
{
	return je -> hash != timer_hash(&je->timer);
}
void print_timer(struct timers *p)
{
	printf("%s %s    %s    %lld \n",
	       (p->state == TS_NONE)?     "none    ":
	       (p->state == TS_ARMED)?    "armed   ":
	       (p->state == TS_DISARMED)? "disarmed":
	       "error   ",
	       p->state & TS_ERROR ? "y": "n", p->descr, 
	       (long long)p->md_inter);
}
void _journal_print(int fd)
{
	struct timer_jentry je;	
	
	while(read(fd, &je, sizeof(je)) == sizeof(je)) {
		
		if(broken (&je)) 
  			wrn_print("broken entry");				                print_timer(&je.timer);		
		
	}		
}

void read_jfile(char *name)
{
	int fd = open(name, O_RDONLY);
	if(fd == -1)
		err_print("open");
	time_t t = _read_time(fd);
	_journal_print(fd);

}
int main (int argc, char *argv[]) 
{
	if(argv[1] == NULL) {
		fprintf(stderr,"Usage: %s string",argv[0]);
		exit(EXIT_FAILURE);
	}
	int ind = 1;
	while(argv[ind]) {
		read_jfile(argv[ind]);
		++ind;
		
	}
	return 0;
}
