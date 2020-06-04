#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

struct User { // a simple struct for keeping users information. 
                    // may be expanded later with high score or other info
                    // may be turned into a full class too
    string username;
    string password;
    int highestScore;
    int highestStreak;
    bool isLoggedIn;
};
#endif //USER_H