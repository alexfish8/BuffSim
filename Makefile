# Makefile

CC= g++
INCLUDE = include
LIB = -lboost_system -lboost_thread #-lboost_thread-mt
CCFLAGS = -g -I $(INCLUDE) $(LIB)

all: sim_manager sim

sim_manager: sim_manager.cc trace.o cache.o lru.o
	$(CC) $(CCFLAGS) trace.o cache.o lru.o sim_manager.cc -o sim_manager

sim: sim.cc trace.o cache.o lru.o
	$(CC) $(CCFLAGS) trace.o cache.o lru.o sim.cc -o sim

trace.o: trace.cc $(INCLUDE)/trace.h
	$(CC) $(CCFLAGS) -c trace.cc -o trace.o

cache.o: cache.cc $(INCLUDE)/cache.h
	$(CC) $(CCFLAGS) -c cache.cc -o cache.o

lru.o: lru.cc $(INCLUDE)/lru.h
	$(CC) $(CCFLAGS) -c lru.cc -o lru.o

clean:
	rm -f *.o $(TARGET)


