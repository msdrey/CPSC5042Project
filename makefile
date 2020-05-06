CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -ggdb

all: server client

server: server.cpp
	$(CC) $(CFLAGS) server.cpp WordLibrary.cpp -o bin/server

client: client.cpp
	$(CC) $(CFLAGS) client.cpp -o bin/client

clean:
	rm bin/*.o

runserver: bin/server
	./bin/server

runclient: bin/client
	./bin/client