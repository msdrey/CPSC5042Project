/**
*	This class responsibles for the game logic. It takes the user input and executes wheather it is the command or user guess, then return
*	the correspondence.
*
*	Audrey Morin, Noah Glusenkamp, Nitid Comvandee
*/

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
	bool status; 			// set to true if the game is ongoing, false if the client decides to quit

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
	void setStatus(bool s);
	int getScore();
	int getBestStreak();
	
	static bool isAMatch(const string& , const string& );
	
};

#endif //GAMESESSION_H