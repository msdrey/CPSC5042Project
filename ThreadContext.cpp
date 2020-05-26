#include "ThreadContext.h"

ThreadContext::ThreadContext(int sock) {
    this->socket = sock;
}

//closes the socket and confirms closure into console
void ThreadContext::disconnectClient() {
    close(socket);
    cout << "The client exited the game." << endl << endl;
}

void ThreadContext::setCurrentUser(int currentUserIndex) {
    this->currentUserIndex = currentUserIndex;
}

// receives whatever is found in the client socket
// and returns it
string ThreadContext::receive() {
    char userInput[1024] = {0};
    int valread = recv(this->socket, userInput, 1024, 0);
    // cout << "valread: " << valread << endl;
    if (valread == 0) {
        return "";
    }
    return string(userInput);
}

//sends the inputted message into the specified socket
// will throw an error message if the sending fails
void ThreadContext::sendToClient(const string& message) {
    int valsend = send(this->socket, message.c_str(), message.length(), 0);
    // cout << "valsend = " << valsend << endl;
    if (valsend == -1) {
        throw "error occured while sending data to server";
    }
}

