//#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

#define DELAY_OK(d) (d > 0)

#define O_PIN 7

#ifndef PRECISION
# define PRECISION 1
#endif
#define PLAY_LIST "/home/arseni/Documents/p/bell_on_pi/daemon/bell_songs"
//#define PLAY_LIST "/etc/bell_songs"
#define MAX_MELODY 1024   /* == 16*8 */

#define OPT_CHAR '$'
//#define VAR_CHAR '$'
#define COMMENT_CHAR '#'
#define NEW_SONG_CHAR_OPEN '['
#define NEW_SONG_CHAR_CLOSE ']'

struct songs
{
	char *name;
	unsigned int id;
	int quantum;				
	
	int length;
	unsigned char melody[MAX_MELODY];
	
	struct codec {
		char *name;
		void(*codec_func) (struct songs *s);
		char *raw;
	} codec;
	
#if	0
	struct var {
		char *name;
		char *value;
		struct var *next;
	} *vars;
#endif
	struct songs *next;
};


#define for_each_option(op) for(op = options;op->name; op+=1)
#define for_each_codec(op) for(op = codecs;op->name; op+=1)
#define for_each_song(sp,s) for(sp = s; sp; sp=sp->next )
#define for_each_var(vr, v) for_each_song(vr, v)

static struct codecs *find_codec(char *name);


static void codec_one(struct songs *s)
{
	char *raw = s->codec.raw;
	int ind = 0, bind = 0;
		
	s->melody[ind] = 0;
	s->length = 0;
	
	if(!raw)
		return;
		
	for(;*raw; raw += 1, s->length ++) {
		if(*raw != '+' && *raw != '-')
			continue;								
		if(s->length > MAX_MELODY) 
			return;		
		
		
		if(bind == 8) {	
			s->melody[++ind] = 0;
			bind = 0;
		}
		
		if(*raw == '+') 
			s->melody[ind] |= 1;
											
		s->melody[ind] <<= 1;
		bind ++;
	}
	

}

static struct codecs {
	char *name;
	void(*codec_func) (struct songs *pl);
} codecs[] = {
	{.name = "codec_one", .codec_func = codec_one},
	{.name = NULL },
};
static struct codecs *find_codec(char *name)
{	
	struct codecs *cp;
	for_each_codec(cp) {
		if(strcmp(name,cp->name) == 0)
			return cp;
	}
	return NULL;
}
static void assign_id(char *val, struct songs *s)
{	
	s->id = atoi(val);
}
static void assign_quantum(char *val, struct songs *s)
{
	s->quantum = atoi(val);
}
static void assign_codec(char *val, struct songs *s)
{
	s->codec.name = strdup(val);
	
	struct codecs *codec = find_codec(s->codec.name);
	
	s->codec.codec_func = codec ? codec ->codec_func : NULL ;
}
static void free_codec(struct songs *s)
{
	if(s->codec.name)
		free(s->codec.name);
}

static void assign_melody(char *val, struct songs *s)
{
	s->codec.raw = strdup(val);
}
static void free_melody(struct songs *s)
{
	if(s->codec.raw)
		free(s->codec.raw);
}


static int strfind(char *p1, char *p2, int delim)
{	
	while(*p1 == *p2 && *p1 != delim && 
	      *p2 != delim && *p1 && *p2) {
			p1 += 1;
			p2 += 1;      		
	}
	return *p1 == *p2 || 
	       *p1 == 0 && *p2 == delim ||
	       *p2 == 0 && *p1 == delim ? 0 : *p1 < *p2 ? -1 : 1;
}
static struct options {
	char *name;
	void(*assign_func) (char *val, struct songs *pl);
	void(*free_func) (struct songs *pl);	
	bool free_me;
} options[] = {
	{ .name = "id", .assign_func = assign_id, .free_func = NULL, .free_me = 0},
	{ .name = "quantum", .assign_func = assign_quantum, .free_func = NULL,.free_me = 0},
	{ .name = "melody", .assign_func = assign_melody, .free_func = free_melody,.free_me = 0},
	{ .name = "codec", .assign_func = assign_codec, .free_func = free_codec,.free_me = 0},
	{ .name = NULL },
};
#if 0
char *var_sub(char *line, struct songs *s)
{
	char *buf = malloc(MAX_LINE);
	char *p  = strchr(line,VAR_CHAR);
	size_t bufFill = 0;
	do{		
		if(*line == VAR_CHAR) {
			
			get_var()
		}
		buf[bufFill] = *line;		
		
	
 	}while(1)
}
static int var_assign(char *line, struct songs *s)
{
	struct var *var;
	char *p = strchr(line, '='), *vp = p;
	char *ep = &vp[strlen(vp)-1];	
	
	do 
		p -= 1;
	while(isspace(*p));

	while(isspace(*ep)) 
		ep -= 1;	
	ep[1] = 0;
	
	do
		vp += 1;
	while(isspace(*vp));
	
	for_each_var(s->vars, var) 
		if(strfind(var->name,line, p[1]) == 0){
			free(var->value);
			var->value = strdup(vp);			
			return 0;
		}
	if(!s->vars) {
		s->vars = malloc(sizeof(*var));
		var = s->vars;
		if(!var)
			return -1;
	} else {	
		for(var = s->vars; var->next; var= var->next) 
			continue;
		var->next = malloc(sizeof(*var));
		if(!var->next)
			return -1;
	}
	
	p[1] = 0;
	var->name = strdup(line);
	var->value = strdup(vp);	
	return 0;	
}
#endif
static int opt_assign(char *line, struct songs *s)
{
	
	struct options *op;
	char *p = strchr(line, '='), *vp = p;
	char *ep = &vp[strlen(vp)-1], *pp;	
	
	do 
		p -= 1;
	while(isspace(*p));

	while(isspace(*ep)) 
		ep -= 1;	
	ep[1] = 0;
	
	do
		vp += 1;
	while(isspace(*vp));
	//pp = var_sub(vp, s);
	///printf(":::::::%s\n")
	for_each_option(op) {		
		if(strfind(op->name,line, p[1]) == 0){
			op->assign_func(vp, s);
			op->free_me = 1;
			return 0;
		}
	}
	free(pp);
	return -1;	
}

