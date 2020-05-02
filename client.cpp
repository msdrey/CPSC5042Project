// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

// Uncomment this for string class
#include <iostream> 
#include <string>
using namespace std;

//audrey's port on cs1 for cpsc5042
#define PORT 12119
#define AWS_IP "54.91.202.143"
   
//First RPC
//Setting up server and establishing connection with a client   
int create_connection(string hostname = "127.0.0.1", int port = PORT) {
    struct sockaddr_in serv_addr; //a struct containing the info of the server's address
    
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
    //"127.0.0.1" is the localhost
    //"54.91.202.143" is the aws box
    //CHANGE THIS ADDRESS FOR USE OVER THE INTERNET????????
    if(inet_pton(AF_INET, hostname.c_str(), &serv_addr.sin_addr)<=0)  
    { 
        throw "Invalid IP address/ Address not supported"; 

    } 
   
    //open a connection between this client's socket and the server's address info
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
		throw "Connecting the socket to the address failed. The server might be down."; 
    } 
    // do auth here?
    return sock;
}

//Second RPC
//receive and print message + prompt
void receiveAndPrintToUser(int sock) {
    char message[1024] = {0};
    int valread = recv(sock, message, 1024, 0);
    if (valread == -1) {
		throw "receiving error";
	}
    cout << message << endl;
}

//Third RPC
//Take in user's input and send to server
string takeInputAndSend(int sock) {
    string ans;
    cin >> ans;
    int valsend = send(sock, ans.c_str(), ans.length(), 0);
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
    return ans;
}

int main(int argc, char const *argv[]) {    
    int sock;

    //the connection is established.

    //game
    //receive and print welcome message & prompt
    try {
        cout << "pre connection check" << endl;
        //establishing connection with the server
        if (argc == 2) {
            string hostnameKeyword = argv[1];
            if (hostnameKeyword.compare("aws") == 0) {
                sock = create_connection(AWS_IP);
            } else {
                sock = create_connection();
            }
        } else if (argc > 2) {
            string hostname = argv[1];
            int port = atoi(argv[2]);
            sock = create_connection(hostname, port);
        } else {
            sock = create_connection();
        }

        cout << "post connection check. Sock = " << sock << endl;
        
        //receive and print welcome message & prompt
        receiveAndPrintToUser(sock);
    
   
        string userInput = "";
        while(userInput.compare(".exit")) {

                //take in user's input and send to server
                userInput = takeInputAndSend(sock);

                //receive feedback + prompt from server, and print them
                receiveAndPrintToUser(sock);
        }

        close(sock);
        cout << "Disconnected from server." << endl;
    
    } catch (const char* message) {
        cerr << message << endl;
        exit(EXIT_FAILURE);
    }
    
    return 0; 
} 