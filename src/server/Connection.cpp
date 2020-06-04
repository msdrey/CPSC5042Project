#include "Connection.h"

/**
 *  Connection constructor 
 * 
 *  @param sock the socket number for this unique client connection
 */
Connection::Connection(int sock) {
    this->socket = sock;
    currentUserIndex = -1;
}

/**
 *  Closes the socket and confirms closure into console
 */
void Connection::disconnectClient() {
    close(socket);
    cout << "A connection was closed." << endl;
}

/**
 *  Sets the index into the user bank for the user on this connection
 */
void Connection::setCurrentUser(int currentUser) {
    this->currentUserIndex = currentUser;
}

int Connection::getCurrentUser() {
    return currentUserIndex;
}

/**
 *  Receives messages from the client socket
 * 
 *  @returns string the text that the user input
 */ 
string Connection::receive() {
    char userInput[1024] = {0};
    int valread = recv(this->socket, userInput, 1024, 0);
    if (valread == 0) {
        return "";
    }
    return string(userInput);
}

/**
 *  Sends a message to the client
 *  
 *  @param message the text to display to the client
 *  @throws if the message is unable to reach the client
 */ 
void Connection::sendToClient(const string& message) {
    int valsend = send(this->socket, message.c_str(), message.length(), 0);
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
}

