/*
 *  Server side C/C++ program to demonstrate Socket programming 
 * 	Group project for Seattle University class CPSC 5042 Operating Systems, Spring 2020
 *  Group members: Audrey Morin, Ken Comvandee, Noah Glusenkamp
 */

// Project files
#include "Server.h"

int main(int argc, char const *argv[]) 
{ 	
	// Creating a network object opens a socket
	Server * server = new Server(argc, argv);
	// Starts an infinite loop blocking on accept
	server->acceptConnections();

	return 0; 
} 
