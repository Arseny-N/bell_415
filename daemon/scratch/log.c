#define _BSD_SOURCE
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "error.h"
#include "log.h"
#include "head.h"

struct log_entry {
	struct log_entry *next;	
	char *buf;
	size_t buf_size;

};
struct log_entry *log_elist = NULL;

char *log_buf = NULL;
static struct log_entry *log_last = NULL;

void *_get_log_buf(int fd, size_t size)
{
	if(ftruncate(fd, size) == -1) {
		err_print("ftruncate");
		return NULL;
	}

	void *p = mmap( NULL, size, 
			PROT_WRITE | PROT_READ, 
			MAP_SHARED | MAP_LOCKED, fd, 0);
	if(p == MAP_FAILED) {
		err_print("mmap");
		return NULL;
	}
	printf("mmap %ld %p\n",size,p);
	return p;
}
static inline void _insert_log_entry(struct log_entry *p)
{
	log_last->next = p;
	log_last = log_last -> next;
	p->next = log_elist;
}
struct log_entry *__new_log_entry(void)
{
	struct log_entry *p;
	
	if(!log_elist) {       
		log_elist = malloc(sizeof(struct log_entry));
		p = log_elist;
		log_last = p;
	} else {
		p = malloc(sizeof(struct log_entry));
	}
	_insert_log_entry(p);
	return p;
}
int _create_log_entries(size_t numlines, size_t linesize) 
{
	int ind = 0;
	for (ind = 0; ind <= numlines*linesize; ind += linesize ) {
		struct log_entry *p = __new_log_entry();
		p->buf = &log_buf[ind];
		p->buf_size = linesize;		
		
		printf("Entry: %p buf:%p size:%ld\n", p,p->buf, p->buf_size);
	}
	return ind;
}
int log_open(char *file, size_t numlines, size_t linesize)
{
	size_t size = numlines * linesize;
	int fd = open(file, O_RDWR | O_CREAT | O_TRUNC, DEF_LOG_PERMS);
	if(fd == -1) {
		err_print("open %s", file);
		return -1;
	}
	
	log_buf = _get_log_buf(fd, size);
	if(!log_buf)
		return -1;       
	memset(log_buf, 0, size);

	return _create_log_entries(numlines,linesize);
}
struct log_entry *_fresh_log_entry(void)
{
	if(!log_elist) {
		wrn_print("log_elist NULL");
		return NULL;
	}
	printf("Processing: %p %p\n",log_elist,log_elist->buf);
	log_elist = log_elist->next;
	memset(log_elist->buf, 0, log_elist->buf_size);

	return log_elist;
}
int _str_transfer(struct log_entry *p, char *str, size_t size)
{	
	int wrote = snprintf(p->buf,p->buf_size,"%s",str);
	
	if(wrote != size) {
		p->buf[wrote-2] = '>';
		p->buf[wrote-1] = '>';
		p->buf[wrote] = '\0';
		wrote -= 3;
	}
	return wrote;
}
int log_write(char *str)
{
	size_t size = strlen(str), wrote = 0;       

	do {
		struct log_entry *p = _fresh_log_entry();
		wrote += _str_transfer(p, str, size);
	} while( wrote < size);
	return wrote;
}
