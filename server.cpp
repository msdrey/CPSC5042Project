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

class Network {
	public:
	  static int createListener() {
		int server_fd, newSocket; 
		cout << "Server is running. socket: " <<  newSocket << ", server_fd: " << server_fd << ", port: " << PORT << endl;;
		struct sockaddr_in address; 
		int opt = 1; 
		int addrlen = sizeof(address); 
		
		// Creating socket file descriptor 
		server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd < 0) 
		{ 
			perror("Server's socket creation failed"); 
			exit(EXIT_FAILURE); 
		} 

		// Forcefully attaching socket to the port 8080 
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) 
		{ 
			perror("setsockopt"); 
			exit(EXIT_FAILURE); 
		} 

		address.sin_family = AF_INET; 
		address.sin_addr.s_addr = INADDR_ANY; 
		address.sin_port = htons( PORT ); 

		// Forcefully attaching socket to the port 8080 
		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
		{ 
			perror("bind failed"); 
			exit(EXIT_FAILURE); 
		} 

		//prepare to accept connections
		if (listen(server_fd, 3) < 0) 
		{ 
			perror("listen"); 
			exit(EXIT_FAILURE); 
		} 

		//wait for a connection on the server_fd socket.
		//when a connection occurs, create a new socket for the client to connect with the server's socket
		newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (newSocket < 0) 
		{ 
			printf("\n error \n");
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 

		return newSocket;
	  }
};

class GameSession {
  private:  
    
    string currentWord;
    string currentClue;
    int score;
    int currentStreak;
	int bestStreak;

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
			// call disconnect from network class???
			return "todo";
		} else {
			return "Invalid command. \n" + promptWord();
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
        selectWord();
    }

	string startSession() {
		string welcome = "Welcome to Wordasaurus!\n" ;
		welcome += "This is a guessing word game. Just type your best guess!\n";
		welcome += "Options:";
		welcome += "\n  .skip \t to skip the current word ";
		welcome += "\n  .score \t to display the current score and best streak";
		welcome += "\n  .quit \t to log out and quit \n\n";
		return welcome + promptWord();
	}

	string handleUserInput(string userInput) {
		if (isCommand(userInput)) {
			return handleCommand(userInput);
		} else {
			return checkGuess(userInput);
		}		
	}

	

};

int main(int argc, char const *argv[]) {

	int newSocket = Network::createListener();
	//the connection is now established.

	//game starts here! welcome user and prompt first word.
    GameSession * thisSession = new GameSession();
	string newSessionText = thisSession->startSession();
	send(newSocket, newSessionText.c_str(), newSessionText.length(), 0);

	while(true) {
		//receive client's answer into the "guess" variable
		char userInput[1024] = {0};
		int valread = recv(newSocket, userInput, 1024, 0);
		if (valread == -1) {
			cout << endl << "error" << endl;
			exit(EXIT_FAILURE);
		}

		//check client's answer and send feedback
		string feedback = thisSession->handleUserInput(string(userInput));
		send(newSocket, feedback.c_str(), feedback.length(), 0);
	}

	return 0; 
} 
