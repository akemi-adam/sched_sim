
# defining the compiler
CC = gcc

# defining the include directory (.h)
INC = -I./include

# defining the flags
CFLAGS = -Wall -g $(INC)

# main objects for all executables
OBJS = build/main.o \
	   build/proc.o \
	   build/queue.o \
	   build/sched.o \
	   build/utils.o \
	   build/time.o \
	   build/thread.o \
	   build/stats.o 

default: fifo

all: fifo sjf ljf prio_static prio_dynamic prio_dynamic_quantum

fifo: $(OBJS) build/schedulers/fifo.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/schedulers/fifo.o build/proc_init.o build/proc_interrupt.o -o main_fifo

sjf: $(OBJS) build/schedulers/sjf.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/schedulers/sjf.o build/proc_init.o build/proc_interrupt.o -o main_sjf

ljf: $(OBJS) build/schedulers/ljf.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/schedulers/ljf.o build/proc_init.o build/proc_interrupt.o -o main_ljf

prio_static: $(OBJS) build/schedulers/prio_static.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/schedulers/prio_static.o build/proc_init.o build/proc_interrupt.o -o main_prio_static

prio_dynamic: $(OBJS) build/schedulers/prio_dynamic.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/schedulers/prio_dynamic.o build/proc_init.o build/proc_interrupt.o -o main_prio_dynamic

prio_dynamic_quantum: $(OBJS) build/schedulers/prio_dynamic_quantum.o build/proc_init.o build/proc_interrupt.o
	$(CC) $(CFLAGS) -pthread $(OBJS) build/schedulers/prio_dynamic_quantum.o build/proc_init.o build/proc_interrupt.o -o main_prio_dynamic_quantum

build/main.o: main.c
	$(CC) $(CFLAGS) -c main.c

build/proc.o: src/proc.c include/proc.h
	$(CC) $(CFLAGS) -c src/proc.c

build/proc_init.o: src/proc_init.c include/proc_init.h
	$(CC) $(CFLAGS) -c src/proc_init.c

build/proc_interrupt.o: src/proc_interrupt.c include/proc_interrupt.h
	$(CC) $(CFLAGS) -c src/proc_interrupt.c

build/queue.o: src/queue.c include/queue.h
	$(CC) $(CFLAGS) -c src/queue.c

build/sched.o: src/sched.c include/sched.h
	$(CC) $(CFLAGS) -c src/sched.c

build/utils.o: src/utils.c include/utils.h
	$(CC) $(CFLAGS) -c src/utils.c

build/time.o: src/time.c include/time.h
	$(CC) $(CFLAGS) -c src/time.c

build/thread.o: src/thread.c include/thread.h
	$(CC) $(CFLAGS) -c src/thread.c

build/stats.o: src/stats.c include/stats.h
	$(CC) $(CFLAGS) -c src/stats.c

build/fifo.o: schedulers/fifo.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/fifo.c

build/sjf.o: schedulers/sjf.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/sjf.c

build/ljf.o: schedulers/ljf.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/ljf.c

build/prio_static.o: schedulers/prio_static.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/prio_static.c

build/prio_dynamic.o: schedulers/prio_dynamic.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/prio_dynamic.c

build/prio_dynamic_quantum.o: schedulers/prio_dynamic_quantum.c include/scheduler.h
	$(CC) $(CFLAGS) -c schedulers/prio_dynamic_quantum.c

clean:
	rm -f build/*.o
	rm -f main_fifo
	rm -f main_sjf
	rm -f main_ljf
	rm -f main_prio_static
	rm -f main_prio_dynamic
	rm -f main_prio_dynamic_quantum

