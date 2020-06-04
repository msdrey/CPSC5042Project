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

#define PORT 12119 //audrey's port on cs1 for cpsc5042
#define AWS_IP "54.91.202.143" // our aws server's IP address
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