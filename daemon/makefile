LOG_DIR := "/var/log/bell"
PID_DIR := "/var/run/bell"
BIN_DIR := "/sbin"
PID_FILE := "$(PID_DIR)/bell_pid"

PROG_USER:= "arseni"
PROG_GROUP:= "arseni"

RINGER_USER:= "arseni"
RINGER_GROUP:= "arseni"

CC_FLAGS := -O2 -g  -Wall 

OBJ_CC_FLAGS :=  $(CC_FLAGS) $(shell mysql_config --cflags)

CC := gcc

obj = timers.o threads.o error.o time.o mysql.o cmd.o mysql_error.o

main: $(obj) ringer.c dcontrol
	$(CC) $(CC_FLAGS) -o run $(obj) \
	-lrt -lpthread `mysql_config --cflags --libs` main.c

	$(CC) $(CC_FLAGS) -o ringer ringer.c 

timers.o:  timers.c timers.h head.h
	$(CC) $(OBJ_CC_FLAGS) -c -lrt -o timers.o timers.c 

threads.o: threads.c threads.h head.h
	$(CC) $(OBJ_CC_FLAGS) -c -o threads.o threads.c 

error.o: error.c error.h head.h ename.h
	$(CC) $(OBJ_CC_FLAGS) -c -o error.o error.c  

time.o:  time.c time.h head.h
	$(CC) $(OBJ_CC_FLAGS) -c -o time.o time.c 

mysql.o: mysql.c mysql.h
	$(CC) $(OBJ_CC_FLAGS) -c -o mysql.o mysql.c 
mysql_error.o: mysql_error.c
	$(CC) $(OBJ_CC_FLAGS) -c -o mysql_error.o mysql_error.c 

cmd.o: cmd.h cmd.c
	$(CC) $(OBJ_CC_FLAGS) -c -o cmd.o cmd.c 


ename.h: mk-ename.sh
	./mk-ename.sh > ename.h
dcontrol:
	./mk-dcontrol.sh $(BIN_DIR) $(LOG_DIR) $(PID_FILE) > dcontrol; chmod +x dcontrol

.PHONY: clean install uninstall
clean:
	@rm -vf $(obj) ename.h 
clean_X2: clean
	@rm -vf ename.h dcontrol run ringer

install: 
	cp run $(BIN_DIR)/run
	cp dcontrol $(BIN_DIR)/dcontrol
	cp ringer $(BIN_DIR)/ringer
	mkdir $(LOG_DIR)
	mkdir $(PID_DIR)

	chown  $(PROG_USER)':'$(PROG_GROUP) $(BIN_DIR)/run  $(BIN_DIR)/dcontrol $(LOG_DIR) $(PID_DIR)
	chown $(RINGER_USER)':'$(RINGER_GROUP) $(BIN_DIR)/ringer

uninstall:
	@rm -vf $(BIN_DIR)/run
	@rm -vf $(BIN_DIR)/dcontrol
	@rm -vf $(BIN_DIR)/ringer

	@rm -vf $(LOG_DIR)/*
	rmdir $(LOG_DIR)

	@rm -vf $(PID_DIR)/*
	rmdir $(PID_DIR)
	@rm -vf $(PID_FILE)
