#include "InterfaceManager.h"

/**
 * displays the inputted string using the inputted color
 * 
 * @param str the string to print to console
 * @param color a string containing the color the str should be 
 *              printed in. Choices are red, green, yellow, blue, 
 *              magenta, cyan or white.  
 */
void display(string str, string color) {//add color param
    string colorCommand = "\033[0;" + colorCode.at(color) + "m";
    cout << colorCommand << str << "\033[0m" << endl;
}

/**
 * displays to console the logo of the game
 */
void displayLogo() {
    ifstream logoFile(DINO_FILE_PATH);
    string result;
    char c;
    while (logoFile.get(c)) {
        if (c == 'Y') { // switch to yellow
            result += "\033[0;33m";
        } else if (c == 'G') { // switch to green
            result += "\033[0;32m";
        } else if (c == 'S') {
            result += "\033[0m"; // reset to white
        } else {
            result += c;
        }
    }
    logoFile.close();
    cout << result << endl;
}

/**
 * gets the first non-empty line from console and returns it. Ignores empty lines.
 * 
 * @return the non empty line the user inputs
 */
string takeInput() {
    string ans = "";
    while(ans.length() == 0) {
        getline(cin, ans);
    }
    return ans;
}

/**
 * On a cleared consoled, asks for a word and a hint and returns them 
 * formatted.
 * 
 * @return the inputted word and hint separated by a comma
 */
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

/**
 * interacts with the user on console to get his choice of log in or sign up, his
 * username and his password. Return all three combined in a formatted string
 * 
 * @return <'log in' or 'sign up'>;username=<inputted username>,password=<inputted password>
 */
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

/**
 * clears the text from the console
 */
void clearScreen() {
    system("clear");
}

/**
 * formats three wtrings together in one string for the server to understand
 * 
 * @param choice either "log in" or "sign up"
 * @param username the username
 * @param password the password
 * @return <'log in' or 'sign up'>;username=<inputted username>,password=<inputted password>
 */
string serializeAuthString(string choice, string username, string password) {
    string result = choice + ";";
    result += "username=" + username;
    result += ",password=" + password;
    return result;
}

/**
 * returns true if two strings are equal while ignoring case.
 * 
 * @param str1 the first string
 * @param str2 the second string
 * @return true if the strings are equal (ignoring case), false otherwise.
 */
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

/**
 * displays a "press any key to continue" message to console and
 * waits for the user to press "enter"
 */
void pressAnyKeyRoutine() {
    string dummy;
    display("Press enter to continue.", "yellow");
    getline(cin, dummy);
}

/**
 * returns an appropriate error message according to an interger authentication
 * status code
 * 
 * @param authStatus the code for the authentication status
 * @return the error message to display to the user
 */
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
