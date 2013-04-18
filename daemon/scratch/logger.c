#include "head.h"
#include "log.h"
#include "error.h"

#include <ctype.h>

int main(int argc,char *argv[] )
{
	if(argv[1] == NULL) {
		fprintf(stderr,"Usage: %s string",argv[0]);
		exit(EXIT_FAILURE);
	}
	char *file = NULL;
	int ind = 1, numlines = 16, linesize = 16;

	while(argv[ind]) {
		switch(tolower(*argv[ind])) {
		case 'f':
			file = &argv[ind][1];
			break;
		case 'n':
			numlines = strtol(&argv[ind][1],NULL,0);
			break;
		case 'l':
			linesize = strtol(&argv[ind][1],NULL,0);
			break;
		default :
			wrn_print("Usage: %s {n|l|f} invalid %c",
				  argv[0], tolower(*argv[ind]));
			exit(EXIT_FAILURE);
		}
		ind += 1;
	}
	printf("Stop\n");
	if ( log_open(file,numlines,linesize) == -1 )
		wrn_print("log_open failed");

	char *buf = NULL;
	size_t n;
	
	while(getline(&buf, &n, stdin) != -1) {
		log_write(buf);
		
	}
	printf("EOF\n");
	return 0;
}
