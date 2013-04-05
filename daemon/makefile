CC_FLAGS := -O2 -g  -Wall
CC := gcc

obj = timers.o threads.o bell_ring.o generic.o \
      error.o protocol.o inter.o journal.o

main: $(obj)
	$(CC) $(CC_FLAGS) -o run $(obj) -lrt -lpthread main.c

timers.o:  timers.c timers.h head.h
	$(CC) $(CC_FLAGS) -c -lrt -o timers.o timers.c

threads.o: threads.c threads.h head.h
	$(CC) $(CC_FLAGS) -c -lrt -lpthread -o threads.o threads.c 

error.o: error.c error.h head.h
	$(CC) $(CC_FLAGS) -c -o error.o error.c -lpthread



bell_ring.o: bell_ring.h bell_ring.c head.h
	$(CC) $(CC_FLAGS) -c -o bell_ring.o bell_ring.c



generic.o: generic.c generic.h head.h
	$(CC) $(CC_FLAGS) -c -o generic.o generic.c 

protocol.o: protocol.h protocol.c
	$(CC) $(CC_FLAGS) -c -o protocol.o protocol.c 

inter.o: inter.h inter.c
	$(CC) $(CC_FLAGS) -c -o inter.o inter.c 	



journal.o: journal.h journal.c
	$(CC) $(CC_FLAGS) -c -o journal.o journal.c 	

.PHONY: clean decoder inter_calc jmanager

decoder: protocol.o error.o decoder.c protocol.c protocol.h
	$(CC) $(CC_FLAGS) -o decoder decoder.c error.o protocol.o 
inter_calc: inter.o error.o decoder.c inter.c inter.h inter_calc.c
	$(CC) $(CC_FLAGS) -o inter_calc inter_calc.c error.o inter.o 
jreader: journal.h journal.c journal.o jreader.c
	$(CC) $(CC_FLAGS) -o jreader jreader.c error.o #journal.o timers.o threads.o generic.o

clean:
	rm -vf $(obj)
#	rm -vf run

