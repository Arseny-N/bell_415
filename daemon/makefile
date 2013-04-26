LOG_DIR := "/var/log/bell"
PID_DIR := "/var/run/bell"
BIN_DIR := "/sbin"
PID_FILE := "$(PID_DIR)/pid"

DEBUG_LOG_FILE:= $(LOG_DIR)"/dbg"
ERROR_LOG_FILE:= $(LOG_DIR)"/err"

PROG_USER:= "arseni"
PROG_GROUP:= "arseni"

RINGER_USER:= "arseni"
RINGER_GROUP:= "arseni"

CC_FLAGS := -O2 -g  -Wall 

MYSQL_CFLAGS:= $(shell mysql_config --cflags)
MYSQL_CLIBS:= $(shell mysql_config --libs)

#MYSQL_CFLAGS:=-I/usr/include/mysql -g -pipe -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4  -m64 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fno-strict-aliasing -fwrapv -fPIC  -fPIC -g -static-libgcc -fno-omit-frame-pointer -fno-strict-aliasing -DMY_PTHREAD_FASTMUTEX=1
#MYSQL_CLIBS:=-L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl

OBJ_CC_FLAGS :=  $(CC_FLAGS) $(MYSQL_CFLAGS)


CC := gcc

obj = timers.o threads.o error.o time.o mysql.o cmd.o mysql_error.o

main: $(obj) ringer.c dcontrol
	$(CC) $(CC_FLAGS) -o run $(obj) \
	-lrt -lpthread $(MYSQL_CLIBS) $(MYSQL_CFLAGS) main.c \
	-DDEBUG_LOG_FILE=\"$(DEBUG_LOG_FILE)\" -DERROR_LOG_FILE=\"$(ERROR_LOG_FILE)\" -DPID_FILE=\"$(PID_FILE)\"

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
