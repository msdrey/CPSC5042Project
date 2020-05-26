#include "Network.h"

using namespace std;

// class constructor sets up the server and initializes users bank
Network::Network(int argc, char const *argv[]) {
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
Network::~Network() {
    //delete[] users;
    // join each thread in vector
    // program will terminate prematurely if thise step is not done
    //for(auto& t : threads) {
        //t.join();
    //}
}

// this call is accepting a connection from a client and returning the 
// id of the socket of the new client connection
int Network::acceptConnection() {
    int newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (newSocket < 0) 
    { 
        throw "accept failed";
    } 
    cout << "New connection was made." << endl;
    return newSocket;
}

void Network::acceptConnections() {
    //infinite loop to keep the server running indefinitely 
    Connection * connectionPtr = new Connection();
	while (1) {
		try{
			// establish connection with a client		
			int newSocket = this->acceptConnection();
            connectionPtr->setSocket(newSocket);
            // start a new thread for the client
            pthread_t p1;
            //lock mutex here
            pthread_create(&p1, NULL, startNewGame, (void *) connectionPtr);
		} catch (const char* message) {
			cerr << message << endl;
			exit(EXIT_FAILURE);
		}
	}
}

void *Network::startNewGame(void * arg) {
    Connection * connection = (Connection *) arg;
    ThreadContext * threadContext = new ThreadContext(connection->getSocket());
    //unlock race condition mutex here

    string authInfo = threadContext->receive();
    //cout << "authInfo: " << authInfo << endl;
    int authResult = connection->checkAuthentication(authInfo);
    threadContext->sendToClient(Network::serializeKeyValuePair("isValidLogin", to_string(authResult)));
    if (authResult > -1) {
        cout << "User is authenticated" << endl;
        threadContext->setCurrentUser(authResult);
        string clientConfirmsAuth = threadContext->receive();
        cout << "Did client confirm authentication? " << clientConfirmsAuth << endl;
    } else {
        threadContext->disconnectClient();
        return NULL;
    }
    
	//set up a new game session
	GameSession * thisSession = new GameSession();

	//welcome user and start game
	threadContext->sendToClient(thisSession->startSession());

	string userInput;
	while (thisSession->getStatus() == 1) { //while session is active

		//receive client's answer
		userInput = threadContext->receive();
		// cout << "userInput : " << userInput << endl;
		if (userInput.empty()) {
			cout << "The client has unexpectedly disconnected" << endl;
			thisSession->setStatus(0);
			break;
		}

		//handle client's answer and send feedback
		threadContext->sendToClient(thisSession->handleUserInput(userInput));
	}

	threadContext->disconnectClient();
    return NULL;
}

