#include "Network.h"

#define USER_BANK_FILE_PATH "../data/UserBank.txt"
#define WORDS_AND_HINTS_FILE_PATH "../data/WordsAndHints.txt"

Network::Network() {
    //initializing the bank of users
    //note: no need for locks here as this constructor
    //is executed before any threads are created.
    ifstream userbankfile(USER_BANK_FILE_PATH);
    string line;
    while (getline(userbankfile, line)) {
        User newUser;

        int comma = line.find(",");
        int semiColon = line.find(";");
        int slash = line.find("/");

        //<username>,<password>;<highestScore>/<highestStreak>
        newUser.username = line.substr(0, comma);
        newUser.password = line.substr(comma + 1, semiColon - comma -1);
        newUser.highestScore = stoi(line.substr(semiColon + 1, slash));
        newUser.highestStreak = stoi(line.substr(slash + 1));
        newUser.isLoggedIn = false;

        users.push_back(newUser);
    }
    userbankfile.close();

    //initializing the locks
    pthread_mutex_init(&network_socket_lock, NULL);
    pthread_mutex_init(&userbankfile_lock, NULL);
    pthread_mutex_init(&userbankvector_lock, NULL);
    pthread_mutex_init(&wordsandhints_lock, NULL);
}

void Network::setSocket(int newSocket) {
    //note: no need for locks here as this method
    //is only used while holding the socket_lock
    this->socket = newSocket;
}

int Network::getSocket() {
    //note: no need for locks here as this method
    //is only used while holding the socket_lock
    return this->socket;
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

    if (isAMatch(loginOrSignup,"log in")) {
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
        if (isAMatch(users[i].username, inputUser)) {
            cout << "Auth fail: user already exists " << endl;
            pthread_mutex_unlock(&userbankvector_lock);
            pthread_mutex_unlock(&userbankfile_lock);
            return -3;
        }
    }
    
    //add to file
    ofstream userbankfile;
    userbankfile.open(USER_BANK_FILE_PATH, ios_base::app);//append to file
    if (userbankfile.is_open()) {
        userbankfile << inputUser << "," << inputPass << ";0/0\n";
        userbankfile.close();
    }
    
    
    //add to loaded userbank
    User newUser;
    newUser.username = inputUser;
    newUser.password = inputPass;
    newUser.isLoggedIn = true;
    newUser.highestScore = 0;
    newUser.highestStreak = 0;
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
    unsigned int currentUserIndex;
    pthread_mutex_lock(&userbankvector_lock);
    for (currentUserIndex = 0; currentUserIndex < users.size() && !isFound; currentUserIndex++) {
        if (isAMatch(users[currentUserIndex].username,inputUser))  {
            isFound = true;
            break;
        }
    }
    
    //if user is not found or if user is found but password is wrong, authentication fails
    if (!isFound) {
        pthread_mutex_unlock(&userbankvector_lock);
        cout << "Auth fail: user not found " << endl; 
        return -1;
    } else if (!isAMatch(inputPass, users[currentUserIndex].password)) {
        pthread_mutex_unlock(&userbankvector_lock);
        cout << "Auth fail: wrong password " << endl;
        return -2;
    } else if (users[currentUserIndex].isLoggedIn) {
        pthread_mutex_unlock(&userbankvector_lock);
        cout << "Auth fail: this user is already logged in" << endl;
        return -4;
    } else {
        users[currentUserIndex].isLoggedIn = true;
        pthread_mutex_unlock(&userbankvector_lock);
        cout << "User " << currentUserIndex << " has successfully logged in." << endl;
        return currentUserIndex;
    }
}

vector<string>* Network::getWordsAndHints() {
    vector<string> * result = new vector<string>();
    ifstream infile;
    pthread_mutex_lock(&wordsandhints_lock);
    infile.open(WORDS_AND_HINTS_FILE_PATH);

    if (infile.is_open())
    {
        string line;

        while (getline(infile, line))
        {
            result->push_back(line);
        }
    }

    infile.close();
    pthread_mutex_unlock(&wordsandhints_lock);

    return result;

}

string Network::getLeaderBoard(){
    pthread_mutex_lock(&userbankvector_lock);
    //make a copy of the userbank
    vector<User> highestScores(users);
    pthread_mutex_unlock(&userbankvector_lock);

    //do 3 loops of selection sort to find the top 3 highest scoring users
    for (unsigned int i = 0; i < 3; i++){
        int maxIndex = i;
        for (unsigned int j = i + 1; j < highestScores.size(); j++)
        {
            if (highestScores[j].highestScore > highestScores[maxIndex].highestScore)
                maxIndex = j;
        }

        User temp = highestScores[maxIndex];
        highestScores[maxIndex] = highestScores[i];
        highestScores[i] = temp;
    }

    //#1 ken, score: 29
    string result = "\n";
    result += "#1 " + highestScores[0].username + ", score: " + to_string(highestScores[0].highestScore) + "\n";
    result += "#2 " + highestScores[1].username + ", score: " + to_string(highestScores[1].highestScore) + "\n";
    result += "#3 " + highestScores[2].username + ", score: " + to_string(highestScores[2].highestScore) + "\n";

    return result;
}

void Network::addWord(string userWordHint){
    pthread_mutex_lock(&wordsandhints_lock);
    ofstream outfile;
    outfile.open(WORDS_AND_HINTS_FILE_PATH, ios::app);
    if (outfile.is_open()) {
        outfile << userWordHint << endl;
        outfile.close();
    }
    pthread_mutex_unlock(&wordsandhints_lock);
}

string Network::getHighScore(int userIndex) {
    pthread_mutex_lock(&userbankvector_lock);
    int highScore = users[userIndex].highestScore;
    int highStreak = users[userIndex].highestStreak;
    pthread_mutex_unlock(&userbankvector_lock);
    string result = "Your highest score: " + to_string(highScore) + "\n";
    result += "Your highest streak: " + to_string(highStreak) + "\n";
    return result;
}

void Network::logOutUser(int userIndex) {
    pthread_mutex_lock(&userbankvector_lock);
    users[userIndex].isLoggedIn = false;
    pthread_mutex_unlock(&userbankvector_lock);
}

// returns true if the two strings match. Case insensitive.
bool Network::isAMatch(const string& str1, const string& str2) {
    unsigned int len = str1.length();
    if (str2.length() != len){
        return false;
    }
    for (unsigned int i = 0; i < len; ++i) {
        if (tolower(str1[i]) != tolower(str2[i])) {
            return false;
        }
    }
    return true;
}

void Network::updateUserScores(int newScore, int newBestStreak, int userIndex) {
    bool changed = false;
    pthread_mutex_lock(&userbankfile_lock);
    pthread_mutex_lock(&userbankvector_lock);
    if (newScore > users[userIndex].highestScore) {
        users[userIndex].highestScore = newScore;
        changed = true;
    }
    if (newBestStreak > users[userIndex].highestStreak) {
        users[userIndex].highestStreak = newBestStreak;
        changed = true;
    }
    if (changed) {
        ofstream userbankfile;
        userbankfile.open(USER_BANK_FILE_PATH);//overwrite file
        if (userbankfile.is_open()) {
            for (User user: users) {
                userbankfile << user.username << "," << user.password << ";"<< user.highestScore <<"/"<< user.highestStreak <<"\n";
            } 
            userbankfile.close();
        }
    }
    pthread_mutex_unlock(&userbankvector_lock);
    pthread_mutex_unlock(&userbankfile_lock);
}