#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "caps.h"


static inline bool not_root(void)
{
	return getuid() != 0;
}

static int modify_cap(int capability, int setting)
{
    cap_t caps;
    cap_value_t capList[1];

    caps = cap_get_proc();
    if (caps == NULL)
        return -1;

    capList[0] = capability;
    if (cap_set_flag(caps, CAP_EFFECTIVE, 1, capList, setting) == -1) {
        cap_free(caps);
        return -1;
    }
    if (cap_set_proc(caps) == -1) {
        cap_free(caps);
        return -1;
    }

    if (cap_free(caps) == -1)
        return -1;

    return 0;
}
void print_caps(char *prefix, cap_t caps)
{
	char *p = cap_to_text(caps, NULL);
	if(!p) {
		cap_free(p);
		return;
	}
	if(prefix)
		printf("%s",prefix);
	printf("\t%s",p);
	cap_free(p);				
}
void print_proc_caps(char *prefix)
{
	cap_t caps = cap_get_proc();
	if (!caps) {
		cap_free(caps);
		return;
	}
	print_caps(prefix, caps);
}
int raise_cap(int cap)
{
    return not_root() ? modify_cap(cap, CAP_SET) : 0;
}

int drop_cap(int cap) 
{
	return not_root() ? modify_cap(cap, CAP_CLEAR) : 0;
}

int drop_all_caps(void)
{
    cap_t empty;
    int s;

    empty = cap_init();
    if (empty == NULL)
        return -1;

    s = cap_set_proc(empty);

    if (cap_free(empty) == -1)
        return -1;

    return s;
}

