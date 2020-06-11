#include "GameSession.h"

using namespace std;

/**
*   Gets the word and hint from WordLibrary and stores in local variables
*/
void GameSession::selectWord() {
    currentWord = wordBank->getWord();
    currentClue = wordBank->getHint();
    wordBank->next();
}
/**
*   check if client's input is a command or not.
*
*   @return True if the input is command, false if not.
*/
bool GameSession::isCommand(const string& str) {
    return str[0] == '.';
}

/**
*   Handles the command from user.
*
*   @return The correspondence of each command.
*/
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

/**
*   Helper function for comparing user input and valid commands. Ignores case.
*
*   @param str1 User input
*   @param str2 Valid command
*   @return True if user input is the valid command.
*/
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

/**
*   Prompts user a message for guessing.
*
*   @return String message asking for user input.
*/
string GameSession::promptWord() {
    return "Guess the word: " + getHint() + "\n";
}

/**
*   Provides hint for user and number of word's characters in space format.
*
*   @return Hint for current word and number of word's characters.
*/

string GameSession::getHint() {
    string result = "";
    for (int i = 0; i < (int) currentWord.length(); i++) {
        result += "__ ";
    }
    result += ": " + currentClue;
    return result;
}

/**
*   Gets the current scores and currect best streak.
*
*   @return Current scores and streak.
*/
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

/**
*   Checks the user guess, updates scores and streak and return the appropriate feedback.
*
*   @param guess User guess
*   @return String message with appropriate feedback.
*/
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

/**
*   Provides user the valid commands and explanation.
*
*   @return String message with valid commands and explanation.
*/
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

/**
*   Constructor
*
*   @param wordsAndHints A string vector containing all words and hints which was passed from Network after reading from file.
*/
GameSession::GameSession(vector<string> * wordsAndHints) {
    wordBank = new WordLibrary(wordsAndHints);
    score = 0;
    currentStreak = 0;
    bestStreak = 0;
    status = true;
    selectWord();
}

/**
*   Welcome user.
*
*   @return String message with welcome note and prompt user for guessing.
*/
string GameSession::startSession() {
    string welcome = "Welcome to Wordasaurus!\n\n" ;
    welcome += "This is a guessing word game. Just type your best guess!\n\n";
    welcome += displayCommands();
    return welcome + "@" + promptWord();
}

/**
*   Determines if the user input is a command or a guess and calls the appropriate function
*
*   @param userInput 
*   @return The appropriate respond associate with user input.
*/
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

/**
*   Gets the current status of the game session.
*
*   @return True if active, false if inactive.
*/
bool GameSession::getStatus() {
    return status;
}

/**
*   sets the current status of the game session: false for inactive, true for active
*/
void GameSession::setStatus(bool s) {
    status = s;
}

/**
*   Gets user's score.
*
*   @return user's score.
*/
int GameSession::getScore() {
    return score;
}

/**
*   Gets user's best streak.
*
*   @return User's best streak.
*/
int GameSession::getBestStreak() {
    return bestStreak;
}
