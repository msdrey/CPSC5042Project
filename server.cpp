// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

// Uncomment this for string class
#include <iostream> 
#include <string>
using namespace std;

//audrey's port on cs1 for cpsc5042
#define PORT 12119

class Connection {
  public:
	int server_socket;
	struct sockaddr_in address;	
	int addrlen;


	Connection() {
		// Creating socket file descriptor 
		int opt = 1; 
		server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket < 0) 
		{ 
			throw "Server's socket creation failed"; 
		} 

		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) 
		{ 
			throw "setsockopt failed"; 
		}

		addrlen = sizeof(address); 
		
		address.sin_family = AF_INET; 
		address.sin_addr.s_addr = INADDR_ANY; 
		address.sin_port = htons( PORT ); 

		// Forcefully attaching socket to the port 12119 
		if (bind(server_socket, (struct sockaddr *)&address, 
									sizeof(address))<0) 
		{ 
			throw "bind failed"; 
		} 

		//prepare to accept connections
		if (listen(server_socket, 3) < 0) 
		{ 
			throw "listen failed"; 
		} 

		cout << "Waiting for a client to connect." << endl;
	}

	int connect() {
		//wait for a connection on the server_socket socket.
		//when a connection occurs, create a new socket 
		// for the client to connect with the server's socket
		int new_socket = accept(server_socket, (struct sockaddr *)&address, 
						(socklen_t*)&addrlen);
		if (new_socket < 0) 
		{ 
			throw "accept failed";
		} 

		cout << "A client is playing the game." << endl;
		return new_socket;
	}

	void closeServerSocket() {
		close(server_socket);
	}
};

class GameSession {
  private:  
    
    string currentWord;
    string currentClue;
    int score;
    int currentStreak;
	int bestStreak;
	int status; //1 if the game is ongoing, 0 if the client decides to quit

    void selectWord() {
        //to do!
        currentWord = "potato";
        currentClue = "A hearty root vegetable";
    }

    //check if client's input is a command or not. 
    bool isCommand(string str) {
        return str[0] == '.';
    }

	//handling commands
	string handleCommand(string str) {
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

	// returns true if the two strings match
	bool isAMatch(string str1, string str2) {
		return str1.compare(str2) == 0;
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

	string checkGuess(string guess) {
		if (isAMatch(guess, currentWord)) {
			string win = "Congrats, you win!\n";
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
	// Constructor
    GameSession() {
        score = 0;
        currentStreak = 0;
		bestStreak = 0;
		status = 1;
        selectWord();
    }

	string startSession() {
		string welcome = "Welcome to Wordasaurus!\n" ;
		welcome += "This is a guessing word game. Just type your best guess!\n";
		welcome += displayCommands();
		return welcome + promptWord();
	}

	string displayCommands() {
		string result = "Options:\n";
		return result 	+ "  .skip \t to skip the current word\n"
						+ "  .score \t to display the current score and best streak\n"
						+ "  .help \t to display commands again\n"
						+ "  .exit \t to log out and exit\n\n";
	}

	string handleUserInput(string userInput) {
		if (isCommand(userInput)) {
			return handleCommand(userInput);
		} else {
			return checkGuess(userInput);
		}		
	}

	bool getStatus() {
		return status;
	}

	void setStatus(int s) {
		status = s;
	}
	

};

//Second RPC
string receive(int socket) {
	char userInput[1024] = {0};
	int valread = recv(socket, userInput, 1024, 0);
	// cout << "valread: " << valread << endl;
	if (valread == 0) {
		return "";
	}
	return string(userInput);
}

//third rpc
void sendToClient(int socket, string message) {
	int valsend = send(socket, message.c_str(), message.length(), 0);
	// cout << "valsend = " << valsend << endl;
	if (valsend == -1) {
        throw "error occured while sending data to server";
    }
}

int main(int argc, char const *argv[]) 
{ 	
	//creating a socket
	Connection * connection = new Connection();

	//infinite loop to keep the server running indefinitely
	while (1) {
		try{

			// establishing connection with a client		
			int socket = connection->connect();
			cout << "post connection check. socket = " << socket << endl;

			//set up a new game session
			GameSession * thisSession = new GameSession();

			//welcome user and start game
			sendToClient(socket, thisSession->startSession());

			string userInput;
			while (thisSession->getStatus() == 1) { //while session is active

				//receive client's answer
				userInput = receive(socket);
				// cout << "userInput : " << userInput << endl;
				if (userInput.empty()) {
					cout << "The client has unexpectedly disconnected" << endl;
					thisSession->setStatus(0);
					break;
				}

				//handle client's answer and send feedback
				sendToClient(socket, thisSession->handleUserInput(userInput));
			}

			close(socket);
			cout << "The client exit the game." << endl << endl;

		} catch (const char* message) {
			cerr << message << endl;
			exit(EXIT_FAILURE);
		}
	}

	network->closeServerSocket();

	return 0; 
} 
