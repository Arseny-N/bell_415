CC_FLAGS := -O2 -g  -Wall
CC := gcc

obj = timers.o threads.o bell_ring.o generic.o error.o
main: $(obj)
	$(CC) $(CC_FLAGS) -o run $(obj) -lrt -lpthread main.c

timers.o:  timers.c timers.h head.h
	$(CC) $(CC_FALGS) -c -lrt -o timers.o timers.c

threads.o: threads.c threads.h head.h
	$(CC) $(CC_FALGS) -c -lrt -lpthread -o threads.o threads.c 

bell_ring.o: bell_ring.h bell_ring.c head.h
	$(CC) $(CC_FALGS) -c -o bell_ring.o bell_ring.c

generic.o: generic.c generic.h head.h
	$(CC) $(CC_FALGS) -c -o generic.o generic.c

error.o: error.c error.h head.h
	$(CC) $(CC_FALGS) -c -o error.o error.c -lpthread

.PHONY: clean

clean:
	rm -vf *.o
	rm -vf run

