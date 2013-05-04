#ifndef __CREDS_H
#define __CREDS_H

#include <sys/types.h>

struct cmd_creds {

	uid_t uid;
	char *user;
		
	gid_t gid;
	char *group;
};


int check_creds(struct cmd_creds *cds);

#endif
