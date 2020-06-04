#ifndef CONNECTION_H
#define CONNECTION_H

#include <unistd.h> 
#include <netinet/in.h> 
#include <string>
#include <iostream> 

using namespace std;

/**
 * The Connection class holds a unique socket number for each client and
 * manages the send and recieve back and forth messaging between server and client
 */
class Connection{
    private:
        int socket;             // unique socket number per client
        int currentUserIndex;   // index into Network's user bank for the user on this connection

    public:
        Connection(int);
	    void disconnectClient();
        void setCurrentUser(int);
        int getCurrentUser();
        string receive();
        void sendToClient(const string &);
};

#endif //CONNECTION_H