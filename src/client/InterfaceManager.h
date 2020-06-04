#ifndef INTERFACEMANAGER_H
#define INTERFACEMANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <fstream>

// Note that client must be started from project directory, like so: 
// "./bin/client" for this path to be found
#define DINO_FILE_PATH "data/dino_color.txt"

using namespace std;

const map<string, string> colorCode = { 
    {"red", "31"}, 
    {"green", "32"}, 
    {"yellow","33"},
    {"blue","34"},
    {"magenta","35"},
    {"cyan","36"},
    {"white","37"}};


//unit methods
void display(string str, string color = "white");
void displayLogo();
void clearScreen();
string takeInput();
string serializeAuthString(string, string, string);


//routine methods
string takeNewWord();
string promptUserAuthentication();
void pressAnyKeyRoutine();

//helper 
bool isAMatch(const string&, const string&);
string errorMessage(int);

#endif //INTERFACEMANAGER_H