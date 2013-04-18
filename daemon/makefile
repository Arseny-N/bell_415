CC_FLAGS := -O2 -g  -Wall 

OBJ_CC_FLAGS :=  $(CC_FLAGS) $(shell mysql_config --cflags)

CC := gcc

obj = timers.o threads.o error.o time.o mysql.o cmd.o mysql_error.o

main: $(obj) ringer.c ename
	$(CC) $(CC_FLAGS) -o run $(obj) \
	-lrt -lpthread `mysql_config --cflags --libs` main.c
	$(CC) $(CC_FLAGS) -o ringer ringer.c 

timers.o:  timers.c timers.h head.h
	$(CC) $(OBJ_CC_FLAGS) -c -lrt -o timers.o timers.c 

threads.o: threads.c threads.h head.h
	$(CC) $(OBJ_CC_FLAGS) -c -o threads.o threads.c 

error.o: error.c error.h head.h
	$(CC) $(OBJ_CC_FLAGS) -c -o error.o error.c  

time.o:  time.c time.h head.h
	$(CC) $(OBJ_CC_FLAGS) -c -o time.o time.c 

mysql.o: mysql.c mysql.h
	$(CC) $(OBJ_CC_FLAGS) -c -o mysql.o mysql.c 
mysql_error.o: mysql_error.c
	$(CC) $(OBJ_CC_FLAGS) -c -o mysql_error.o mysql_error.c 

cmd.o: cmd.h cmd.c
	$(CC) $(OBJ_CC_FLAGS) -c -o cmd.o cmd.c 
ename: mk-ename.sh
	./mk-ename.sh > ename.h

.PHONY: clean install uninstall
clean:
	rm -vf $(obj)

install: 
	cp run /sbin/run
	cp dcontrol /sbin/dcontrol
	cp ringer /sbin/ringer

	mkdir /var/log/bell

uninstall:
	rm -vf /sbin/run
	rm -vf /sbin/dcontrol
	rm -vf /sbin/ringer

	rm -vf /tmp/pid_file
