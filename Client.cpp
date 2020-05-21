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

string receiveFromServer(int sock) {
    char message[1024] = {0};
    int valread = recv(sock, message, 1024, 0);
    if (valread == -1) {
		throw "receiving error";
	}
    return string(message);
}

void sendToServer(int sock, string message) {
    int valsend = send(sock, message.c_str(), message.length(), 0);
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
}

//Takes in user's input, and then both sends it to the server and returns it
string takeInputAndSend(int sock) {
    string ans;
    cin >> ans;
    sendToServer(sock, ans);
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

    sendToServer(sock, serializeAuthString(username, password));
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
    sendToServer(sock, choice);
    receiveFromServer(sock);
}

//check if authentication is valid. if valid,
//finalize connection and return true. if not valid, return false.
bool checkAuthResult(int sock, string serverResponse) {

    // TODO: deserialize with function
    if (serverResponse.compare(serializeKeyValuePair("isValidLogin", "false")) == 0) {
        cout << "Incorrect username or password. Disconnecting..." << endl;
        // TODO: allow retries
        return false;
    } else {
        cout << "Your login was successful." << endl;
        //Confirm authorization to server.
        string isConfirmed = "true";
        sendToServer(sock, isConfirmed);
        return true;
    }
}

void addWord(int sock){
    string userWord;
    string userHint;

    cout << "Enter a word you want to add: ";
    cin >> userWord;
    cin.ignore();
    cout <<"\nEnter hint for the word: ";
    getline(cin, userHint);
    
    userWordAndHint = userWord + "," + userHint;

    wordAddingProcess = false;
    sendToServer(sock, serializeAuthString(userWord, userHint));
}

string toLowerCase(const string &userInput){
    int len = userInput.length();
    string temp;

    for (int i = 0; i < len; i++)
    {
        temp += tolower(userInput[i]);
    }

    return temp;
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
        if (!checkAuthResult(sock, receiveFromServer(sock))) {
            disconnect(sock);
            exit(0);
        }

        //receive and display welcome message & prompt
        cout << receiveFromServer(sock) << endl;
    
        //keep playing as long as the player does not issue the command ".exit"
        string userInput;
        do {
            if (wordAddingProcess == true) {    
                addWord(sock);
                cout << receiveFromServer(sock) << endl;
            }

            //take in user's input and send to server
            userInput = takeInputAndSend(sock);

            if (toLowerCase(userInput).compare(".addword") == 0) {
                wordAddingProcess = true;
            }

            //receive feedback + next prompt from server, and display them
            cout << receiveFromServer(sock) << endl;
        } while(userInput.compare(".exit") != 0);

        //close connection with server
        disconnect(sock);
    
    } catch (const char* message) {
        cerr << message << endl;
        exit(EXIT_FAILURE);
    }
    
    return 0; 
} 