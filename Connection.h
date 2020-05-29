#ifndef CONNECTION_H
#define CONNECTION_H

#include <unistd.h> 
#include <netinet/in.h> 
#include <string>
#include <iostream> 

using namespace std;

class Connection{
    private:
        int socket;     
        int currentUserIndex;

    public:
        Connection(int);
	    void disconnectClient();
        void setCurrentUser(int);
        int getCurrentUser();
        string receive();
        void sendToClient(const string &);
};

#endif //CONNECTION_H