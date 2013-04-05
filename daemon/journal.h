
#ifndef _JOURNAL_H
#define _JOURNAL_H

#include "head.h"
int journal_open(int n, char **fv);
int journal_close(void);

void journal_read(char *name);
void journal_write(void);

time_t _read_time(int fd);

/*!! Buggy code !!*/
#define for_each_jentry(p) for(p = j_list->next; p != j_list; p = p->next)
extern struct journal_list *j_list;

/* In memory rapresentation */
struct journal_list {
	int fd;
	char *name;
	struct journal_list * next;
};
/* On file rapresentation */
struct timer_jentry {
	long hash;	
	struct timers timer;
};

bool broken(struct timer_jentry *je);
#endif
