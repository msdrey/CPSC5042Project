#include "Connection.h"

Connection::Connection(int socket) {
    this->socket = socket;

    //initializing the bank of users
    users = new User[USER_CAPACITY];
    ifstream userbankfile("UserBank.txt");
    string line;
    int i = 0;
    while (i < USER_CAPACITY && getline(userbankfile, line)) {
        users[i].username = line;
        getline(userbankfile, line);
        users[i].password = line;
        i++;
    }
    userbankfile.close();
    usersCount = i;
}

int Connection::getSocket() {
    return socket;
}

int Connection::checkAuthentication(string authInfo) {
    
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

int Connection::createNewUser(string inputUser, string inputPass) {
    //add to file
    ofstream userbankfile;
    userbankfile.open("UserBank.txt", ios_base::app);//append to file
    if (userbankfile.is_open()) {
        userbankfile << "\n"<< inputUser << "\n" << inputPass;
        userbankfile.close();
    }
    
    //add to loaded userbank
    //todo: check if there is space for a new user
    //todo: make user capacity bigger if needed?
    //todo: check if user already there
    
    users[usersCount].username = inputUser;
    users[usersCount].password = inputPass;
    usersCount++;
    cout << "A new user signed up." << endl;
    return usersCount - 1;
}

int Connection::validateUsernamePassword(string inputUser, string inputPass) {
    //find the inputted user in our users bank, if so, initilize currentUserIndex
    bool isFound = false;
    int currentUserIndex;
    for (int i = 0; i < USER_CAPACITY && !isFound; i++) {
        if (users[i].username.compare(inputUser) == 0)  {
            //cout << "found user : " << user << endl;
            currentUserIndex = i;
            isFound = true;
        }
    }

    //if user is not found or if user is found but password is wrong, authentication fails
    if (!isFound || inputPass.compare(users[currentUserIndex].password) != 0) {
        cout << "Auth fail " << endl; //for string: " << authString << endl;
        return -1;
    } else {
        //cout << "Auth success " << endl;// for string: " << authString << endl;
        return currentUserIndex;
    }
}


