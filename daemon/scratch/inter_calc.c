//#define _XOPEN_SOURCE

#include <ctype.h>
#include <time.h>
#include "head.h"
#include "error.h"
#include "inter.h"

#define print_t(pref, t)						\
	({ typeof(t) __t = t;						\
		printf(pref"%s (%lld)\n",_stime(&__t), (long long) __t); })
const char *format = "%H:%M:%S %d %m %Y";
static time_t time_str_decode(char* ts)
{
	struct tm tm;
	if(strptime(ts,format,&tm) == -1) {
		err_print("strptime %s",ts);
	}       
	return mktime(&tm);
}
int main(int argc, char * argv[])
{
	if(argv[1] == NULL) {
                wrn_print("Usage: %s {c|i}%s",argv[0],format);
		exit(EXIT_FAILURE);
	}

	int ind = 1;
	time_t inter_md, curr;
	time(&curr);

	while(argv[ind]) {
		switch(tolower(*argv[ind])) {
		case 'c':
			curr = time_str_decode(&argv[ind][1]);
			curr = _only_hours(curr);
			break;
		case 'i':
			inter_md = time_str_decode(&argv[ind][1]);	       
			inter_md = _only_hours(inter_md);
			break;
		default :
			wrn_print("Usage: %s {c|i}time_string invalid %c",
				  argv[0], tolower(*argv[ind]));
			continue;			
		}
		ind += 1;
	}

	time_t inter = _get_inter(inter_md, curr);
	time_t zero = (time_t) 0;
	time_t mn = __get_midnight(curr);

        printf( "\n");
	
	print_t("zero    :", zero);
	printf( "\n");
	
	print_t("md_inter:", inter_md);
	print_t("curr    :", curr);	
	printf( "\n");

	if(curr > inter_md)  
		curr += h24; 

	
	print_t("curr-md :", (inter_md - curr));
	print_t("timezone:", (timezone));

		
		
	return 0;
}
