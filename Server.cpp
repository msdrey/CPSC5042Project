#include "Server.h"

using namespace std;

// class constructor sets up the server
Server::Server(int argc, char const *argv[]) {
    if (argc == 2) {
        port = atoi(argv[1]);
    } else {
        port = AUDREYS_PORT;
    }

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
    address.sin_port = htons( port ); 

    // Forcefully attaching socket to the port  
    if (::bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    { 
        throw "bind failed";
    } 

    //prepare to accept connections
    if (listen(server_fd, 3) < 0) 
    { 
        throw "listen failed"; 
    } 
    cout << "Server is listening on port " << port << endl;
}

// class destructor
Server::~Server() {
}

// this call is accepting a connection from a client and returning the 
// id of the socket of the new client connection
int Server::acceptConnection() {
    int newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (newSocket < 0) 
    { 
        throw "accept failed";
    } 
    cout << "New connection was made." << endl;
    return newSocket;
}

void Server::acceptConnections() {
    //infinite loop to keep the server running indefinitely 
    Network * networkPtr = new Network();
	while (1) {
		try{
			// establish connection with a client		
			int newSocket = this->acceptConnection();
            
            // lock mutex here
            pthread_mutex_lock(&networkPtr->socket_lock);
            // cout << "before sleep..." << endl;
            // sleep(20);
            // cout << "after sleep..." << endl;
            
            // temporarily set network's socket to the new socket
            networkPtr->setSocket(newSocket);
            
            // start a new thread for the client
            pthread_t p1;
            pthread_create(&p1, NULL, startNewGame, (void *) networkPtr);
		
        } catch (const char* message) {
			cerr << message << endl;
			exit(EXIT_FAILURE);
		}
	}
}

void *Server::startNewGame(void * arg) {
    Network * network = (Network *) arg; // the shared data
    Connection * connection = new Connection(network->getSocket()); //the thread-specific data
    //unlock race condition mutex here
    pthread_mutex_unlock(&network->socket_lock);

    //receive client's authentication info: log in or sign up, username and pw
    string authInfo = connection->receive();
    // ask the connection object to validate authentication info
    int authResult = network->checkAuthentication(authInfo);
    // send the result back to the client
    connection->sendToClient(Server::serializeKeyValuePair("isValidLogin", to_string(authResult)));
    
    if (authResult > -1) { //successful authentication. Handshake from client.
        cout << "User is authenticated" << endl;
        connection->setCurrentUser(authResult);
        string clientConfirmsAuth = connection->receive();
        cout << "Did client confirm authentication? " << clientConfirmsAuth << endl;
    } else { // authentication failed. Disconnect and force exit thread.
        connection->disconnectClient();
        return NULL;
    }
    
	//set up a new game session
	GameSession * thisSession = new GameSession();

	//welcome user and start game
	connection->sendToClient(thisSession->startSession());

	string userInput;
	while (thisSession->getStatus() == 1) { //while session is active

		//receive client's answer
		userInput = connection->receive();
		// cout << "userInput : " << userInput << endl;
		if (userInput.empty()) {
			cout << "The client has unexpectedly disconnected" << endl;
			thisSession->setStatus(0);
			break;
		}
        // get a struct or something from connection->someNewFunction(); getScores or something
        //user ID retrieved from connection
        // Network->updateLeaderboard(the struct of scores, streaks, packaged with current user ID)



		//handle client's answer and send feedback
		connection->sendToClient(thisSession->handleUserInput(userInput));
	}

	connection->disconnectClient();
    return NULL;
}

