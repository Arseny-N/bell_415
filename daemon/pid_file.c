#ifndef _PID_FILE_C
#define _PID_FILE_C

#define KILL_FROM_PID_ERRORS(call) switch ( call ) { \
			case -1: \
				err_print("kill_from_pid_file"); \
				terminate(); \
			case -2: \
				wrn_print("No deamon is currentlly running"); \
				terminate(); \
		} 

#define PID_PERMS  0664

//S_IRUSR | S_IWUSR

/* Thanks to Mikael Kerrisk */
static int lockReg ( int fd, int cmd, int type, int whence, 
		     int start, off_t len )
{
	struct flock fl;
	
	fl.l_type = type;
	fl.l_whence = whence;
	fl.l_start = start;
	fl.l_len = len;
	
	return fcntl ( fd, cmd, &fl );
}
static bool _could_lock_file (int fd, int type, int whence, int start, off_t len)
{
	struct flock fl;
	
	fl.l_type = type;
	fl.l_whence = whence;
	fl.l_start = start;
	fl.l_len = len;
	
	if( fcntl(fd, F_GETLK, &fl) == -1) 
		return -1;
	return fl.l_type == F_UNLCK;
}


static inline int lock_region ( int fd, int type, int whence, int start, off_t len )
{
	return lockReg ( fd, F_SETLK, type, whence, start, len );
}
static inline int lock_file( int fd, int l_type )
{
	return lock_region(fd, l_type, SEEK_SET,0, 0);
}
static inline bool could_lock_file( int fd, int type)
{
	return _could_lock_file(fd, type, SEEK_SET, 0, 0);
}




int create_pid_file (const char * pidFile)
{
	char buf [BUF_SIZE];
  
	int fd = open ( pidFile, O_RDWR | O_CREAT 
#ifdef O_CLOEXEC
			| O_CLOEXEC
#endif			
			,PID_PERMS );
	if ( fd == -1 ) {
		err_print("open");
		return -1;
	} 
	
#ifndef O_CLOEXEC
	if (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) { 
		err_print("fcntl");
		return -1;
	}
#endif
	if ( lock_file ( fd, F_WRLCK ) == -1 ) {
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

pid_t read_pid_file(char *pidfile)
{
	char buf[BUF_SIZE];
	size_t numRead;
	pid_t pid;
	
	int fd = open ( pidfile, O_RDONLY);
	if ( fd == -1 ) 
		return -1;
	 
	if ( could_lock_file(fd, F_WRLCK) ) 
		return -2;
		
	numRead = read(fd, buf, BUF_SIZE);
	if(numRead == -1)
		return -1;
	
	buf[numRead+1] = '\0';
	
	pid = (pid_t) atol(buf);
	return pid;	
}
int kill_from_pid_file(char *pidfile, int sig)
{
	pid_t pid = read_pid_file(pidfile);
	if(pid < 0)
		return pid;	
	return kill(pid, sig);
}
#endif
