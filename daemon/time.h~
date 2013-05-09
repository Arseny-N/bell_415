#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "cmd.h"
#include "head.h"
char *str_time(char *fmt, time_t *t);
time_t get_inter(time_t md);
char * curr_time (const char *format);
time_t str_to_time(const char *p, bool abs); /* FIXIT: Relies on glibc  */
char *ts_to_string(struct timespec *ts);
struct tm *str_to_tm(const char *p,bool abs);

static inline char *curr_dayn(void)
{
	return curr_time("%u");
}
static inline char *curr_date(void)
{
	return curr_time("%Y-%d-%m");
}

static inline int fit_default_durations(char *time_str)
{
	if(strcmp(time_str, cmd.short_ring) == 0)
		return SIG_RING_SHORT;
	if(strcmp(time_str, cmd.long_ring) == 0)
		return SIG_RING_LONG;
	return -1;
}
static inline char *get_duration(siginfo_t *info)
{
	static char buf[BUF_SIZE];
	snprintf(buf,BUF_SIZE,"%ld", (long)info->si_int);
	return buf;
}
