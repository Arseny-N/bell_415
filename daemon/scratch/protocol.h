/*
 * Portocol:
 *       (1)  (2)                     (3)
 *     S 1.1:00 2.2:00 3.3:00 4.12:04
 *
 *     1 - reqest type.
 *     2 - descr.inter (timers).
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string.h>
#include "generic.h"
#define skip_chrs(p, len) p += len
#define skip_rq_type(p) skip_chrs(p, 1)
int free_rq(struct request *rq);
int get_rq_type( char *p );
int get_rq_stuff( char *p, struct request *rq );
#endif
