#ifndef WORDLIBRARY_WORDLIBRARY_H
#define WORDLIBRARY_WORDLIBRARY_H
#include <iostream>
#include <vector>

using namespace std;

class WordLibrary{
    public:
        WordLibrary();
        string getWord(int);
        string getHint(int);
        void addWord(string);
    private:

        int count = 36;

        vector<string> library;
        void shuffle();

};


#endif //WORDLIBRARY_WORDLIBRARY_H