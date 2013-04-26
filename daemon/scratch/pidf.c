#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 256
#define err_print(m) perror(m)
#include "../pid_file.c" 




int main ( int argc, char * argv [] )
{
	if(!argv[1]) {
		printf("Wrong cmd\n");
		return 1;
	}
	switch( create_pid_file(argv[1])) {
	case -1:
		perror("mk_pid_file");
		return 1;
	case -2:
		printf("An instance of %s is already running\n",argv[0]);
		return 1;
	}
	
	pause();	
	return 0;

}
