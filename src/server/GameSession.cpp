#include "GameSession.h"

using namespace std;

void GameSession::selectWord() {
    currentWord = wordBank->getWord();
    currentClue = wordBank->getHint();
    wordBank->next();
}

//check if client's input is a command or not. 
bool GameSession::isCommand(const string& str) {
    return str[0] == '.';
}

//handling commands
string GameSession::handleCommand(const string& str) {
    if (isAMatch(str, ".skip")) {
        selectWord();
        currentStreak = 0;
        return "Let's try a different word. \n";
    } else if (isAMatch(str, ".score")) {
        return displayScore() + "\n";
    } else if (isAMatch(str, ".addWord")) {
		return "Your word has been added to the library.\nLet's continue playing.\n\n";
    } else if (isAMatch(str, ".leaderboard") ) {
        return "\n";
    } else if (isAMatch(str, ".highScore") ) {
        return "\n";
    } else if (isAMatch(str, ".help")){
        return displayCommands();
    } else {
        return "Invalid command. \n" + displayCommands();
    }
}

// returns true if the two strings match. Case insensitive.
bool GameSession::isAMatch(const string& str1, const string& str2) {
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

// returns a string prompting the user for an answer
string GameSession::promptWord() {
    return "Guess the word: " + getHint() + "\n";
}

// returns a formatted hint for the current word
string GameSession::getHint() {
    string result = "";
    for (int i = 0; i < (int) currentWord.length(); i++) {
        result += "__ ";
    }
    result += ": " + currentClue;
    return result;
}

// returns a string that displays the current score and streak
string GameSession::displayScore() {
    string result = "Your score: " + to_string(score);
    result += "\n";
    result += "Your best streak: " + to_string(bestStreak);
    result += "\n";
    return result;
}

// checks if current streak is better than best streak and
// updates bestStreak
void GameSession::updateBestStreak() {
    if (currentStreak > bestStreak) {
        bestStreak = currentStreak;
    }
}

//checks the user's guess, modifies the state of the game accordingly
// and returns an appropriate response to be sent to the user 
string GameSession::checkGuess(const string& guess) {
    if (isAMatch(guess, currentWord)) {
        string win = "\nCongrats, you win!\n";
        selectWord();
        score += 1;
        currentStreak += 1;
        updateBestStreak();
        return win + displayScore()
                + "\nLet's try a new word. \n";
    } else {
        currentStreak = 0;
        return "Nope, wrong word. Try again. \n";
    }
}

// returns a formatted string explaining the available commands to the user
string GameSession::displayCommands() {
    string result = "Options:\n";
    return result 	+ "  .skip \t to skip the current word\n"
                    + "  .score \t to display the current score and best streak in this session\n"
                    + "  .help \t to display commands again\n"
                    + "  .addWord \t to add your own word and its hint to our game\n"
                    + "  .leaderboard \t to show the leaderboard of the server\n"
                    + "  .highscore \t to display your highest score and highest streak records\n"
                    + "  .exit \t to log out and exit\n\n";
}


GameSession::GameSession(vector<string> * wordsAndHints) {
    wordBank = new WordLibrary(wordsAndHints);
    score = 0;
    currentStreak = 0;
    bestStreak = 0;
    status = 1;
    selectWord();
}

// returns a welcoming message to the user
string GameSession::startSession() {
    string welcome = "Welcome to Wordasaurus!\n\n" ;
    welcome += "This is a guessing word game. Just type your best guess!\n\n";
    welcome += displayCommands();
    return welcome + "@" + promptWord();
}

// determines if the user input is a command or a guess and calls the appropriate function
string GameSession::handleUserInput(const string& userInput) {
    string result;
    if (isAMatch(userInput,".exit")) {
            status = 0;
            return displayScore() + "\nThank you for playing! Goodbye.@";
    }

    if (isCommand(userInput)) {
        result = handleCommand(userInput);
    } else {
        result = checkGuess(userInput);
    }		

    return result + "@" + promptWord();
}

// returns the current status of the game session: 0 for inactive, 1 for active
bool GameSession::getStatus() {
    return status;
}

// sets the current status of the game session: 0 for inactive, 1 for active
void GameSession::setStatus(int s) {
    status = s;
}

int GameSession::getScore() {
    return score;
}

int GameSession::getBestStreak() {
    return bestStreak;
}
