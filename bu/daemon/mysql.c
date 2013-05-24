
#include "mysql.h"
#include "error.h"
#include "time.h"
#include "timers.h"
#include "head.h"
#include "cmd.h"

static MYSQL *open_mysql( char *host, char *dbname, char *user, char *pass)
{
	MYSQL *conn = malloc(sizeof(MYSQL));
	if(!conn) {
		err_print("malloc");
		return NULL;
	}
	conn = mysql_init(NULL);
	if(!conn) {
		mysqle_print(conn,"my_init");
		return NULL;
	}

	//dbg_print("Connecting to host:\"%s\" user:\"%s\" pass:\"%s\" dbname:\"%s\"",host,user,pass,dbname);
	
	if(mysql_real_connect(conn, host, user, pass, dbname, 0,NULL,0) == NULL ) {
		mysqle_print(conn,"mysql_real_connect");
		return NULL;
	}
	return conn;
}
static int close_mysql(MYSQL *p) 
{
	mysql_close(p);
	return 0;
}


static MYSQL_RES *query_result(MYSQL *mysql, char *q)
{
	if( mysql_query(mysql, q) != 0 ) {
		mysqle_print(mysql, "mysql_query");
		return NULL;
	}

	MYSQL_RES *result = mysql_store_result(mysql);
	if(!result) {
		mysqle_print(mysql, "mysql_store_result");
		goto err;
	}
	return result;
err:
	mysql_free_result(result);
	return NULL;
}

static void push_timer_from_row(MYSQL_ROW row, int fields) 
{
	char *rng_duration, *rng_time;
	if( fields < 3 ) {
		wrn_print("fields(%d) < 3",fields);
		return;
	}
	rng_duration = row[3];
	rng_time = row[2]; 
	
	struct sigevent se;
	se.sigev_notify = SIGEV_SIGNAL;

	se.sigev_signo = fit_default_durations(rng_duration);

	if(se.sigev_signo == -1 ) {
		struct tm *tm = str_to_tm(rng_duration,0);
		
		se.sigev_signo = SIG_RING_CUSTOM;				
		se.sigev_value.sival_int = tm->tm_sec + tm->tm_min * 60 + tm->tm_hour * 3600;		
	}

	if( push_timer_from_string(&se, rng_time, NULL,TIMER_ABSTIME,1) == -1 ) {
		wrn_print("push_timer_from_string %s", rng_time);
		return;			
	}

	dbg_print("duration: (%s) time: (%s)",rng_duration, rng_time);	
	
}
static void for_each_result_entry( MYSQL_RES *result, void (*row_func) ( MYSQL_ROW row, int fields))
{      
	int fields = mysql_num_fields(result);
	if(fields == -1) {
		mysqle_print(NULL,"mysql_num_fields");
		return;
	}
	MYSQL_ROW row;

	while ((row = mysql_fetch_row(result))) {	
	  row_func(row,fields);
	}	

}
static char *get_query_first_field(MYSQL *mysql,  char *query)
{
	MYSQL_RES *result = query_result(mysql,query);
	char *r = NULL;
	if(!result) {       
		mysqle_print(mysql, "query_result");
		goto freeing;
	}
	
	
	static char buf[MAX_PROFID];
	
	

	MYSQL_ROW row = mysql_fetch_row(result);
	if(row == NULL) {		
		goto freeing;
	}

	snprintf(buf,MAX_PROFID,"%s",row[0]);
	r = &buf[0];

freeing:
	mysql_free_result(result);
	return r;
}



static inline char *gen_override_query(void)
{
	static char buf[MAX_QUERY];
	snprintf(buf,MAX_QUERY,"SELECT profile_id FROM overrides WHERE over_date='%s'",curr_date());
	return buf;
}
static inline char *gen_rule_query(void)
{
	static char buf[MAX_QUERY];
	snprintf(buf,MAX_QUERY,"SELECT profile_id FROM rules WHERE rule_day='%s'",curr_dayn());
	return buf;
}
static inline char *gen_ring_query(char *prof)
{
	static char buf[MAX_QUERY];
	snprintf(buf,MAX_QUERY,"SELECT * FROM rings WHERE profile_id='%s'",prof);
	return buf;
}



static char *check_overrides(MYSQL *mysql)
{
	char *q = gen_override_query();
	char *r = get_query_first_field(mysql, q);
	return r; 
}
static char *get_by_rules(MYSQL *mysql)
{
	char *q = gen_rule_query();
	char *r = get_query_first_field(mysql, q);
	return r; 
}
static char *fresh_profile_id(MYSQL *mysql)
{
	char *p = check_overrides(mysql);
	dbg_print("override %s%s", p ? "fetched " :"empty", p ? p :"");
	if(!p) {
		p = get_by_rules(mysql);	
		dbg_print("rule %s%s", p ? "fetched " :"empty", p ? p :"");
	}
	return p;       
	
}




static MYSQL_RES *get_rings(MYSQL *sql,char *prof)
{
	char *q = gen_ring_query(prof);
	return query_result(sql,q);
}



int arm_main_timers(void)
{
	int r = 0;
	MYSQL *mysql = open_mysql( cmd.host, cmd.dbname, cmd.user, cmd.pass);
	if(!mysql) {
		wrn_print("open_mysql");
		return -1;
	}
	
	char *prof = fresh_profile_id(mysql);
	if(!prof) {
		wrn_print("fresh_profile_id");
		r = -1;
		goto close;
	}

	MYSQL_RES *result = get_rings(mysql, prof);	

	if(!result) {
		wrn_print("get_rings");
		r = -1;
		goto close;
	}	   

	for_each_result_entry(result, push_timer_from_row);

	mysql_free_result(result);	       
close:
	close_mysql(mysql);
	return r;
}
