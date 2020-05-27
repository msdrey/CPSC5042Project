#ifndef NETWORK_H
#define NETWORK_H

/*
* A unique Connection object is held by the Network and distributed 
* to all Threads so they can access common data such as the userBank 
* and the leaderboard.
*
* All data here must be protected with mutexes since it will be 
* accessed by multiple concurrent threads.
*/

#include <fstream>
#include <string>
#include <iostream> 
#include <vector>

using namespace std;

class Network{
    private:
        int socket;

        struct User { // a simple struct for keeping users information. 
                    // may be expanded later with high score or other info
                    // may be turned into a full class too
            string username;
            string password;
        };

        vector<User> users; //the bank of users
        //leaderboard;

    public:
        pthread_mutex_t socket_lock;

        Network();
        void setSocket(int);
        int getSocket();
        int checkAuthentication(string);
        int createNewUser(string, string);
        int validateUsernamePassword(string , string);
	    
};

#endif //NETWORK_H