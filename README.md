# Wordasaurus
Word guessing game demonstrating client and server programming in C++.  

## Overview
Creeated as group project for Seattle University class CSPC5042, spring quarter 2020.  

Project members:
- Audrey Morin
- Ken Comvandee
- Noah Glusenkamp

### Project Idea
We are building a word guessing game that is inspired by the online game [Drawasaurus](https://www.drawasaurus.org/). A player can use the command-line client program to can connect to the server with a username and password and start a new game. The server will choose a secret word at random from a bank and give the player the number of letters in the word as well as a clue. The player sends guesses to the server until the correct word is guessed. Possible enhancements include concurrent users playing the game socially, and allowing users to submit word/hint combinations to the present game's word bank.

## Docs
### Compile
```
make
```

### Run
Can either be run using published binaries in bin directory or by compiling as above.
#### Server
```
./bin/server
```
#### Client
```
./bin/client
```

## Credits
Used [GeeksforGeeks - Socket Programming in C](https://www.geeksforgeeks.org/socket-programming-cc/) example as the starting point.