CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -ggdb

all: server client

server: Server.cpp
	$(CC) $(CFLAGS) Server.cpp WordLibrary.cpp Network.cpp GameSession.cpp -o bin/server

client: Client.cpp
	$(CC) $(CFLAGS) Client.cpp -o bin/client

clean:
	rm bin/*.o

runserver: bin/server
	./bin/server

runclient: bin/client
	./bin/client