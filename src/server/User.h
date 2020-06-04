/**
*   User object, to contain user information for identifying player in server.
*
*   Audrey Morin, Noah Glusenkamp, Nitid Comvandee
*/
#ifndef USER_H
#define USER_H

#include <string>

using namespace std;


struct User { // a simple struct for keeping users information. 
                    
    string username;    
    string password;    
    int highestScore;  
    int highestStreak;
    bool isLoggedIn;
};
#endif //USER_H