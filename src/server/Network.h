#ifndef NETWORK_H
#define NETWORK_H

#include <fstream>
#include <string>
#include <iostream> 
#include <vector>

#include "User.h"

// Note that server must be started from project directory, like so: 
// "./bin/server" for these paths to be found
#define USER_BANK_FILE_PATH "data/UserBank.txt"
#define WORDS_AND_HINTS_FILE_PATH "data/WordsAndHints.txt"

using namespace std;

/*
* The Network class 
* A single Network object is held by the Server and distributed 
* to all threads so they can access common data such as the user bank 
* and the leaderboard.
*
* All data here must be protected with mutexes since it will be 
* accessed by multiple concurrent threads.
*/
class Network{
    private:
        int socket;             // momentarily holds a new socket number before thread creation, then hand to Connection object in a thread
        vector<User> users;     // the bank of users

        //WARNING: when locking both the userbank file and the userbank vector, 
        //         to avoid deadlocks, always lock and unlock in the same order
        //         in this project, we always lock the file before the vector
        //         and unlock the vector before the file.
        pthread_mutex_t userbankfile_lock;          // the lock for the user bank text file
        pthread_mutex_t userbankvector_lock;        // the lock for the user vector
        
        pthread_mutex_t wordsandhints_lock;         // the lock for the words and hints text file

    public:
        pthread_mutex_t network_socket_lock;        // the lock for the new socket number, unlocked once thread Connection object has it

        Network();
        void setSocket(int);  
        int getSocket();
        int checkAuthentication(string);
        int createNewUser(string, string);
        int validateUsernamePassword(string, string);
        vector<string> * getWordsAndHints();
        void addWord(string);
        string getLeaderBoard();
        string getHighScore(int);
        void logOutUser(int);
        void updateUserScores(int, int, int);

        static bool isAMatch(const string& , const string& );
};

#endif //NETWORK_H