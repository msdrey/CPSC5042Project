#include "WordLibrary.h"

WordLibrary::WordLibrary(vector<string> * wordsAndHints){

    //make a local copy of the network's words and hints
    copy((*wordsAndHints).begin(), (*wordsAndHints).end(), back_inserter(library));

    shuffle();
    index = 0;
 
}

string WordLibrary::getWord(){

    int comma = library.at(index).find(",");
    return library.at(index).substr(0, comma);
}

string WordLibrary::getHint(){
    int comma = library.at(index).find(",");
    return library.at(index).substr(comma + 1);
}

void WordLibrary::next() {
    index++;
}


void WordLibrary :: shuffle(){

    string temp;
    int index;
    int len = library.size();
    srand(time(NULL));

    for (int i = 0; i < len; i++)
    {
        index = rand() % len;
        temp = library.at(i);
        library.at(i) = library.at(index);
        library.at(index) = temp;
    }

}