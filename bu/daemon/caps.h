#include <sys/capability.h>
#include <sys/types.h>

#ifndef __CAP_H
#define __CAP_H

int raise_cap(int cap);
int drop_cap(int cap);

void print_proc_caps(char *prefix);
void print_caps(char *prefix, cap_t caps);

int drop_all_caps(void); /* All caps fom all sets! */

#endif