static inline void set_if_unset(struct songs *s)
{
	
}
char *Unnamed = "Unnamed";
char *def_codec = "codec_one";
static inline void set_defaults(struct songs *s)
{
	s->next = NULL;
	s->name = strdup(Unnamed);
	s->quantum = 10000;
	s->id = 0;
	s->length = 0;
	
	s->codec.name = strdup(def_codec);
	s->codec.codec_func = codec_one;
	
}
static void get_option(char *line, struct songs **pl)
{
	char *p;
	
	for(;*line;line ++) {
		if(isspace(*line))
			continue;		
		switch(*line) {
			case OPT_CHAR:				
				
				if(opt_assign(&line[1], *pl) == -1)
					fprintf(stderr,"E: error in playlist"
						  " near:%s\n",line);
				return;
			//case VAR_CHAR:
			//	if(var_assign(&line[1], *pl) == -1)
			//		fprintf(stderr,"E: error in var assign"
			//			  " near:%s\n",line);
			//	return;
				
			case COMMENT_CHAR:
				return;
			case NEW_SONG_CHAR_OPEN:
				
				if((*pl)->name) {
					set_if_unset(*pl);
					
					(*pl)->next = malloc(sizeof(struct songs));
					(*pl) = (*pl)->next;
				}
				set_defaults(*pl);				
				p = strchr(line, NEW_SONG_CHAR_CLOSE);
				if(p)
					*p = '\0';
				
				(*pl) -> name = strdup(&line[1]);				
				return;			
		}
			
	}
}
struct songs *load_playlist(char *pl_file)
{
	struct songs *pl = malloc (sizeof(*pl)), *plp = pl;	
	FILE *s = fopen(pl_file, "r");
	char *line = NULL;
	size_t len = 0;
	if(!s||!pl)
		return NULL;
	
	pl->name = NULL;
	
	while(getline(&line, &len, s) != -1) {	
		get_option(line,&plp);	
	}		
	return pl;
}
#ifndef ON_PI
void play_melody(unsigned char *melody, int length, int quantum)
{
	int ind = 0;
	unsigned char mask = 1<<7;
	
	while(length--) {
		if(mask == 0) {
			ind ++;
			mask = 1<<7;			
		}
		if(melody[ind] & mask) {
			printf("1\n");
		}		
		usleep(quantum);		
		mask >>= 1;
	}

}
#else
void play_melody(unsigned char *melody, int length, int quantum)
{
	int ind = 0;
	unsigned char mask = 1<<7;
	bool state = 0;
	
	while(length--) {
		if(mask == 0) {
			ind ++;
			mask = 1<<7;	
			printf("\n");		
		}
		if(melody[ind] & mask ) {
			if(!state){
				digitalWrite(O_PIN, HIGH);
				state = 1;
				printf("1");
			}
		} else {
			if(state) {
				digitalWrite(O_PIN, LOW);
				state = 0;
				printf("0");
			}
		}
		usleep(quantum);		
		mask >>= 1;
	}
	if(state)
		digitalWrite(O_PIN, LOW);

}
#endif
struct songs *find_song(struct songs *s,int songId, char *songName)
{
	struct songs *sp;
	if(songId > 0 || songName) {
		for_each_song(sp, s) {
			if(songId>0)
				if(sp->id == songId)
					return sp;
			if(songName)
				if(strcmp(songName, sp->name)==0)
					return sp;
		}
	}
	return NULL;
}
static inline void free_needed(struct songs *s)
{
	struct options *op;
	for_each_option(op)
		if(op->free_me && op->free_func )
			op->free_func(s);	
}
void unload_playlist(struct songs *s)
{
	struct songs *next;
	for(; s; s = next ) {
		free_needed(s);
		next=s->next;
		free(s);
	}
	return;
}
static void singer(int songId)
{
	struct songs *sp,*s = load_playlist(PLAY_LIST);
	if(!s) {
		fprintf(stderr,"No playlist\n");
		return;		
	}
	
	printf("Play List Loadeed\n");
	sp = find_song(s,-songId,NULL);
	if(!sp) {
		fprintf(stderr,"No such song %d\n",songId);
		return;
	}
	printf("Song Found\n");
	sp->codec.codec_func(sp);	
	printf("Raw Data Decoded\n");
	
	play_melody(sp->melody, sp->length, sp->quantum);	
	
	unload_playlist(s);

}

int main( int argc, char *argv[])
{
	if(!argv[1])
		exit(EXIT_FAILURE);

	int d = atoi(argv[1]);	
	singer(d);
	/*
	if (wiringPiSetup () == -1) {
		perror("wirionPiSetup");
		exit (EXIT_FAILURE);
	}
	pinMode(O_PIN, OUTPUT);	
		
	if(!DELAY_OK(d)) {
		
		exit(EXIT_SUCCESS);
	}
	

	
	digitalWrite(O_PIN, HIGH);

	delay (d * PRECISION);

	digitalWrite(O_PIN, LOW);
*/
	return 0;

}

