#include <pthread.h>
#include "error.h"
/**/

void *reader_thread(void *non_used);              /* Get request from gui, run _sever_thread */
void *_server_thread(void *vrq);                  /* Serve requests from gui */

void restorer_thread(union sigval sigval_self);  /* Ring the bell, perform cleanup */
void *sig_thread(void *not_used);                /* Handle signals */

/* Just for fun :-) */
#define __mk_start_th(__th_name)					\
	static inline int start_##__th_name##_thread (void)		\
	{								\
		pthread_t __dummy;					\
		int e = pthread_create( &__dummy, NULL, __th_name##_thread, NULL); \
		if(e) {							\
			nerr_print( e, "pthread_create" );		\
			return -1;					\
		}							\
		return 0;						\
	}								\
	
__mk_start_th(sig);
__mk_start_th(reader);

