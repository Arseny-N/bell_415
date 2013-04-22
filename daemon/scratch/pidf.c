#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define BUF_SIZE 256
#define err_print(m) perror(m)
static inline int lockReg ( int fd, int cmd, int type, int whence, 
		     int start, off_t len )
{
	struct flock fl;
	
	fl.l_type = type;
	fl.l_whence = whence;
	fl.l_start = start;
	fl.l_len = len;
	
	return fcntl ( fd, cmd, &fl );
}


static int lock_region ( int fd, int type, int whence, int start, off_t len )
{
	return lockReg ( fd, F_SETLK, type, whence, start, len );
}			  
int create_pid_file (const char * pidFile)
{
	char buf [BUF_SIZE];
  
	int fd = open ( pidFile, O_RDWR | O_CREAT | O_CLOEXEC, 
			S_IRUSR | S_IWUSR );
	if ( fd == -1 ) {
		err_print("open");
		return -1;
	} 
		
	if ( lock_region ( fd, F_WRLCK, SEEK_SET, 0, 0 ) == -1 ) {
		if ( errno == EAGAIN || errno == EACCES )
			return -2;		
		else {
		        err_print("lock_region");
			return -1;		
		}
	}
	if ( ftruncate ( fd, 0 ) == -1 ) {
		err_print("ftrucate");
		return -1;
	}
	snprintf ( buf, BUF_SIZE, "%ld\n", (long) getpid () );
	if ( write ( fd, buf, strlen (buf) ) != strlen (buf) ) {
		err_print("write");
		return -1;
	}
  
	return fd;
}
int main ( int argc, char * argv [] )
{
	if(!argv[1] || !argv[2]) {
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
	printf("Slepping %s secs\n",argv[2]);
	sleep (atol(argv[2]));
	return 0;

}
