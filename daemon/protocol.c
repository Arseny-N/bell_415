#include <string.h>
#include <ctype.h>
#include <time.h>

#include "head.h"
#include "protocol.h"
#include "error.h"
#include "timers.h"
#include "inter.h"
static inline int char_occ(char *p, char c)
{
	int count = 0;
	while(*p) {
		if ( *p == c )
			count++;
		p++;
	}
	return count;
}

static inline int count_set_rqs(char *p)
{
	return char_occ(p,' ');
}


static time_t _str_to_time(char *p)
{

	struct tm tm;
	time_t t;
	if(strptime(p,"%H:%M:%S",&tm) == -1) {
		err_print("strptime");
		return -1;
	}	
	__zero_out_unneed(&tm);

	t = mktime(&tm);

	dbg_print("cell: %ld %ld %s", (long)t,(long)timezone,ctime(&t));
	if( t == -1 ) {
		err_print("mktime()");
		return -1;
	}

	return t;
	
}


static int get_set_cell(char *p, char **dest)
{
	int num = 0;
	char *pp;

	while(isspace(*p)) {
		++num;
		++p;
	}

	pp = strchr(p,' ');
	if(pp != NULL)		
		*pp = 0; 

	num += strlen(p)+1;

	strncpy(*dest, p, MAX_DESCR);
	(*dest)[BUF_SIZE-1] = 0;
	return num;
	
	
}

static int decode_set_descr(char *cell, char **dest)
{

	int i, size = min(strlen(cell),MAX_DESCR);
	for(i = 0; i < size ; i++) {
		(*dest)[i] = cell[i];
		if((*dest)[i]=='.')
			break;		
	}
	if((*dest)[i] != '.') 
		return -1;


        (*dest)[i] = '\0';
	return 0;
}


static int decode_set_md_inter(char *cell,  time_t *md_dest)
{
	char *p = strchr(cell, '.');	
	if ( p == NULL )
		return -1;
	p ++;
	*md_dest = _str_to_time(p);
//dbg_print("%d - %s", *md_dest, ctime(md_dest));
	return *md_dest == -1 ? -1 : 0;
}

static inline int _get_sub_set(struct timer_set_sub *rq ,char *cell)
{
	char *buf = malloc(MAX_DESCR);
	if ( decode_set_descr(cell,&buf) == -1 )
		return -1;

	if( strncpy(rq->descr, buf, MAX_DESCR) == NULL ) {
		err_print("strncpy()");
	}		
		
	if (decode_set_md_inter(cell,&rq->md_inter) == -1 )
		return -1;
	return 0;
		
}
static int get_set_rqs(char *p, struct timer_set_rq **rq)
{      
	int num = count_set_rqs(p);
	char *cell = malloc(BUF_SIZE);


	*rq = malloc (sizeof(struct timer_set_rq));
	(*rq)->sub = calloc( num , sizeof(struct timer_set_sub*));
	if((*rq)->sub == NULL) 
		err_print("malloc");

	
	int cnt;
	for( cnt = 0; cnt < num; cnt++ ) { 
		(*rq)->sub[cnt] = malloc( sizeof(struct timer_set_sub));
		
		
		p += get_set_cell(p,&cell);
		
		if (_get_sub_set((*rq)->sub[cnt], cell) == -1) {
			return -1;
		}		

	}
	(*rq)->num = num;
	
	return 0;
}
static int free_set_rq(struct request *rq)
{
	struct timer_set_rq *set = rq->stuff;
	int i, num = set -> num;

	for(i = 0; i < num; ++i) 
		free(set->sub[i]);
	free(rq->stuff);
	return 0;
}
int get_rq_stuff( char *p, struct request *rq )
{
	
	switch(rq->type) {
	case RT_SET_TIMERS:
	  return get_set_rqs(p, (struct timer_set_rq **)&rq->stuff);
	}
	return -1;
}

int get_rq_type( char *p )
{
	switch(tolower(*p)) {
	case 's': return RT_SET_TIMERS;
	default:  return -1;
	}
	return 0;
}
int free_rq(struct request *rq)
{
	switch(rq->type) {
	case RT_SET_TIMERS:
	  return free_set_rq(rq);
	}
	return -1;
}
