#include "bell_ring.h"
#include <time.h>
void ring_the_bell(void)
{
	time_t t;
	time(&t);
	printf("%s din don :-)\n", ctime(&t)); 
	return;
}
