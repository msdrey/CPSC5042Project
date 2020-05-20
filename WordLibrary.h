#ifndef WORDLIBRARY_WORDLIBRARY_H
#define WORDLIBRARY_WORDLIBRARY_H
#include <iostream>

using namespace std;

class WordLibrary{
    public:
        WordLibrary();
        string getWord(int);
        string getHint(int);
        void addWord(string);
    private:

        int count = 36;
    
        // struct wordCard{
        //     string word;
        //     string hint;
       // };

        string *library = new string[count];
        void shuffle();

};


#endif //WORDLIBRARY_WORDLIBRARY_H