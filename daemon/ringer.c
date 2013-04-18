#include <stdio.h>
#include <stdlib.h>

#include "head.h"
#include <time.h>

char *str_brtime(const char *fmt, struct tm *tm)
{
	static char buf[BUF_SIZE];
	if( strftime( buf, BUF_SIZE, (fmt != NULL) ? fmt : "%c %Z", tm) == 0) {
		perror("gmtime_r()");
		return NULL;
	}
	return buf;
}
char *str_time(const char *fmt, time_t *t)
{
        struct tm tm;	
	if( localtime_r( t, &tm) == NULL ) {
		perror("gmtime_r()");
		return NULL;
	}	       
	return str_brtime(fmt, &tm);
}
char * curr_time (const char *fmt)
{
	time_t t = time (NULL);	       	
	return str_time(fmt, &t);
}


int main (int argc, char *argv[])
{
	FILE *out = fopen("/home/arseni/Documents/p/bell/daemon/ringed.list","a");
	if(!out) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	fprintf(out,"%s %s\n",argv[1],curr_time("%T"));
	if(fclose(out) == -1) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}	
	return 0;

}
