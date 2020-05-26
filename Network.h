#ifndef NETWORK_H
#define NETWORK_H
#include <unistd.h> 
//#include <sys/socket.h> 
//#include <stdlib.h> 
#include <netinet/in.h> 


#include <string>

#include <thread>
#include <mutex>
#include <vector>

#include "Connection.h"
#include "GameSession.h"
#include "ThreadContext.h"

// Audrey's port on cs1 for cpsc5042
#define AUDREYS_PORT 12119

using namespace std;
// This class holds the details of the established server socket and its
// address and allows the server to create a new socket, 
class Network {
  private:
    int server_fd; //the server's socket
	struct sockaddr_in address; // the address information of the server socket
	int addrlen; // the length of the address
	int port;

	//vector<std::thread> threads;
	static void *startNewGame(void *);
	
  public:	
	Network(int, const char**);
	~Network();
	Connection * acceptConnection();
    void acceptConnections();
	


	// helper static function that puts a key and value into a 
    // standardized format
    static string serializeKeyValuePair(string key, string value) {
        return key + "=" + value;
    }
};

#endif //NETWORK_H