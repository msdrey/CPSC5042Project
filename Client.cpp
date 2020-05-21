// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include <iostream> 
#include <string>
using namespace std;

//audrey's port on cs1 for cpsc5042
#define PORT 12119
#define AWS_IP "54.91.202.143"
#define LOCAL_IP "127.0.0.1"
bool wordAddingProcess = false;

// Helper functions

//receive a string from the server and print it in console
void receiveAndPrintToUser(int sock) {
    char message[1024] = {0};
    //cout << "in receive and print" << endl;
    int valread = recv(sock, message, 1024, 0);
    if (valread == -1) {
		throw "receiving error";
	}
    //cout << "after recv" << endl;
    cout << message << endl;
}

//Takes in user's input, and then both sends it to the server and returns it
string takeInputAndSend(int sock) {
    string ans;
    cin >> ans;
    int valsend = send(sock, ans.c_str(), ans.length(), 0);
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
    return ans;
}

//closes the connection
void disconnect(int socket) {
    close(socket);
    cout << "Disconnected from server." << endl;
}

//formats a key and value pair
string serializeKeyValuePair(string key, string value) {
    return key + "=" + value;
}

//formats a username and password pair for communication with the server
string serializeAuthString(string username, string password) {
    string result;
    result = serializeKeyValuePair("username", username);
    result += "," + serializeKeyValuePair("password", password);
    return result;
}

// prompt the user for a username and password and send it to server
void promptAndSendUserAuthentication(int sock) {
    string username;
    string password;
    string authString;


    cout << "Please enter your username: " << endl;
    cin >> username;
    cout << "Please enter your password: " << endl;
    cin >> password;

    authString = serializeAuthString(username, password);

    int valsend = send(sock, authString.c_str(), authString.length(), 0);
    if (valsend == -1) {
        throw "error occured while sending data to server, on authentication attempt";
    }
}

//returns 'log in' or 'sign up'
void promptSignupOrLogin(int sock) {
    string choice;
    bool validInput = false;
    while(!validInput) {
        cout << "Please enter 'sign up' if you are a new user or 'log in' if you are a returning user" << endl;
        getline(cin, choice);

        if (choice.compare("sign up")!=0&&choice.compare("log in")!=0) {
            cout << "This is not a valid choice. Try again." << endl;
        } else {
            validInput = true;
        }
    }
    //send choice to server so it prepares to get log in or sign in
    int valsend = send(sock, choice.c_str(), choice.length(), 0);
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
    //receive handshake
    char serverResponseBuffer[1024] = {0};
    int valread = recv(sock, serverResponseBuffer, 1024, 0);
    if (valread == -1) {
        throw "receiving error";
    }
}

//receive authentication result and check if valid. if valid,
//finalize connection and return true. if not valid, return false.
bool receiveAuthResult(int sock) {
    char serverResponseBuffer[1024] = {0};
    int valread = recv(sock, serverResponseBuffer, 1024, 0);
    if (valread == -1) {
		throw "receiving error, on authentiction attempt";
	}

    string serverResponseString(serverResponseBuffer);
    //cout << serverResponseString << endl;
    // TODO: deserialize with function
    if (serverResponseString.compare(serializeKeyValuePair("isValidLogin", "false")) == 0) {
        cout << "Incorrect username or password. Disconnecting..." << endl;
        // TODO: allow retries
        return false;
    } else {
        cout << "Your login was successful." << endl;
        //Confirm authorization to server.
        string isConfirmed = "true";
        int valsend = send(sock, isConfirmed.c_str(), isConfirmed.length(), 0);
        if (valsend == -1) {
            throw "error occured while sending data to server, on confirmation attempt";
        }
        return true;
    }
}

void addWord(int sock){
    string userWord;
    string userHint;
    string userWordAndHint;

    cout << "Enter a word you want to add: ";
    cin >> userWord;
    cout <<"\nEnter hint for the word: ";
    cin >> userHint;

    userWordAndHint = serializeAuthString(userWord, userHint);
    wordAddingProcess = false;
    int valsend = send(sock, userWordAndHint.c_str(), userWordAndHint.length(), 0);
    if (valsend == -1) {
        throw "Error while sending user's word and hint suggestion";
    }
}

//establishing connection with the server according to default ip and port 
//or according to specified values when program was executed
//ex: ./bin/client aws          will set up ip and port automatically
//    ./bin/client              will set up default ip and port (localhost and 12119)
//    ./bin/client <IP> <port>  will set up specified IP and port 
int create_connection(int argc, char const *argv[]) {
    struct sockaddr_in serv_addr; //a struct containing the info of the server's address
    string hostname;
    int port;

    if (argc > 2) { //hostname and port were specified
        hostname = argv[1];
        port = atoi(argv[2]);
    } else if (argc == 2 && string(argv[1]).compare("aws") == 0) {
        //connecting to aws box with default port
        hostname = AWS_IP;
        port = PORT;
    } else { //default values
        hostname = LOCAL_IP;
        port = PORT;
    }
    
    //create an endpoint socket for communication
    //AF_INET is an address family: IPv4 Internet protocols
    //SOCK_STREAM indicates the kind of socket to be created: a tcp socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);// returns a descriptor of this client's socket
    if (sock < 0) { 
        throw "Client's socket creation error";
    } 
    //defining the server's address.
    //first, specify address family
    serv_addr.sin_family = AF_INET;
    //then, the server's port number. 
    //htons translates from a 16-bit number in host byte order into a 16-bit
    //number in network byte order (used in the AF_INET family)
    serv_addr.sin_port = htons(port); 
       
    // Convert IPv4 address from text to binary form and store in serv_addr.sin_addr
    if(inet_pton(AF_INET, hostname.c_str(), &serv_addr.sin_addr)<=0)  
    { 
        throw "Invalid IP address/ Address not supported"; 
    } 
   
    //open a connection between this client's socket and the server's address info
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
		throw "Connecting the socket to the address failed. The server might be down."; 
    } 

    return sock;
}

int main(int argc, char const *argv[]) {    
    
    try {
        //establishing connection with the server
        int sock;
        sock = create_connection(argc, argv);
        cout << "Post connection check. Sock = " << sock << endl;
        
        // provide user with choice: sign in or log in?
        promptSignupOrLogin(sock);

        promptAndSendUserAuthentication(sock);
        
        
        //receive authentication result and check if valid, if not, disconnect
        if (!receiveAuthResult(sock)) {
            disconnect(sock);
            exit(0);
        }

        //receive and display welcome message & prompt
        receiveAndPrintToUser(sock);
    
        //keep playing as long as the player does not issue the command ".exit"
        string userInput;
        do {
            if (wordAddingProcess == true) {    
                addWord(sock);
                receiveAndPrintToUser(sock);
            }

            //take in user's input and send to server
            userInput = takeInputAndSend(sock);

            if (userInput.compare(".addWord") == 0) {
                wordAddingProcess = true;
            }

            //receive feedback + next prompt from server, and display them
            receiveAndPrintToUser(sock);
        } while(userInput.compare(".exit") != 0);

        //close connection with server
        disconnect(sock);
    
    } catch (const char* message) {
        cerr << message << endl;
        exit(EXIT_FAILURE);
    }
    
    return 0; 
} 