CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -ggdb
OUT_PATH=../../bin

all: client

client: main.cpp Client.cpp InterfaceManager.cpp
	$(CC) $(CFLAGS) main.cpp Client.cpp InterfaceManager.cpp -o ${OUT_PATH}/client

clean:
	rm ${OUT_PATH}/*.o