/**
 *  Client side C/C++ program to demonstrate Socket programming 
 * 	Group project for Seattle University class CPSC 5042 Operating Systems, Spring 2020
 *  Group members: Audrey Morin, Ken Comvandee, Noah Glusenkamp
 */

#include "Client.h"
#include "InterfaceManager.h"

int main(int argc, char const *argv[]) {    
    try {
        //initializeing client connection with the server
        Client * client = new Client(argc - 1, argv);

        //authentication input routine
        clearScreen();
        displayLogo();
        client->sendToServer(promptUserAuthentication());
                
        //receive authentication result and check if valid, if not, disconnect
        int authResult = stoi(client->receiveFromServer());
        if (authResult < 0) {
            display(errorMessage(authResult), "red");
            client->closeSocket();
            exit(0);
        }
        display("Your log in was successful", "green");
        client->sendToServer("true");
        pressAnyKeyRoutine();

        //receive and display welcome message
        clearScreen();
        displayLogo();
        string response = client->receiveFromServer();
        int clearPos = response.find("@");
        display(response.substr(0, clearPos), "green");
        pressAnyKeyRoutine();

        //display first prompt
        clearScreen();
        display(response.substr(clearPos + 1), "green");
    
        //keep playing as long as the player does not issue the command ".exit"
        string userInput;
        do {
            //take in user's input and send to server
            userInput = takeInput();
            client->sendToServer(userInput);

            //if user is adding a word, take that in and send it
            if (isAMatch(userInput, ".addword")) {
                response = client->receiveFromServer();
                client->sendToServer(takeNewWord());
            }

            //receive and display feedback from server
            response = client->receiveFromServer();
            int clearPos = response.find("@");
            display(response.substr(0, clearPos), "green");
            pressAnyKeyRoutine();

            //display next prompt
            clearScreen();
            display(response.substr(clearPos + 1), "green");

        } while(!isAMatch(userInput, ".exit"));

        client->closeSocket();

    } catch (const char* message) {
        cerr << message << endl;
        exit(EXIT_FAILURE);
    } 
    
    return 0; 
} 