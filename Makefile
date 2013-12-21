CC=g++
SRC=sim_manager.cc lru.cc
TARGET=sim_manager

all: sim_manager.cc lru.cc
	$(CC) $(SRC) -o $(TARGET) 

clean:
	rm -rf *.o


