#include "Network.h"

Network::Network() {
    //initializing the bank of users
    ifstream userbankfile("UserBank.txt");
    string line;
    while (getline(userbankfile, line)) {
        User newUser;
        newUser.username = line;
        getline(userbankfile, line);
        newUser.password = line;
        users.push_back(newUser);
    }
    userbankfile.close();
}

void Network::setSocket(int newSocket) {
    this->socket = newSocket;
}

int Network::getSocket() {
    return socket;
}

int Network::checkAuthentication(string authInfo) {
    
    //extract username and entered password from authString
    int colonPos = authInfo.find(";");
    int equalPos = authInfo.find("=");
    int commaPos = authInfo.find(",");
    string loginOrSignup = authInfo.substr(0, colonPos);
    string inputUser = authInfo.substr(equalPos+1, commaPos - equalPos - 1);
    string inputPass = authInfo.substr(commaPos+10);

    if (loginOrSignup.compare("log in")==0) {
        return validateUsernamePassword(inputUser, inputPass);
    } else { // "sign up"
        return createNewUser(inputUser, inputPass);
    }
}

int Network::createNewUser(string inputUser, string inputPass) {
    //check if user already exists
    for (unsigned int i = 0; i < users.size(); i++) {
        if (users[i].username.compare(inputUser) == 0) {
            cout << "Auth fail: user already exists " << endl;
            return -3;
        }
    }
    
    //add to file
    ofstream userbankfile;
    userbankfile.open("UserBank.txt", ios_base::app);//append to file
    if (userbankfile.is_open()) {
        userbankfile << "\n"<< inputUser << "\n" << inputPass;
        userbankfile.close();
    }
    
    //add to loaded userbank
    User newUser;
    newUser.username = inputUser;
    newUser.password = inputPass;
    users.push_back(newUser);   
    cout << "A new user signed up." << endl;
    return users.size() - 1;
}

int Network::validateUsernamePassword(string inputUser, string inputPass) {
    //find the inputted user in our users bank, if so, initilize currentUserIndex
    bool isFound = false;
    int currentUserIndex;
    for (unsigned int i = 0; i < users.size() && !isFound; i++) {
        if (users[i].username.compare(inputUser) == 0)  {
            //cout << "found user : " << user << endl;
            currentUserIndex = i;
            isFound = true;
        }
    }

    //if user is not found or if user is found but password is wrong, authentication fails
    if (!isFound) {
        cout << "Auth fail: user not found " << endl; //for string: " << authString << endl;
        return -1;
    } else if (inputPass.compare(users[currentUserIndex].password) != 0) {
        cout << "Auth fail: wrong password " << endl;
        return -2;
    } else {
        //cout << "Auth success " << endl;// for string: " << authString << endl;
        return currentUserIndex;
    }
}


