#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <mutex>
#include "Network.h"
#include "GameSession.h"
#include "Connection.h"

// Audrey's port on cs1 for cpsc5042
#define AUDREYS_PORT 12119

using namespace std;
// This class holds the details of the established server socket and its
// address and allows the server to create a new socket, 
class Server {
private:
    int server_fd; //the server's socket
	  struct sockaddr_in address; // the address information of the server socket
	  int addrlen; // the length of the address
	  int port;

  	static void *startNewGame(void *); //starts new threads for clients


public:	
	  Server(int, const char**);
	  ~Server();
	  int acceptConnection();
    void acceptConnections();
	
	  // helper static function that puts a key and value into a 
    // standardized format
    static string serializeKeyValuePair(string key, string value) {
        return key + "=" + value;
    }
};

#endif //SERVER_H