
#include "WordLibrary.h"

/**
*   Constructor
*   Creates a copy of vector for every threads so that the reading from file will occur just once when the server is initiated.
*
*   @param *wordsAndHints A vector pointer which was passed from the Network class after reading from file.
*/
WordLibrary::WordLibrary(vector<string> * wordsAndHints){

    //make a local copy of the network's words and hints
    copy((*wordsAndHints).begin(), (*wordsAndHints).end(), back_inserter(library));

    shuffle();
    // start at beginning of vector
    currentWordIndex = 0;

}

/**
*   Retrieves the word from vector.
*
*   @return Word for guessing.
*/
string WordLibrary::getWord(){

    int comma = library.at(currentWordIndex).find(",");
    return library.at(currentWordIndex).substr(0, comma);
}

/**
*   Retrieves the hint from vector.
*
*   @return Hint for the guessing word.
*/
string WordLibrary::getHint(){
    int comma = library.at(currentWordIndex).find(",");
    return library.at(currentWordIndex).substr(comma + 1);
}

/**
*   Increments word index for the next word to be retrieved.
*/
void WordLibrary::next() {
    currentWordIndex++;

    // if you are at the end of the vector, loop to beginning of vector
    if((int)library.size() < currentWordIndex + 1 ) {
        currentWordIndex = 0;
    }

}

/**
*   Shuffles the vector so that words will be in randomize order.
*/
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