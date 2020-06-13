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
        client->sendToServer("1"); //handshake
        pressAnyKeyRoutine();

        //receive and display welcome message
        clearScreen();
        displayLogo();
        string response = client->receiveFromServer();
        int clearPos = response.find("@");
        display(response.substr(0, clearPos), "green");
        pressAnyKeyRoutine();
        
        string userInput;
        do {
            //display prompt
            clearScreen();
            display("Type .help to display options again.\n\n", "yellow");
            display(response.substr(clearPos + 1), "green");
    
            //take in user's input and send to server
            userInput = takeInput();
            client->sendToServer(userInput);

            //if user is adding a word, take that in and send it
            if (isAMatch(userInput, ".addword")) {
                response = client->receiveFromServer();
                client->sendToServer(takeNewWord());
            }
            if (isAMatch(userInput, ".leaderboard")) {
                response = client->receiveFromServer();
                display(response, "green");
                client->sendToServer("leaderboard diplayed");
            }
            if (isAMatch(userInput, ".highscore")) {
                response = client->receiveFromServer();
                display(response, "green");
                client->sendToServer("highscores displayed");
            }

            //receive and display feedback from server
            response = client->receiveFromServer();
            clearPos = response.find("@");
            display(response.substr(0, clearPos), "green");
            pressAnyKeyRoutine();
        } while(!isAMatch(userInput, ".exit"));

        client->closeSocket();

    } catch (const char* message) {
        cerr << message << endl;
        exit(EXIT_FAILURE);
    } 
    
    return 0; 
} 