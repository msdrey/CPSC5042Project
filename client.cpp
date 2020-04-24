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
   
int create_connection() {
    struct sockaddr_in serv_addr; //a struct containing the info of the server's address
    
    //create an endpoint socket for communication
    //AF_INET is an address family: IPv4 Internet protocols
    //SOCK_STREAM indicates the kind of socket to be created: a tcp socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);// returns a descriptor of this client's socket
    if (sock < 0) { 
        printf("\n Client's socket creation error \n"); 
        return -1; 
    } 
   
    //defining the server's address.
    //first, specify address family
    serv_addr.sin_family = AF_INET;
    //then, the server's port number. 
    //htons translates from a 16-bit number in host byte order into a 16-bit
    //number in network byte order (used in the AF_INET family)
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 address from text to binary form and store in serv_addr.sin_addr
    //"127.0.0.1" is the localhost
    //CHANGE THIS ADDRESS FOR USE OVER THE INTERNET????????
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    //open a connection between this client's socket and the server's address info
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
		printf("\nConnection Failed \n"); 
        return -1; 
    } 
    return sock;
}




int main(int argc, char const *argv[]) 
{    
    int sock = create_connection();
    //the connection is established.

    // //testing communication
    // string hello = "This message was sent by the client"; 
    // char buffer[1024] = {0}; 

    // send(sock, hello.c_str(), hello.length(), 0); 
    // printf("A message was sent to the server.\n"); 
    // int valread = read(sock, buffer, 1024); 
    // if (valread == -1) {
	// 	printf("\n error \n");
	// 	return -1;
	// }
    // printf("%s\n",buffer ); 

    //game
    //receive and print welcome message & prompt
    char message[1024] = {0};
    int valread = recv(sock, message, 1024, 0);
    if (valread == -1) {
		cout << endl << "error" << endl;
		return -1;
	}
    cout << message << endl;

    while(true) {
        //take in user's answer and send to server
        string ans;
        cin >> ans;
        send(sock, ans.c_str(), ans.length(), 0);

        //recieve feedback and prompt and print them
        char feedback[1024] = {0};
        valread = read(sock, feedback, 1024);
        if (valread == -1) {
            cout << endl << "error" << endl;
            return -1;
        }
        cout << feedback << endl;
    }


    return 0; 
} 