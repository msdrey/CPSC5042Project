#include "Network.h"
#include "GameSession.h"
#include "Connection.h"

using namespace std;

// class constructor sets up the server and initializes users bank
Network::Network(int argc, char const *argv[]) {
    if (argc == 2) {
        port = atoi(argv[1]);
    } else {
        port = AUDREYS_PORT;
    }

    //initializing the bank of users
    users = new User[USER_CAPACITY];
    ifstream userbankfile("UserBank.txt");
    string line;
    int i = 0;
    while (i < USER_CAPACITY && getline(userbankfile, line)) {
        users[i].username = line;
        getline(userbankfile, line);
        users[i].password = line;
        i++;
    }
    userbankfile.close();
    usersCount = i;

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
    delete[] users;
    // join each thread in vector
    // program will terminate prematurely if thise step is not done
    for(auto& t : threads) {
        t.join();
    }
}

// this call is accepting a connection from a client and returning the 
// id of the socket of the new client connection
Connection * Network::acceptConnection() {
    int newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (newSocket < 0) 
    { 
        throw "accept failed";
    } 
    cout << "New connection was made." << endl;
    Connection * newConnectionPtr = new Connection(newSocket);
    return newConnectionPtr;
}

void Network::acceptConnections() {
    //infinite loop to keep the server running indefinitely 
	while (1) {
		try{
			// establish connection with a client		
			Connection * connectionPtr = this->acceptConnection();
			//cout << "post connection check. socket = " << this->getCurrentClientSocket() << endl; 
            ////////// TODO /////// FIX Compile error
            std::thread t(&Network::startNewGame, ref(connectionPtr));
            threads.push_back(std::move(t));
		} catch (const char* message) {
			cerr << message << endl;
			exit(EXIT_FAILURE);
		}
	}
}

void Network::startNewGame(Connection * connection) {
    // authenticate client that created connection
	// if (this->receiveAndCheckAuthentication()) {
	// 	cout << "User is authenticated" << endl;
	// 	connection->sendToClient(Network::serializeKeyValuePair("isValidLogin", "true"));
	// 	string clientConfirmsAuth = connection->receive();
	// 	cout << "Did client confirm authentication? " << clientConfirmsAuth << endl;
	// } else {
	// 	connection->sendToClient(Network::serializeKeyValuePair("isValidLogin", "false"));
	// 	// force disconnect on server side
	// 	connection->disconnectClient(); 
	// }

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

		//handle client's answer and send feedback
		connection->sendToClient(thisSession->handleUserInput(userInput));
	}

	connection->disconnectClient();
}

bool Network::createNewUser(string inputUser, string inputPass) {
    //add to file
    ofstream userbankfile;
    userbankfile.open("UserBank.txt", ios_base::app);//append to file
    if (userbankfile.is_open()) {
        userbankfile << "\n"<< inputUser << "\n" << inputPass;
        userbankfile.close();
    }
    
    //add to loaded userbank
    //todo: check if there is space for a new user
    //todo: make user capacity bigger if needed?
    //todo: check if user already there
    
    users[usersCount].username = inputUser;
    users[usersCount].password = inputPass;
    usersCount++;
    cout << "A new user signed up." << endl;
    return true;
}

bool Network::validateUsernamePassword(string inputUser, string inputPass) {
    //find the inputted user in our users bank, if so, initilize currentUserIndex
    bool isFound = false;
    for (int i = 0; i < USER_CAPACITY && !isFound; i++) {
        if (users[i].username.compare(inputUser) == 0)  {
            //cout << "found user : " << user << endl;
            currentUserIndex = i;
            isFound = true;
        }
    }

    //if user is not found or if user is found but password is wrong, authentication fails
    if (!isFound || inputPass.compare(users[currentUserIndex].password) != 0) {
        cout << "Auth fail " << endl; //for string: " << authString << endl;
        return false;
    } else {
        //cout << "Auth success " << endl;// for string: " << authString << endl;
        return true;
    }
}

bool Network::receiveAndCheckAuthentication(Connection *connection) {
    string loginOrSignup = connection->receive(); //"log in " or "sign in"
    //cout << "user choice: " << loginOrSignin << endl;

    //send handshake
    connection->sendToClient("ans");

    string authString = connection->receive(); //"username=<value>,password=<value>"
    //cout << authString << endl; 

    //extract username and entered password from authString
    int equalPos = authString.find("=");
    int commaPos = authString.find(",");
    string inputUser = authString.substr(equalPos+1, commaPos - equalPos - 1);
    string inputPass = authString.substr(commaPos+10);

    if (loginOrSignup.compare("log in")==0) {
        return validateUsernamePassword(inputUser, inputPass);
    } else { // "sign up"
        return createNewUser(inputUser, inputPass);
    }
}
