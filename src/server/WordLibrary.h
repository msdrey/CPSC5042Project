/**
*   WordLibrary object, responsible as the actual library for distribute words and hints to user. 
*   
*   Audrey Morin, Noah Glusenkamp, Nitid Comvandee
*/

#ifndef WORDLIBRARY_WORDLIBRARY_H
#define WORDLIBRARY_WORDLIBRARY_H
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <algorithm> // for copy() and assign() 
#include <iterator> // for back_inserter

using namespace std;

class WordLibrary{
    public:
        WordLibrary(vector<string> *);
        string getWord();
        string getHint();
        void next();
    private:
        void shuffle();
        vector<string> library; // Vector containing all words and hints reading from file
        int currentWordIndex;   // index of the current word to be guessed in the word library
};

#endif //WORDLIBRARY_WORDLIBRARY_H