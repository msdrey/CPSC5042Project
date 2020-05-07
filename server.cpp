// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include "WordLibrary.h"

#include <iostream> 
#include <string>
using namespace std;

// Audrey's port on cs1 for cpsc5042
#define PORT 12119
#define USER_CAPACITY 100

// This class holds the details of the established server socket and its
// address and allows the server to create a new socket, 
class Network {
  private:
    int server_fd; //the server's socket
	struct sockaddr_in address; // the address information of the server socket
	int addrlen; // the length of the address

	struct User { // a simple struct for keeping users information. 
				  // may be expanded later with high score or other info
				  // may be turned into a full class too
		string username;
		string password;
	};

	User * users; //the bank of users
	int currentUserIndex; //the index where we can find the information of the 
						  //user that is currently connected

  public:
	int currentSocket; // holds the socket once a connection is established
	// class constructor sets up the server and initializes users bank
	Network() {

		//initializing the bank of users
		users = new User[USER_CAPACITY];
		users[0].username = "asdf";
		users[0].password = "qwer";
		users[1].username = "noah";
		users[1].password = "zxcv";
		users[2].username = "ken";
		users[2].password = "zxcv";
		users[3].username = "audrey";
		users[3].password = "zxcv";
		users[4].username = "mikemckee";
		users[4].password = "5042";
		users[5].username = "default";
		users[5].password = "123";

		//creating a listening socket
		int opt = 1; 
		addrlen = sizeof(address); 
		
		// Creating socket file descriptor 
		server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd < 0) 
		{ 
			throw "Server's socket creation failed"; 
		} 

		//setting up the options for the socket
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) 
		{ 
			throw "setsockopt failed";
		} 

		//specifying the address and the port the server will connect to
		address.sin_family = AF_INET; 
		address.sin_addr.s_addr = INADDR_ANY; 
		address.sin_port = htons( PORT ); 

		// Forcefully attaching socket to the port 12119 
		if (::bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
		{ 
			throw "bind failed";
		} 

		//prepare to accept connections
		if (listen(server_fd, 3) < 0) 
		{ 
			throw "listen failed"; 
		} 
		cout << "Server is listening on port " << PORT << endl;
	}

	// class destructor
	~Network() {
		delete[] users;
	}

	// First RPC
	// this call is accepting a connection from a client and returning the 
	// id of the socket of the new client connection
	void connect() {
		int newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (newSocket < 0) 
		{ 
			throw "accept failed";
		} 
		cout << "New connection was made." << endl;
		currentSocket = newSocket;
		return;
	}

	// Second RPC: receives whatever is found in the specified socket
	//    		  and returns it
	string receive() {
		char userInput[1024] = {0};
		int valread = recv(currentSocket, userInput, 1024, 0);
		// cout << "valread: " << valread << endl;
		if (valread == 0) {
			return "";
		}
		return string(userInput);
	}

	// Third RPC: sends the inputted message into the specified socket
	//           will throw an error message if the sending fails
	void sendToClient(const string& message) {
		int valsend = send(currentSocket, message.c_str(), message.length(), 0);
		// cout << "valsend = " << valsend << endl;
		if (valsend == -1) {
			throw "error occured while sending data to server";
		}
	}

	// Fourth RPC: closes the socket and confirms closure into console
	void disconnect() {
		close(currentSocket);
		cout << "The client exit the game." << endl << endl;
	}

	bool isAuthenticatedClient() {
		string authString = receive(); //"username=<value>,password=<value>"
		cout << authString << endl; 

		//extract username and entered password from authString
		int equalPos = authString.find("=");
		int commaPos = authString.find(",");
		string user = authString.substr(equalPos+1, commaPos - equalPos - 1);
		string passwordAttempt = authString.substr(commaPos+10);

		//find the inputted user in our users bank, if so, initilize currentUserIndex
		bool isFound = false;
		for (int i = 0; i < USER_CAPACITY && !isFound; i++) {
			if (users[i].username.compare(user) == 0)  {
				cout << "found user : " << user << endl;
				currentUserIndex = i;
				isFound = true;
			}
		}

		//if user is not found or if user is found but password is wrong, authentication fails
		if (!isFound || passwordAttempt.compare(users[currentUserIndex].password) != 0) {
			cout << "Auth fail for string: " << authString << endl;
			return false;
		} else {
			cout << "Auth success for string: " << authString << endl;
			return true;
		}
	}

	// helper static function that puts a key and value into a 
	// standardized format
	static string serializeKeyValuePair(string key, string value) {
		return key + "=" + value;
	}
};


//This class manages a game of guessing word. A game lasts as long as the player wants
//to continue playing and keeps asking the user to guess words according to some
//clues.
class GameSession {
  private:  
    WordLibrary *wordBank;
    string currentWord;
    string currentClue;
	int index; // index of the current word to be guessed in the word library
    int score; // number of correct guesses since the beginning of the session
    int currentStreak; // current streak of correct guesses by the player
	int bestStreak; // greatest streak of correct guesses in the current game session
	int status; //1 if the game is ongoing, 0 if the client decides to quit

