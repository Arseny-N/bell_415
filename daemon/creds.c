#include "creds.h"
#include "error.h"
#include "caps.h"

#include <pwd.h>
#include <grp.h>

#define IS_PWD_SEARCH_ERROR(e) (e != EINTR && e != EIO && EMFILE && e != ENFILE && e != ENOMEM && e != ERANGE)
#define IS_GRP_SEARCH_ERROR(e) IS_PWD_SEARCH_ERROR(e)

static inline uid_t nametouid(char *name)
{
	struct passwd *pwd;	
	pwd = getpwnam(name);
	if(pwd == NULL) {
		if(!IS_GRP_SEARCH_ERROR(errno)) {
			err_print("pwd");
			return -1;
		}
		wrn_print("no %s user found",name);
		return -1;
	}		
	return pwd->pw_uid;
}
static inline gid_t nametogid(char *name)
{
	struct group *grp;	
	grp = getgrnam(name);
	if(grp == NULL) {
		if(!IS_PWD_SEARCH_ERROR(errno)) {
			err_print("pwd");
			return -1;
		}
		wrn_print("no %s group found",name);
		return -1;
	}		
	return grp->gr_gid;
}
static inline int _check_user(struct cmd_creds *cds)
{	
	uid_t uid = getuid();
	if(uid == -1) {
		err_print("getuid");
		return -1;
	}

	if(cds->uid == uid) 
		return 0;
	
	if(cds->user) {
		cds->uid = nametouid(cds->user);
		if(cds->uid == -1)
			return -1;
	}		
	
	if(cds->uid == uid) 
		return 0;
	
	if(raise_cap(CAP_SETUID) == -1) {
		err_print("raise_cap");
		return -1;
	}
	if(setuid(cds->uid) == -1) {
		err_print("setuid");
		return -1;
	}
	if(drop_cap(CAP_SETUID) == -1) {
		err_print("drpo_cap");
		return -1;
	}
	return 0;
	
}
static inline int _check_group(struct cmd_creds *cds)
{
	gid_t gid = getgid();
	if(gid == -1) {
		err_print("getgid");
		return -1;
	}
	
	if(cds->gid == gid)
		return 0;
	if(cds->group) {
		cds->gid = nametogid(cds->user);
		if(cds->gid == -1)
			return -1;
	}
	
	if(cds->gid == gid)
		return 0;	
	if(raise_cap(CAP_SETGID) == -1) {
		err_print("raise_cap");
		return -1;
	}
	if(setgid(cds->gid) == -1) {
		err_print("setuid");
		return -1;
	}
	if(drop_cap(CAP_SETGID) == -1) {
		err_print("drpo_cap");
		return -1;
	}
	return 0;
	
}
int check_creds(struct cmd_creds *cds)
{
	if( cds->uid == -1 && !cds->user ) {
		wrn_print("no suitable user id or name specified");
		return -1;
	}
	if( cds->gid == -1 && !cds->group ) {
		wrn_print("no suitable group id or name specified");
		return -1;
	}	
	
	if(_check_user(cds) == -1) {
		return -1;
	}
	if(_check_group(cds) == -1) {
		return -1;
	}	
	return 0;
	
}
