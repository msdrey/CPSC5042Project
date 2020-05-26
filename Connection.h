#ifndef CONNECTION_H
#define CONNECTION_H

#include <fstream>
#include <unistd.h> 
#include <netinet/in.h> 
#include <string>
#include <iostream> 
#include <vector>


#define USER_CAPACITY 100

using namespace std;

class Connection{
    private:
        int socket;

        struct User { // a simple struct for keeping users information. 
                    // may be expanded later with high score or other info
                    // may be turned into a full class too
            string username;
            string password;
        };

        vector<User> users; //the bank of users

    public:
        Connection();
        void setSocket(int);
        int getSocket();
        int checkAuthentication(string);
        int createNewUser(string, string);
        int validateUsernamePassword(string , string);
	    
};

#endif //CONNECTION_H