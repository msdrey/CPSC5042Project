# Wordasaurus
Word guessing game demonstrating client and server programming in C++.  

## Overview
Creeated as group project for Seattle University class CSPC5042, spring quarter 2020.  
Project members
- Audry Morin
- Ken Comvandee
- Noah Glusenkamp

### Project Idea
We are building aword guessing game that is inspired by the online game Drawasaurus. The client (to which I also refer as player) can connect to the server with a username and password. Then, the client can start a new game. The server then chooses a secret word at random in a bank and gives the player the number of letters in the word as well as a clue. The player sends guesses to the server until he guesses the right word. The player can also request an additional clue. Possible enhancements include concurrent users playing the game socially, and allowing users submission to the present games word bank.

## Docs
### Compile
```
g++ server.cpp -o server 
g++ client.cpp -o client
```

### Run
Can either be run using published binaries in bin directory or by compiling as above.
#### On server
```
./bin/server
```
#### On client
```
./bin/client
```