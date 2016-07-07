// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFFER_SIZE 256

// Function Prototypes
void error(const char *msg);

/*
 * main function
 */
int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[BUFFER_SIZE];
	fd_set server_fd_set;

	// Check arguments
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	// Convert port argument to int
	portno = atoi(argv[2]);

	// Set up a socket in the AF_INET domain (Internet Protocol v4 addresses)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");

	// Get a pointer to 'hostent' containing info about host.
	// Error if invalid host
	server = gethostbyname(argv[1]);
	if (server == NULL)
		error("ERROR, no such host");

	// Initializing serv_addr memory footprint to all integer zeros ('\0')
	memset(&serv_addr, 0, sizeof(serv_addr));

	// Setting up our serv_addr structure
	serv_addr.sin_family = AF_INET;            // Internet Protocol v4 addresses
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length); // Copy address
	serv_addr.sin_port = htons(portno);         // Convert port byte order to 'network byte order'

	// Connect to server. Error if can't connect.
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");

	// Set buffer to all integer zeros ('\0')
	memset(buffer, 0, BUFFER_SIZE);

	// Read accept message from server. Error if can't read.
	n = read(sockfd, buffer, BUFFER_SIZE - 1);
	if (n < 0) 
		error("ERROR reading from socket");

	// Confirm expected server response
	if (strcmp(buffer, "accept") != 0)
		error("Unknown response from connection request. Quitting.");

	// Print server accept message
	printf("Connected Successfully\n");

	// Initialize fd set
	FD_ZERO (&server_fd_set);

	// Add server socket to fd set
	FD_SET (sockfd, &server_fd_set);

	while (true) {

		// Block until input arrives on server socket
		if (select(FD_SETSIZE, &server_fd_set, NULL, NULL, NULL) < 0)
			error("select error");

		// Clear buffer for reading (all integer zeros ('\0'))
		memset(buffer, 0, BUFFER_SIZE);

		// Read from server.
		n = read(sockfd, buffer, BUFFER_SIZE - 1);

		// Error if can't read from socket
		if (n < 0) 
			error("ERROR reading from socket");

		// Print msg and exit if we got EOF (server terminated connection)
		else if (n == 0) {
			fprintf(stderr, "Server terminated connection.\n");
			exit(0);
		}

		// Evaluate commands and respond
		else {

			// Check for ping command
			if (strcmp(buffer, "ping") == 0)
				n = write(sockfd, "ping", 4);

		}

	}

	// Close socket
	close(sockfd);

	return 0;
}

/*
 * Print error message and exit
 */
void error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(0);
}
