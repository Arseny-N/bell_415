LOG_DIR := "/var/log/bell"
PID_DIR := "/var/run/bell"
BIN_DIR := "/sbin"

WEB_GUI_PATH ?= "/var/www/html"

RING_PROG_NAME:="ringer"
BELL_PROG_NAME:="run"
CTRL_PROG_NAME:="dcontrol"

RING_PROG:=$(BIN_DIR)"/"$(RING_PROG_NAME)
BELL_PROG:=$(BIN_DIR)"/"$(BELL_PROG_NAME)
CTRL_PROG:=$(BIN_DIR)"/"$(CTRL_PROG_NAME)

# add to startup pid dir creation

PID_FILE := $(PID_DIR)"/pid"
 
DEBUG_LOG_FILE:= $(LOG_DIR)"/dbg"
ERROR_LOG_FILE:= $(LOG_DIR)"/err"


DEFAULT_HOST:="localhost"
DEFAULT_DB:="ringer"
DEFAULT_USER:="arseni"
DEFAULT_PASS:="1"

PROG_USER:= "arseni"
PROG_GROUP:= "arseni"

RINGER_USER:= "root"
RINGER_GROUP:= "root"

LOGRTE_SRPT:="bell"
SYSD_UNIT_FILE := "bell.service"

SYSD_DIR:=$(shell pkg-config systemd --variable=systemdsystemunitdir)

DEFAULT_UID:=$(shell id -u $(PROG_USER))
DEFAULT_GID:=$(shell id -u $(PROG_GROUP))




CC_FLAGS := -O2 -g  -Wall 

MYSQL_CFLAGS:= $(shell mysql_config --cflags)
MYSQL_CLIBS:= $(shell mysql_config --libs)

OBJ_CC_FLAGS :=  $(CC_FLAGS) $(MYSQL_CFLAGS)

CC := gcc

obj = timers.o error.o time.o mysql.o cmd.o mysql_error.o caps.o creds.o

main: $(obj) ringer dcontrol logrotate systemd
	$(CC) $(CC_FLAGS) -o run $(obj) \
	-lrt -lcap -lpthread $(MYSQL_CLIBS) $(MYSQL_CFLAGS) main.c \
	-DDEBUG_LOG_FILE=\"$(DEBUG_LOG_FILE)\" \
	-DERROR_LOG_FILE=\"$(ERROR_LOG_FILE)\" \
	-DPID_FILE=\"$(PID_FILE)\" \
	-DRING_PROG=\"$(RING_PROG)\"  \
	-DDEFAULT_HOST=\"$(DEFAULT_HOST)\" \
	-DDEFAULT_DB=\"$(DEFAULT_DB)\" \
	-DDEFAULT_USER=\"$(DEFAULT_USER)\"  \
	-DDEFAULT_PASS=\"$(DEFAULT_PASS)\"  \
	-DDEFAULT_UID=$(DEFAULT_UID)  \
	-DDEFAULT_GID=$(DEFAULT_GID) 

timers.o:  timers.c timers.h head.h
	$(CC) $(OBJ_CC_FLAGS) -c -lrt -o timers.o timers.c 	
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
creds.o: creds.h creds.c
	$(CC) $(OBJ_CC_FLAGS) -c -o creds.o creds.c
caps.o: caps.h caps.c
	$(CC) $(OBJ_CC_FLAGS) -c -o caps.o caps.c 
pi_ringer:
	$(CC) $(CC_FLAGS) -o ringer wiring_ringer.c -lwiringPi
ringer:
	$(CC) $(CC_FLAGS) -o ringer ringer.c

ename.h: mk-ename.sh
	./mk-ename.sh > ename.h

.ONESHELL:
#dcontrol: mk-dcontrol.sh
#	@printf "#!/bin/bash \nring_prog=$(RING_PROG)\nbell_prog=$(BELL_PROG)\nERR_LOG=$(ERROR_LOG_FILE)\nDBG_LOG=$(DEBUG_LOG_FILE)\nPID_FILE=$(PID_FILE)\nP_USER=$(PROG_USER)\nP_GROUP=$(PROG_GROUP)\n" > $(CTRL_PROG_NAME)
#	cat dcontrol.sh.scr >> $(CTRL_PROG_NAME)

dcontrol: error.o
	gcc -o $(CTRL_PROG_NAME) dcontrol.c -DPID_FILE=\"$(PID_FILE)\" -DBELL_PROG=\"$(BELL_PROG)\"

.ONESHELL:
logrotate:
	@printf "$(DEBUG_LOG_FILE) $(ERROR_LOG_FILE) {\n\tweekly\n\trotate 4\n\tdateext\n\tcreate 0664 $(PROG_USER) $(PROG_GROUP)\n\tifempty\n}" > $(LOGRTE_SRPT)

.ONESHELL:
systemd:
	@printf "# Bell Daemon\n[Unit]\nDescription= Bell daemon\nDocumentation= file:$(WEB_GUI_PATH)/help.php http://localhost/help.php\nRequiers= mysql.service httpd.service\nWants= mysqld.service httpd.service\nAfter= mysqld.service\n\n[Service]\nExecStart=$(CTRL_PROG) exec-up\nExecReload=/bin/kill -HUP $MAINPID\nKillMode=process\n[Install]\nWantedBy=multi-user.target " > $(SYSD_UNIT_FILE)



.PHONY: clean install uninstall
clean:
	@rm -vf $(obj) ename.h 
clean_X2: clean
	@rm -vf ename.h $(BELL_PROG_NAME) $(RING_PROG_NAME) $(CTRL_PROG_NAME) $(LOGRTE_SRPT) $(SYSD_UNIT_FILE)

install: 		
	cp ./$(BELL_PROG_NAME) $(BELL_PROG)
	cp ./$(RING_PROG_NAME) $(RING_PROG)
	cp ./$(CTRL_PROG_NAME) $(CTRL_PROG)		
	#chmod +x $(CTRL_PROG)
	cp ./$(LOGRTE_SRPT) /etc/logrotate.d/$(LOGRTE_SRPT)
	# cp $(SYSD_UNIT_FILE) $(SYSD_DIR)
	mkdir $(LOG_DIR);
	chown  $(PROG_USER)':'$(PROG_GROUP) $(BELL_PROG) $(CTRL_PROG) $(LOG_DIR)
	chown $(RINGER_USER)':'$(RINGER_GROUP) $(RING_PROG)	
	chmod 'ug+s' $(CTRL_PROG) # Disabled for scripts ?!
	chmod 'ug+s' $(RING_PROG) # Disabled for scripts ?!
	
	
	setcap 'cap_setuid=pe cap_setgid=pe cap_dac_override=pe' $(BELL_PROG)	
	setcap 'cap_kill=pe cap_dac_override=pe' $(CTRL_PROG)


uninstall:
	#@$(CTRL_PROG) kill
	@rm -vf $(BELL_PROG) $(CTRL_PROG) $(RING_PROG) $(LOG_DIR)/*  $(SYSD_DIR)/$(SYSD_UNIT_FILE)
	@rmdir $(LOG_DIR)

reinstall: uninstall install
	
