// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include "Network.h"
#include "GameSession.h"

#include <iostream> 
#include <string>
using namespace std;

int main(int argc, char const *argv[]) 
{ 	
	//creating a socket
	Network * network = new Network(argc, argv);

	//infinite loop to keep the server running indefinitely 
	while (1) {
		try{
			// establish connection with a client		
			network->connect();
			cout << "post connection check. socket = " << network->getCurrentClientSocket() << endl;

			// authenticate client that created connection
			// authStatus: 0 --> success
			//             -1 --> user not found
			//             -2 --> wrong password
			//             -3 --> user already exists
			int authStatus = network->receiveAndCheckAuthentication();
			network->sendToClient(Network::serializeKeyValuePair("isValidLogin", to_string(authStatus)));
			if (authStatus == 0) {
				cout << "User is authenticated" << endl;
				string clientConfirmsAuth = network->receive();
				cout << "Did client confirm authentication? " << clientConfirmsAuth << endl;
			} else {
				network->disconnectClient(); 
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

			network->disconnectClient();

		} catch (const char* message) {
			cerr << message << endl;
			exit(EXIT_FAILURE);
		}
	}

	//network->disconnect();

	return 0; 
} 
