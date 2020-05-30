// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include <iostream> 
#include <string>

#include "InterfaceManager.h"

using namespace std;

//audrey's port on cs1 for cpsc5042
#define PORT 12119
#define AWS_IP "54.91.202.143"
#define LOCAL_IP "127.0.0.1"

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

int main(int argc, char const *argv[]) {    
    try {
        //establishing connection with the server
        int sock = create_connection(argc, argv);

        clearScreen();
        display(LOGO, "green");
        sendToServer(sock, promptUserAuthentication());
                
        //receive authentication result and check if valid, if not, disconnect
        int authResult = stoi(receiveFromServer(sock));
        if (authResult < 0) {
            display(errorMessage(authResult), "red");
            close(sock);
            exit(0);
        }
        display("Your log in was successful", "green");
        sendToServer(sock, "true");
        pressAnyKeyRoutine();

        //receive and display welcome message & prompt
        clearScreen();
        display(receiveFromServer(sock), "green");
    
        //keep playing as long as the player does not issue the command ".exit"
        string userInput;
        string response;
        do {
            //take in user's input and send to server
            userInput = takeInput();
            sendToServer(sock, userInput);

            if (isAMatch(userInput, ".addword")) {
                response = receiveFromServer(sock);
                sendToServer(sock, takeNewWord());
            }

            //receive feedback + next prompt from server, and display them
            response = receiveFromServer(sock);
            display(response, "green");

        } while(userInput.compare(".exit") != 0);

        close(sock);

    } catch (const char* message) {
        cerr << message << endl;
        exit(EXIT_FAILURE);
    }
    
    return 0; 
} 