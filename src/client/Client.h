/**
 * Client side C/C++ program to demonstrate Socket programming 
 * 
 * Audrey Morin, Noah Glusenkamp, Nitid Comvandee
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h> 
#include <unistd.h> 
#include <iostream> 
#include <string>

using namespace std;

#define PORT 12119 //default port that is accessible on the virtual machine
#define VM_IP "34.82.231.190" // our google cloud virtual machine's IP address
#define LOCAL_IP "127.0.0.1"

class Client {
private:
    int clientSocket; /** the id of the client socket */

public:
    Client(int, char const*argv[]);

    string receiveFromServer();
    void sendToServer(string);
    void closeSocket();
};

#endif //CLIENT_H