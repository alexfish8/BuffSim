# Makefile

CC= g++
INCLUDE = include
LIB = -lboost_system -lboost_thread
CCFLAGS = -I $(INCLUDE) $(LIB)
SIM = lru.cc
SRC = sim_manager.cc trace.cc cache.cc $(SIM)
OBJ = $(SRC: %.cc=%.o)
TARGET = sim_manager


all: $(SRC) sim
	$(CC) $(CCFLAGS) $(SRC) -o $(TARGET) 

sim: $(SRC)
	$(CC) $(CCFLAGS) trace.cc cache.cc sim.cc lru.cc -o sim

clean:
	rm -f *.o $(TARGET)


