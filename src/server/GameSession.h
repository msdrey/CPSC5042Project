#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <iostream> 
#include <string>

#include "WordLibrary.h"

using namespace std;

/**
 * The Game Session manages the state of gameplay for each client. 
 */
class GameSession {
  private:  
    WordLibrary *wordBank;	// this thread's unique instance of the Word Library
    string currentWord;		// the current word
    string currentClue;		// the current hint
    int score; 				// number of correct guesses since the beginning of the session
    int currentStreak; 		// current streak of correct guesses by the player
	int bestStreak; 		// greatest streak of correct guesses in the current game session
	int status; 			// set to 1 if the game is ongoing, 0 if the client decides to quit

    void selectWord();
    bool isCommand(const string& );
	string handleCommand(const string& );
	
	string promptWord();
	string getHint();
	string displayScore();
	void updateBestStreak();
	string checkGuess(const string&);
	string displayCommands();

  public:
    GameSession(vector<string>*);
	string startSession() ;
	string handleUserInput(const string& );
	bool getStatus();
	void setStatus(int s);
	int getScore();
	int getBestStreak();
	
	static bool isAMatch(const string& , const string& );
	
};

#endif //GAMESESSION_H