// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 


int create_connection() {
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	
	// Creating socket file descriptor 
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) 
	{ 
		perror("Server's socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 

	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 

	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 

	//prepare to accept connections
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 

	//wait for a connection on the server_fd socket.
	//when a connection occurs, create a new socket for the client to connect with the server's socket
	new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen);
	if (new_socket < 0) 
	{ 
		printf("\n error \n");
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

	return new_socket;
}

int main(int argc, char const *argv[]) 
{ 	
	
	int new_socket = create_connection();
	//the connection is now established.
	
	//testing communication.
	const char *hello = "This message was sent by the server";
	char buffer[1024] = {0}; 
	//read N bytes from the client's socket and puts it in the buffer
	int valread = recv(new_socket, buffer, 1024, 0); 
	if (valread == -1) {
		printf("\n error \n");
		exit(EXIT_FAILURE);
	}
	//print the client's message
	printf("%s\n", buffer); 
	//send N bytes of the message hello into the client's socket
	send(new_socket, hello, strlen(hello), 0); 
	//prints a confirmation that a message was sent to the client
	printf("A message was sent to the client.\n"); 
	return 0; 
} 
