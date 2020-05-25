/*
 *  Server side C/C++ program to demonstrate Socket programming 
 * 	Group project for Seattle University class CPSC 5042 Operating Systems, Spring 2020
 *  Group members: Audrey Morin, Ken Comvandee, Noah Glusenkamp
 */

// C dependencies
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 

// C++ dependencies
#include <iostream> 
#include <string>

// Project files
#include "Network.h"

using namespace std;

int main(int argc, char const *argv[]) 
{ 	
	// Creating a network object opens a socket
	Network * network = new Network(argc, argv);
	// Starts an infinite loop blocking on accept
	network->acceptConnections();

	return 0; 
} 