    void selectWord() {
        currentWord = wordBank->getWord(index);
        currentClue = wordBank->getHint(index);
		index++; // go to next word in the list (the list has been randomized upon its creation)
    }

    //check if client's input is a command or not. 
    bool isCommand(const string& str) {
        return str[0] == '.';
    }

	//handling commands
	string handleCommand(const string& str) {
		if (isAMatch(str, ".skip")) {
			selectWord();
			currentStreak = 0;
			return "Let's try a different word. \n" + 
					promptWord();
		} else if (isAMatch(str, ".score")) {
			return displayScore() + "\n" + promptWord();
		} else if (isAMatch(str, ".exit")) {
			status = 0;
			return displayScore() + "\nThank you for playing! Goodbye.";
		} else if (isAMatch(str, ".help")){
			return displayCommands() + promptWord();
		} else {
			return "Invalid command. \n" + displayCommands() + promptWord();
		}
	}

	// returns true if the two strings match. Case insensitive.
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

	// returns a string prompting the user for an answer
	string promptWord() {
		return "Guess the word: " + getHint() + "\n";
	}

	// returns a formatted hint for the current word
	string getHint() {
        string result = "";
        for (int i = 0; i < (int) currentWord.length(); i++) {
            result += "__ ";
        }
        result += ": " + currentClue;
        return result;
    }

	// returns a string that displays the current score and streak
	string displayScore() {
		string result = "Your score: " + to_string(score);
		result += "\n";
		result += "Your best streak: " + to_string(bestStreak);
		result += "\n";
		return result;
	}

	// checks if current streak is better than best streak and
	// updates bestStreak
	void updateBestStreak() {
		if (currentStreak > bestStreak) {
			bestStreak = currentStreak;
		}
	}

	//checks the user's guess, modifies the state of the game accordingly
	// and returns an appropriate response to be sent to the user 
	string checkGuess(const string& guess) {
		if (isAMatch(guess, currentWord)) {
			string win = "\nCongrats, you win!\n";
			selectWord();
			score += 1;
			currentStreak += 1;
			updateBestStreak();
			return win + displayScore()
					+ "\nLet's try a new word. \n"
					+ promptWord();
		} else {
			currentStreak = 0;
			return "Nope, wrong word. Try again. \n" 
					+ promptWord();
		}
	}

  public:
	// Constructor initializes the game session's state
    GameSession() {
		wordBank = new WordLibrary();
		index = 0;
        score = 0;
        currentStreak = 0;
		bestStreak = 0;
		status = 1;
        selectWord();
    }

	// returns a welcoming message to the user
	string startSession() {
		string welcome = "\n\n\nWelcome to Wordasaurus!\n" ;
		welcome += "This is a guessing word game. Just type your best guess!\n";
		welcome += displayCommands();
		return welcome + promptWord();
	}

	// returns a formatted string explaining the available commands to the user
	string displayCommands() {
		string result = "Options:\n";
		return result 	+ "  .skip \t to skip the current word\n"
						+ "  .score \t to display the current score and best streak\n"
						+ "  .help \t to display commands again\n"
						+ "  .exit \t to log out and exit\n\n";
	}

	// determines if the user input is a command or a guess and calls the appropriate function
	string handleUserInput(const string& userInput) {
		if (isCommand(userInput)) {
			return handleCommand(userInput);
		} else {
			return checkGuess(userInput);
		}		
	}

	// returns the current status of the game session: 0 for inactive, 1 for active
	bool getStatus() {
		return status;
	}

	// sets the current status of the game session: 0 for inactive, 1 for active
	void setStatus(int s) {
		status = s;
	}
	
};

int main(int argc, char const *argv[]) 
{ 	
	//creating a socket
	Network * network = new Network();

	//infinite loop to keep the server running indefinitely
	while (1) {
		try{
			// establish connection with a client		
			network->connect();
			cout << "post connection check. socket = " << network->currentSocket << endl;

			// authenticate client that created connection
			if (network->isAuthenticatedClient()) {
				cout << "User is authenticated" << endl;
				network->sendToClient(Network::serializeKeyValuePair("isValidLogin", "true"));
				string clientConfirmsAuth = network->receive();
				cout << "Did client confirm authentication? " << clientConfirmsAuth << endl;
			} else {
				network->sendToClient(Network::serializeKeyValuePair("isValidLogin", "false"));
				// force disconnect on server side
				network->disconnect(); 
				// skip rest of while loop to keep server alive
				continue;	
			}

			//set up a new game session
			GameSession * thisSession = new GameSession();

			//welcome user and start game
			network->sendToClient(thisSession->startSession());

			string userInput;
			while (thisSession->getStatus() == 1) { //while session is active

				//receive client's answer
				userInput = network->receive();
				// cout << "userInput : " << userInput << endl;
				if (userInput.empty()) {
					cout << "The client has unexpectedly disconnected" << endl;
					thisSession->setStatus(0);
					break;
				}

				//handle client's answer and send feedback
				network->sendToClient(thisSession->handleUserInput(userInput));
			}

			network->disconnect();

		} catch (const char* message) {
			cerr << message << endl;
			exit(EXIT_FAILURE);
		}
	}

	//network->disconnect();

	return 0; 
} 
