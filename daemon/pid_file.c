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


int read_pid_file(char *pidfile, pid_t *pid)
{
	char buf[BUF_SIZE];
	size_t numRead;
	int r = 0;	
	
	int fd = open ( pidfile, O_RDONLY);	
	if ( fd == -1 ) 
		return -1;
	 
	if ( !could_lock_file(fd, F_WRLCK) ) 
		r = -2; 
		
	numRead = read(fd, buf, BUF_SIZE);
	if(numRead == -1)
		return -1;
	
	buf[numRead+1] = '\0';
	
	*pid = (pid_t) atol(buf);
	return r;	
}
int kill_from_pid_file(char *pidfile, int sig)
{
	int ret;
	pid_t pid;
	
	if((ret = read_pid_file(pidfile, &pid)) < 0 )
		return ret;	
	return kill(pid, sig);
}
#endif
