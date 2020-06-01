#include "InterfaceManager.h"

void display(string str, string color) {//add color param
    string colorCommand = "\033[0;" + colorCode.at(color) + "m";
    cout << colorCommand << str << "\033[0m" << endl;
}

string takeInput() {
    string ans;
    getline(cin, ans);
    return ans;
}

string takeNewWord(){
    clearScreen();
    string userWord;
    string userHint;
    display("Enter a word you want to add(with no space): ", "cyan");
    userWord = takeInput();
    display("Enter hint for the word: ", "cyan");
    userHint = takeInput();
    return userWord + "," + userHint;
}

// prompt the user for a username and password and send it to server
string promptUserAuthentication() {
    string choice;
    bool validInput = false;
    while(!validInput) {
        display("Please enter 'sign up' if you are a new user or 'log in' if you are a returning user", "yellow");
        choice = takeInput();
        if ((!isAMatch(choice, "log in"))&&(!isAMatch(choice, "sign up"))) {
            display("This is not a valid choice. Try again.", "red");
        } else {
            validInput = true;
        }
    }

    display("Please enter your username: ", "yellow");
    string username = takeInput();
    display("Please enter your password: ", "yellow");
    string password = takeInput();

    return serializeAuthString(choice, username, password);
}

void clearScreen() {
    system("clear");
}

string serializeAuthString(string choice, string username, string password) {
    string result = choice + ";";
    result += "username=" + username;
    result += ",password=" + password;
    return result;
}

bool isAMatch(const string& str1, const string& str2) {
    unsigned int len = str1.length();
    if (str2.length() != len){
        return false;
    }
    for (unsigned int i = 0; i < len; ++i) {
        if (tolower(str1[i]) != tolower(str2[i])) {
            return false;
        }
    }
    return true;
}

void pressAnyKeyRoutine() {
    string dummy;
    display("Press enter to continue.", "yellow");
    getline(cin, dummy);
}

string errorMessage(int authStatus) {
    switch(authStatus){
        case -1: 
            return "Incorrect username. Disconnecting...";
        case -2: 
            return "Incorrect password. Disconnecting...";     
        case -3: 
            return "User already exists. Disconnecting...";
        case -4: 
            return "User is already logged in. Disconnecting...";
        default:
            return "auth error";
    }
}
