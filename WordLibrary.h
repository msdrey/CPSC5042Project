#ifndef WORDLIBRARY_WORDLIBRARY_H
#define WORDLIBRARY_WORDLIBRARY_H
#include <iostream>

using namespace std;

class WordLibrary{
    public:
        WordLibrary();
        string getWord(int);
        string getHint(int);
    private:

        int const COUNT = 36;
    
        struct wordCard{
            string word;
            string hint;
        };

        wordCard *library = new wordCard[COUNT];
        void shuffle();

};


#endif //WORDLIBRARY_WORDLIBRARY_H