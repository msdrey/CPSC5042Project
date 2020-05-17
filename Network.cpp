#include "Network.h"

using namespace std;

// getter for current client's socket identifier
int Network::getCurrentClientSocket() {
    return currentClientSocket;
}

// class constructor sets up the server and initializes users bank
Network::Network() {

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
Network::~Network() {
    delete[] users;
}

// this call is accepting a connection from a client and returning the 
// id of the socket of the new client connection
void Network::connect() {
    int newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (newSocket < 0) 
    { 
        throw "accept failed";
    } 
    cout << "New connection was made." << endl;
    currentClientSocket = newSocket;
    return;
}

// receives whatever is found in the specified socket
// and returns it
string Network::receive() {
    char userInput[1024] = {0};
    int valread = recv(currentClientSocket, userInput, 1024, 0);
    // cout << "valread: " << valread << endl;
    if (valread == 0) {
        return "";
    }
    return string(userInput);
}

//sends the inputted message into the specified socket
// will throw an error message if the sending fails
void Network::sendToClient(const string& message) {
    int valsend = send(currentClientSocket, message.c_str(), message.length(), 0);
    // cout << "valsend = " << valsend << endl;
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
}

//closes the socket and confirms closure into console
void Network::disconnectClient() {
    close(currentClientSocket);
    cout << "The client exit the game." << endl << endl;
}

bool Network::receiveAndCheckAuthentication() {
    string authString = receive(); //"username=<value>,password=<value>"
    cout << authString << endl; 

    //extract username and entered password from authString
    int equalPos = authString.find("=");
    int commaPos = authString.find(",");
    string user = authString.substr(equalPos+1, commaPos - equalPos - 1);
    string passwordAttempt = authString.substr(commaPos+10);

    return userAuthentication(user, passwordAttempt);
}

bool Network::userAuthentication(string user, string passwordAttempt) {
    //find the inputted user in our users bank, if so, initilize currentUserIndex
    bool isFound = false;
    for (int i = 0; i < USER_CAPACITY && !isFound; i++) {
        if (users[i].username.compare(user) == 0)  {
            //cout << "found user : " << user << endl;
            currentUserIndex = i;
            isFound = true;
        }
    }

    //if user is not found or if user is found but password is wrong, authentication fails
    if (!isFound || passwordAttempt.compare(users[currentUserIndex].password) != 0) {
        cout << "Auth fail " << endl; //for string: " << authString << endl;
        return false;
    } else {
        //cout << "Auth success " << endl;// for string: " << authString << endl;
        return true;
    }
}


