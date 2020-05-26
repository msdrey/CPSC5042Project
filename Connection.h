#ifndef CONNECTION_H
#define CONNECTION_H

using namespace std;

class Connection{
    private:
        int socket;

        struct User { // a simple struct for keeping users information. 
                    // may be expanded later with high score or other info
                    // may be turned into a full class too
            string username;
            string password;
            int socket;
        };

    public:
        Connection(int socket);
        int getSocket(int);
        string receive();
	    void sendToClient(const string& );
	    void disconnectClient();
};

#endif //CONNECTION_H