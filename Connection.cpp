#include "Connection.h"

Connection::Connection(int sock) {
    this->socket = sock;
    currentUserIndex = -1;
}

//closes the socket and confirms closure into console
void Connection::disconnectClient() {
    close(socket);
    cout << "User " << currentUserIndex << " exited the game." << endl << endl;
}

void Connection::setCurrentUser(int currentUserIndex) {
    this->currentUserIndex = currentUserIndex;
}

// receives whatever is found in the client socket
// and returns it
string Connection::receive() {
    char userInput[1024] = {0};
    int valread = recv(this->socket, userInput, 1024, 0);
    if (valread == 0) {
        return "";
    }
    return string(userInput);
}

//sends the inputted message into the specified socket
// will throw an error message if the sending fails
void Connection::sendToClient(const string& message) {
    int valsend = send(this->socket, message.c_str(), message.length(), 0);
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
}

