
#include <iostream>
#include <string>
#include <map>

using namespace std;

const string LOGO = 
    " __          __           _                                       \n"
    " \\ \\        / /          | |                                      \n"
    "  \\ \\  /\\  / /__  _ __ __| | __ _ ___  __ _ _   _ _ __ _   _ ___  \n"
    "   \\ \\/  \\/ / _ \\| '__/ _` |/ _` / __|/ _` | | | | '__| | | / __| \n"
    "    \\  /\\  / (_) | | | (_| | (_| \\__ \\ (_| | |_| | |  | |_| \\__ \\ \n"
    "     \\/  \\/ \\___/|_|  \\__,_|\\__,_|___/\\__,_|\\__,_|_|   \\__,_|___/ \n\n";



//unit methods
void display(string str, string color = "white");
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
