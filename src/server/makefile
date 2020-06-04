CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -ggdb -lpthread -pthread
OUT_PATH=../../bin

all: server

server: main.cpp WordLibrary.cpp Server.cpp GameSession.cpp Network.cpp Connection.cpp
	$(CC) $(CFLAGS) main.cpp WordLibrary.cpp Server.cpp GameSession.cpp Network.cpp Connection.cpp -o ${OUT_PATH}/server

clean:
	rm ${OUT_PATH}/*.o