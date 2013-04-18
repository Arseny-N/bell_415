#include <time.h>
#include "head.h"
#include "protocol.h"

static void print_sub(struct timer_set_sub *s)
{
	printf("\tSub:\n");
	printf("\t\t\tdescr: %s\n",s->descr);
	printf("\t\t\tmd_inter: %ld %s\n",s->md_inter,ctime(&s->md_inter));
}
int main (int argc, char *argv[]) 
{
	if(argv[1] == NULL) {
		fprintf(stderr,"Usage: %s string",argv[0]);
		exit(EXIT_FAILURE);
	}
	
	struct request rq;
	struct timer_set_rq * set;

	rq.type = get_rq_type(argv[1]);	
	skip_rq_type(argv[1]);


	get_rq_stuff(argv[1], &rq);
	
	set = rq.stuff;
	printf("Request:\n");
	printf("\tnum: %d\n",set->num);

	int i;
	for ( i = 0; i < set->num; i++ ) {
		print_sub(set->sub[i]);
	}
	free_rq(&rq);
	return 0;


	
}
