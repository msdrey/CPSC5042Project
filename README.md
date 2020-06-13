~~~
             .
          \  :  / 
           ' _ '      __          __           _                                       
       -= ( (_) ) =-  \ \        / /          | |                                      
           .   .       \ \  /\  / /__  _ __ __| | __ _ ___  __ _ _   _ _ __ _   _ ___  
          /  :  \       \ \/  \/ / _ \| '__/ _` |/ _` / __|/ _` | | | | '__| | | / __|
                         \  /\  / (_) | | | (_| | (_| \__ \ (_| | |_| | |  | |_| \__ \       .-.
               __         \/  \/ \___/|_|  \__,_|\__,_|___/\__,_|\__,_|_|   \__,_|___/       |.|
              / _)                                                                         /)|`|(\
     _.----._/ /                                                                          (.(|'|)`)
~~~~/         /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`\`'./'~~~~
 __/ (  | (  |               ~~                                             ~~               |.|    ~    
/__.-'|_|--|_|                                  ~~                       ~~                  |`|            
                     ~~                                            ~~                ~      ,|'|.
                               ~~                                                            "'"
~~~
# Wordasaurus
Word guessing game demonstrating client and server programming in C++.  

## Overview
Creeated as group project for Seattle University class CSPC5042, spring quarter 2020.  

Project members:
- Audrey Morin
- Ken Comvandee
- Noah Glusenkamp

### Project Idea
We are building a word guessing game that is inspired by the online game [Drawasaurus](https://www.drawasaurus.org/). A player can use the command-line client program to can connect to the server with a username and password and start a new game. The server will choose a secret word at random from a bank and give the player the number of letters in the word as well as a clue. The player sends guesses to the server until the correct word is guessed. Then, a new word is picked by the server and the game continues. The client can also send commands to to skip a word, display their score or exit the game.
Possible enhancements include concurrent users playing the game, allowing users to submit word/hint combinations to the present game's word bank, and allowing users to play socially.

## Docs
### Compile
#### Server
```
make -C src/server
```
#### Client
```
make -C src/client
```

### Run
#### Server
```
./bin/server         --->  will use default port 12119
./bin/server <PORT>  --->  will use specified port
```
#### Client
```
./bin/client              --->  will use default IP address 127.0.0.1 and default port 12119
./bin/client <IP> <PORT>  --->  will use specified IP address and specified port number
./bin/client cloud        --->  will use our google cloud virtual machine's IP address and default port 12119
```

## Play on our live server
Just clone this repo, cd into the project directory, and run the client release with the *cloud* option.
```
git clone https://github.com/msdrey/CPSC5042Project.git
cd CPSC5042Project
./releases/linux/client aws
```

## Credits
Used [GeeksforGeeks - Socket Programming in C](https://www.geeksforgeeks.org/socket-programming-cc/) example as the starting point.