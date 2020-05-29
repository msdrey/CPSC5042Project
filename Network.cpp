#include "Network.h"

Network::Network() {
    //initializing the bank of users
    //note: no need for locks here as this constructor
    //is executed before any threads are created.
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

    //initializing the socket data field's lock
    pthread_mutex_init(&network_socket_lock, NULL);
    pthread_mutex_init(&userbankfile_lock, NULL);
}

void Network::setSocket(int newSocket) {
    //note: no need for locks here as this method
    //is only used while holding the socket_lock
    this->socket = newSocket;
}

int Network::getSocket() {
    //note: no need for locks here as this method
    //is only used while holding the socket_lock
    return socket;
}

int Network::checkAuthentication(string authInfo) {
    
    //extract username and entered password from authString
    //authInfo = <loginOrSignup>;username=<inputUser>,password=<inputPass>
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
    //note: must lock the whole function to make sure users are unique
    pthread_mutex_lock(&userbankfile_lock);
    pthread_mutex_lock(&userbankvector_lock);
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
    int userIndex = users.size() - 1;

    pthread_mutex_unlock(&userbankvector_lock);
    pthread_mutex_unlock(&userbankfile_lock);

    cout << "A new user signed up: User " << userIndex << "." << endl;
    return userIndex;
}

int Network::validateUsernamePassword(string inputUser, string inputPass) {
    //find the inputted user in our users bank, if so, initilize currentUserIndex
    bool isFound = false;
    int currentUserIndex;
    pthread_mutex_lock(&userbankvector_lock);
    for (unsigned int i = 0; i < users.size() && !isFound; i++) {
        if (users[i].username.compare(inputUser) == 0)  {
            currentUserIndex = i;
            isFound = true;
        }
    }

    //if user is not found or if user is found but password is wrong, authentication fails
    if (!isFound) {
        pthread_mutex_unlock(&userbankvector_lock);
        cout << "Auth fail: user not found " << endl; 
        return -1;
    } else if (inputPass.compare(users[currentUserIndex].password) != 0) {
        pthread_mutex_unlock(&userbankvector_lock);
        cout << "Auth fail: wrong password " << endl;
        return -2;
    } else {
        pthread_mutex_unlock(&userbankvector_lock);
        //cout << "Auth success " << endl;// for string: " << authString << endl;
        return currentUserIndex;
    }
}


