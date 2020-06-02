CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -ggdb -lpthread

all: server client

server: ServerMain.cpp WordLibrary.cpp Server.cpp GameSession.cpp Network.cpp Connection.cpp
	$(CC) $(CFLAGS) ServerMain.cpp WordLibrary.cpp Server.cpp GameSession.cpp Network.cpp Connection.cpp -o bin/server

client: Client.cpp InterfaceManager.cpp
	$(CC) $(CFLAGS) Client.cpp InterfaceManager.cpp -o bin/client

clean:
	rm bin/*.o