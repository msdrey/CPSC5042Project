#include <iostream>
#include <string>
#include "WordLibrary.h"
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

WordLibrary :: WordLibrary(){

    ifstream infile;
    infile.open("WordsAndHints.txt");

    if (infile.is_open())
    {
        string line;

        while (getline(infile, line))
        {
            library.push_back(line);
        }
    }

    infile.close();

    shuffle();
 
}

string WordLibrary::getWord(int index){

    int comma = library.at(index).find(",");
    return library.at(index).substr(0, comma);
}

string WordLibrary::getHint(int index){
    int comma = library.at(index).find(",");
    return library.at(index).substr(comma + 1, library.at(index).length() - 1);
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

void WordLibrary :: addWord(string userWordHint){

    ofstream outfile;
    outfile.open("WordsAndHints.txt", ios::app);
    outfile << userWordHint << endl;

    outfile.close();

    // Add to the vector 

    //Keep tracks of the sequence in vector

    // Handle boundary

}