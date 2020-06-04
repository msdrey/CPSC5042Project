#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <mutex>

#include "Network.h"
#include "GameSession.h"
#include "Connection.h"

#define AUDREYS_PORT 12119      // Audrey's port on cs1 for cpsc5042

/**
 * The Server class is responsible for accepting new connections and spawning threads.
 * It holds the details of the established server socket and makes the connections between 
 * each thread's: 
 * 				- pointer to the global Network singleton object
 * 				- unique Connection object used for communiction to the unique client socket
 * 				- unique GameSession object to manage Game functions and state
 */
class Server {
private:
    int server_fd; 				// the server's socket file descriptor 
	struct sockaddr_in address; // the address information of the server socket
	int addrlen; 				// the length of the address
	int port;

  	static void *startNewGame(void *); //starts new threads for clients

public:	
	Server(int, const char**);
	~Server();
	int acceptConnection();
    void acceptConnections();
};

#endif //SERVER_H