#include "generic.h"
#include "error.h"

#include "protocol.h"

static FILE *stream;
static char buf[BUF_SIZE];
static struct request request;
static bool once = 1;
void *read_rq(void) 
{
/* return fgets( buf, BUF_SIZE, stream); */
	if(once) {
		once = 0;
		sprintf(buf,"S a1.7:00 a2.8:00 ME.18:19 a4.22:54");
		return buf;  
	}
	sleep(1000);
	return NULL;
}

struct request *decode_buf(void *p) 
{ 	
	int type = get_rq_type(p);	
	if(type == -1) {
		goto err;
	}
		
	request.type = type;	
	skip_rq_type(p);

	if(get_rq_stuff(p, &request) == -1) {
		goto err;
	}
		

	return &request;
err:
	err_print_no_exit("(1)broken request");
	err_print        ("(2)%s", (char*)p);
	return NULL;
}
void destroy_rq ( struct request *rq ) 
{
	free_rq(rq);
}


void on_init(void)
{
//	stream = fopen(FIFO_NAME, "r");
//	if(stream == NULL)
//		err_print("open \'%s\'", FIFO_NAME);
	
//	return;
}


void on_finit(void)
{
//	if(fclose(stream) == -1)
//		err_print("close \'%s\' fd", FIFO_NAME);
}
struct method method = {
	.read = read_rq,
	.destroy = destroy_rq,
	.decode = decode_buf,

	.on_exit = on_finit,
	.init = on_init,
};
