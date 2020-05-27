#ifndef WORDLIBRARY_WORDLIBRARY_H
#define WORDLIBRARY_WORDLIBRARY_H
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class WordLibrary{
    public:
        WordLibrary();
        string getWord(int);
        string getHint(int);
        void addWord(string);
    private:
        vector<string> library;
        void shuffle();
};

#endif //WORDLIBRARY_WORDLIBRARY_H