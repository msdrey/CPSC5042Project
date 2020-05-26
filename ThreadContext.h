#ifndef THREADCONTEXT_H
#define THREADCONTEXT_H

#include <fstream>
#include <unistd.h> 
#include <netinet/in.h> 
#include <string>
#include <iostream> 


using namespace std;

class ThreadContext{
    private:
        int socket;

        struct User { // a simple struct for keeping users information. 
                    // may be expanded later with high score or other info
                    // may be turned into a full class too
            string username;
            string password;
        };
        int currentUserIndex;

    public:
        ThreadContext(int);
	    void disconnectClient();
        void setCurrentUser(int);
        string receive();
        void sendToClient(const string &);
};

#endif //THREADCONTEXT_H