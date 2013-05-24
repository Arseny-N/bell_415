#!/bin/bash

max_err=256
echo "#define MAX_ERR $max_err"

cat <<EOF 

char *ename[MAX_ERR];
static bool __was_already_inited = 0;

static inline void __ename_init(void) 
{
	if(__was_already_inited) 
		return;
	__was_already_inited = 1;
        memset(ename, 0, sizeof(ename));
        ename[0] = "NO_SUCH_ERR";
EOF

cat /usr/include/asm-generic/errno{-base,}.h | \
    grep -Ev "(\#include|\#if|\#endif)" | grep -E [0-9] | awk '{print "        ename[" $2 "] = \"" $2 "\"; /* " $3 " */"}' 
cat << EOF

}
EOF
