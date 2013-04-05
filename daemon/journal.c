
/*
 *  After every valid operation on the linked list,
 *  the upper mentioned is written to one of the N files
 *  which are switched sequentaly
 */
struct journal_list *j_list = NULL;
static void _insert_jentry(struct journal_list *p)
{
	p->next = j_list->next;
	j_list->next = p;		
}
static struct journal_list *new_jentry(void)
{
	struct journal_list *p;
	if(!j_list) {
		j_list = malloc(sizeof(struct journal_list));		
		p = j_list;
	}
	_insert_jentry(p);
}
static struct journal_list *fresh_jentry(void)
{
	if(!j_list) {
		return NULL;
	}
	j_list = j_list->next;
	return j_list;
}
int journal_open(int n, char **fv)
{
	if(j_list)
		return -1;
	struct journal_list *p;
	for(i = 0 ;i < n ; ++i) {
		p = new_jentry();
		if(!p) {
			wrn_print("new_jentry");
			continue;
		}
		j_list->name = strdup(fv[i]);
		p->fd = open(fv[i], O_CREAT | O_DIRECT | O_WRONLY, 0777);
		if(p->fd == -1) {
			err_print("open() %s", fv[i]);			
			continue;
		}
		
	}
}
int journal_close(void)
{
}
static long _descr_hash(char *s)
{
	long h = 0;
	while(*s) {
		h += *s;
		s++;
	}
	return h;
}
static long timer_hash(struct timers *timer)
{
	long h = 0;
	h += timer->state;
	h += timer->inter;
	h += timer->md_inter;
	h += _descr_hash(timer->descr);
	return h;	
}

static void _write_time(int fd)
{
	time_t t;
	time(&t);
   	if( write( fd, &t, sizeof(t) ) != sizeof(t))
		wrn_print("partial write occured");	
}
static void _journal_write(int fd)
{
	long resh = 0;
	for_each_timer(p) {
		struct timer_jentry je;

		je.hash = timer_hash(p);
		resh += je.hash;

		memcpy(je.timer,p,sizeof(*p));	
		
		if( write( fd, &je, sizeof(je) ) != sizeof(je))
			wrn_print("partial write occured");		
	}
}
void journal_write(void)
{
	struct journal_list *p;
	p = fresh_jentry();
	if(!p){
		return;		
	}
	_write_time(jp->fd);
	_journal_write(p->fd);
}

static int broken(struct timer_jentry *je)
{
	return je -> hash != timer_hash(&je->timer);
}
static void _journal_read(int fd)
{
	struct timer_jentry je;
	struct timers *p;	
	
	while(read(fd, &je, sizeof(je)) == sizeof(je)) {
		
		if(broken (&je)) 
			wrn_print("broken entry");				
		if( timer_mk_md(je.timer.descr, je.timer.md_inter) == -1 ) {
			wrn_print("timer_mk_md failed on %s", je.timer.descr);
		}
	}
	
}
static struct journal_list * _find_jentry(char *name)
{
	for_each_jentry(p) {
		if( name && p->name )
			if(strcmp(name,p->name) == 0) 
				return p;
	}
	return NULL;
}
void journal_read(char *name)
{
	struct journal_list *jp = _find_jentry(name);
	if(!jp) {
		wrn_print("%s not found",name);
		return;
	}
	_read_date(jp->fd);
	_journal_read(jp->fd);
}