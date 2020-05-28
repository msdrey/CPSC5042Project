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
string serializeAuthString(string choice, string username, string password) {
    string result;
    result = serializeKeyValuePair("username", username);
    result += "," + serializeKeyValuePair("password", password);
    return choice + ";" + result;
}

// prompt the user for a username and password and send it to server
void promptAndSendUserAuthentication(int sock) {
    
    string choice;
    bool validInput = false;
    while(!validInput) {
        cout << "\033[0;33m" 
             << "Please enter 'sign up' if you are a new user or 'log in' if you are a returning user" 
             << "\033[0m"
             << endl;
        getline(cin, choice);

        if (choice.compare("sign up")!=0&&choice.compare("log in")!=0) {
            cout << "\033[0;31m" 
                 << "This is not a valid choice. Try again." 
                 << "\033[0m"
                 << endl;
        } else {
            validInput = true;
        }
    }

    string username;
    string password;

    cout << "\033[0;33m" 
         << "Please enter your username: " 
         << "\033[0m"
         << endl;
    cin >> username;
    cout << "\033[0;33m" 
         << "Please enter your password: " 
         << "\033[0m"
         << endl;
    cin >> password;

    string authString = serializeAuthString(choice, username, password);

    sendToServer(sock, authString);
}


//check if authentication is valid. if valid,
//finalize connection and return true. if not valid, return false.
bool checkAuthResult(int sock, string serverResponse) {
    int authStatus = stoi(serverResponse.substr(13));
    //cout << "authStatus = " << authStatus<< endl;
    if (authStatus >= 0) {
        cout << "\n\033[0;32m" << "Your login was successful." <<  endl;
        //Confirm authorization to server.
        sendToServer(sock, "true");
        cout << "Press any key to continue." << "\033[0m" << flush;
        string dummy;
        cin.ignore();
        getline(cin, dummy);
        return true;
    } else if (authStatus == -1) {
        cout << "\031[0;32m"
             << "Incorrect username. Disconnecting..." 
             << "\033[0m"
             << endl;
        // TODO: allow retries
    } else if (authStatus == -2) {
        cout << "\031[0;32m"
             << "Incorrect password. Disconnecting..." 
             << "\033[0m"
             << endl;
    } else if (authStatus == -3) {
        cout << "\031[0;32m"
             << "User already exists. Disconnecting..."
             << "\033[0m"
             << endl;
    } else {
        cout << "auth error" << endl;
    }
    return false;
}

void addWord(int sock){
    string userWord;
    string userHint;

    cout << "Enter a word you want to add(with no space): ";
    cin >> userWord;
    cin.ignore();
    cout <<"\nEnter hint for the word: ";
    getline(cin, userHint);

    wordAddingProcess = false;
    sendToServer(sock, userWord + "," + userHint);
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
        //cout << "Post connection check. Sock = " << sock << endl;
        
        
        system("clear");
        cout << "\033[1;32m __          __           _                                       " << endl
             << " \\ \\        / /          | |                                      " << endl
             << "  \\ \\  /\\  / /__  _ __ __| | __ _ ___  __ _ _   _ _ __ _   _ ___  " << endl
             << "   \\ \\/  \\/ / _ \\| '__/ _` |/ _` / __|/ _` | | | | '__| | | / __| " << endl
             << "    \\  /\\  / (_) | | | (_| | (_| \\__ \\ (_| | |_| | |  | |_| \\__ \\ " << endl
             << "     \\/  \\/ \\___/|_|  \\__,_|\\__,_|___/\\__,_|\\__,_|_|   \\__,_|___/ \033[0m" << endl << endl << endl;

        promptAndSendUserAuthentication(sock);
                
        //receive authentication result and check if valid, if not, disconnect
        if (!checkAuthResult(sock, receiveFromServer(sock))) {
            disconnect(sock);
            exit(0);
        }

        system("clear");
        //receive and display welcome message & prompt
        cout << "\033[0;33m" << receiveFromServer(sock) << "\033[0m"<< endl;
    
        //keep playing as long as the player does not issue the command ".exit"
        string userInput;
        do {
            if (wordAddingProcess == true) {    
                addWord(sock);
                cout << "\033[0;33m"<<  receiveFromServer(sock) << "\033[0m" << endl;
            }

            //take in user's input and send to server
            userInput = takeInputAndSend(sock);

            if (toLowerCase(userInput).compare(".addword") == 0) {
                wordAddingProcess = true;
            }

            //receive feedback + next prompt from server, and display them
            cout << "\033[0;33m" << receiveFromServer(sock) << "\033[0m" << endl;
        } while(userInput.compare(".exit") != 0);

        //close connection with server
        disconnect(sock);
    
    } catch (const char* message) {
        cerr << message << endl;
        exit(EXIT_FAILURE);
    }
    
    return 0; 
} 