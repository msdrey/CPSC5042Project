#ifndef NETWORK_H
#define NETWORK_H
#include <unistd.h> 
//#include <sys/socket.h> 
//#include <stdlib.h> 
#include <netinet/in.h> 

#include <iostream> 
#include <string>

// Audrey's port on cs1 for cpsc5042
#define PORT 12119
#define USER_CAPACITY 100

using namespace std;
// This class holds the details of the established server socket and its
// address and allows the server to create a new socket, 
class Network {
  private:
    int server_fd; //the server's socket
	struct sockaddr_in address; // the address information of the server socket
	int addrlen; // the length of the address

	struct User { // a simple struct for keeping users information. 
				  // may be expanded later with high score or other info
				  // may be turned into a full class too
		string username;
		string password;
		int socket;
	};

	User * users; //the bank of users
	int currentUserIndex;
	int currentClientSocket;

  public:
	int getCurrentClientSocket(); 
	string getCurrentUser();	
	Network();
	~Network();
    void connect();
	string receive();
	void sendToClient(const string& );
	void disconnectClient();
	void closeServerSocket();
	bool receiveAndCheckAuthentication();
	bool userAuthentication(string , string);
	
    // helper static function that puts a key and value into a 
    // standardized format
    static string serializeKeyValuePair(string key, string value) {
        return key + "=" + value;
    }
};

#endif //NETWORK_H