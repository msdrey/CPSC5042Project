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

int create_connection() {
	int server_fd, new_socket; 
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
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
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
	new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen);
	if (new_socket < 0) 
	{ 
		printf("\n error \n");
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

	return new_socket;
}

class Session {
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

    string getHint() {
        string result = "";
        for (int i = 0; i < (int) currentWord.length(); i++) {
            result += "__ ";
        }
        result += ": " + currentClue;
        return result;
    }

    //check if client's input is a command or not. If it is, handle it.
    bool handleCommand() {
        //todo
		return 0;
    }

	bool isAMatch(string guess) {
		return guess.compare(currentWord) == 0;
	}

	string promptWord() {
		return "Guess the word: " + getHint() + "\n";
	}

	string displayScore() {
		string result = "Your score: " + to_string(score);
		result += "\n";
		result += "Your best streak: " + to_string(bestStreak);
		result += "\n";
		return result;
		// return "Your score: " + " \nYour best streak: " + getStreak() + "\n";
	}

	void updateBestStreak() {
		if (currentStreak > bestStreak) {
			bestStreak = currentStreak;
		}
	}

  public:
    Session() {
        score = 0;
        currentStreak = 0;
		bestStreak = 0;
        selectWord();
    }

	string startSession() {
		return "Welcome to Wordasaurus!\n" + promptWord();
	}

	string checkGuess(string guess) {
		if (isAMatch(guess)) {
			string win = "Congrats, you win!\n";
			selectWord();
			score += 1;
			currentStreak += 1;
			updateBestStreak();
			return win + displayScore()
					   + "Let's try a new word. \n"
					   + promptWord();
		} else {
			currentStreak = 0;
			return "Nope, wrong word. Try again. \n" 
					+ promptWord();
		}
	}

};

int main(int argc, char const *argv[]) 
{ 	
	
	int new_socket = create_connection();
	//the connection is now established.
	
	// //testing communication.
	// string hello = "This message was sent by the server";
	// char buffer[1024] = {0}; 
	// //read N bytes from the client's socket and puts it in the buffer
	// int valread = recv(new_socket, buffer, 1024, 0); 
	// if (valread == -1) {
	// 	printf("\n error \n");
	// 	exit(EXIT_FAILURE);
	// }
	// //print the client's message
	// printf("%s\n", buffer); 
	// //send N bytes of the message hello into the client's socket
	// send(new_socket, hello.c_str(), hello.length(), 0); 
	// //prints a confirmation that a message was sent to the client
	// printf("A message was sent to the client.\n"); 


	//game starts here! welcome user and prompt first word.
    Session * thisSession = new Session();
	string newSessionText = thisSession->startSession();
	send(new_socket, newSessionText.c_str(), newSessionText.length(), 0);

	while(true) {
		//recieve client's answer into the "guess" variable
		char guess[1024] = {0};
		int valread = recv(new_socket, guess, 1024, 0);
		if (valread == -1) {
			cout << endl << "error" << endl;
			exit(EXIT_FAILURE);
		}

		//check client's answer and send feedback
		string feedback = thisSession->checkGuess(string(guess));
		send(new_socket, feedback.c_str(), feedback.length(), 0);
	}

	return 0; 
} 
