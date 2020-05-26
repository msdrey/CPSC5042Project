#include "Connection.h"
#include <unistd.h> 
#include <netinet/in.h> 
#include <string>
#include <iostream> 

Connection::Connection(int socket) {
    socket = socket;
}

// receives whatever is found in the specified socket
// and returns it
string Connection::receive() {
    char userInput[1024] = {0};
    int valread = recv(socket, userInput, 1024, 0);
    // cout << "valread: " << valread << endl;
    if (valread == 0) {
        return "";
    }
    return string(userInput);
}

//sends the inputted message into the specified socket
// will throw an error message if the sending fails
void Connection::sendToClient(const string& message) {
    int valsend = send(socket, message.c_str(), message.length(), 0);
    // cout << "valsend = " << valsend << endl;
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
}

//closes the socket and confirms closure into console
void Connection::disconnectClient() {
    close(socket);
    cout << "The client exited the game." << endl << endl;
}





