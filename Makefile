# Makefile

CC= g++
INCLUDE = include
LIB = -lboost_system -lboost_thread
CCFLAGS = -I $(INCLUDE) $(LIB)
SIM = lru.cc
SRC = sim_manager.cc trace.cc cache.cc $(SIM)
OBJ = $(SRC: %.cc=%.o)
TARGET = sim_manager

all: $(SRC)
	$(CC) $(CCFLAGS) $(SRC) -o $(TARGET) 

clean:
	rm -f *.o $(TARGET)


