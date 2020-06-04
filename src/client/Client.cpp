#include "Client.h"

/**
 * Constructor. Establishes connection with the server according to input 
 * parameters.
 * 
 * @param numberOfParameters is the number of parameters passed to the 
 *                           function call 
 * @param argv is an array of the parameters, where we need to ignore item 0.
 *             argv[1], if specified, is the hostname. It is either an IP 
 *                      address or "aws".
 *             argv[2], if specified, is the port number.
 * 
 * @return the int id of the created socket for the client.
 * 
 * @throw an error if socket creation fails
 * @throw an error if converting the address fails
 * @throw an error if connecting to the server fails
 */
Client::Client(int numberOfParameters, char const *argv[]) {
    string hostname;
    int port;

    if (numberOfParameters > 1) { //hostname and port were specified
        hostname = argv[1];
        port = atoi(argv[2]);
    } else if (numberOfParameters == 1 && 
               string(argv[1]).compare("aws") == 0) {
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
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { 
        throw "Client's socket creation error";
    } 

    //defining the server's address.
    struct sockaddr_in serv_addr;
    //first, specify address family
    serv_addr.sin_family = AF_INET;
    //then, the server's port number. 
    //htons translates from a 16-bit number in host byte order into a 16-bit
    //number in network byte order (used in the AF_INET family)
    serv_addr.sin_port = htons(port); 
       
    //Convert IPv4 address to binary form and store in serv_addr.sin_addr
    if(inet_pton(AF_INET, hostname.c_str(), &serv_addr.sin_addr)<=0) { 
        throw "Invalid IP address/ Address not supported"; 
    } 
   
    //open a connection between this client's socket and the server's 
    //address info
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
		throw "Connecting the socket to the address failed. The server might be down."; 
    } 

    this->clientSocket = sock;
}

/**
 * Receives messages of maximum 1024 characters from the socket that is
 * passed as input and returns the received message.
 * 
 * @param sock the socket from which to receive
 * @return the message in the form of a string.
 * @throw an error if the receiving fails
 */
string Client::receiveFromServer() {
    char message[1024] = {0};
    int valread = recv(this->clientSocket, message, 1024, 0);
    if (valread == -1) {
		throw "receiving error";
	}
    return string(message);
}

/**
 * Sends an inputted message of maximum 1024 characters to the socket that is
 * passed as input.
 * 
 * @param sock the socket on which to send
 * @param message the message to be sent
 * @throw an error if the send fails.
 */
void Client::sendToServer(string message) {
    int valsend = send(this->clientSocket, message.c_str(), message.length(), 0);
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
}

/**
 * Closes the socket connection with the server.
 */
void Client::closeSocket() {
    close(this->clientSocket);
}