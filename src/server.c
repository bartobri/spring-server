// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "main.h"

int command_ping(int, char *, fd_set *);
int command_quit(int, char *, fd_set *);
int command_test(int, char *, fd_set *);

int startup(char *hostname, char *portno) {
	int mainsockfd;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	
	// Initializing serv_addr memory footprint to all integer zeros ('\0')
	memset((char *) &hints, 0, sizeof(struct addrinfo));
	
	// Get one or more arrdinfo structures that conforms with that provided by 'hints'
	hints.ai_family = AF_UNSPEC;              // Return IPv4 and IPv6 choices
	hints.ai_socktype = SOCK_STREAM;          // We want a TCP socket
	hints.ai_flags = AI_PASSIVE;              // All interfaces
	if (getaddrinfo(hostname, portno, &hints, &result) != 0) {
		fprintf(stderr, "server: Could not obtain internet address info.\n");
		return -1;
	}

	// Loop over results from getaddrinfo() and try to bind. Exit loop on first successful bind.
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		mainsockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (mainsockfd == -1)
			continue;

		if (bind(mainsockfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;                           // Successful binding

		close(mainsockfd);
	}
	
	// Error if we didn't bind to any sockets
	if (rp == NULL) {
		fprintf(stderr, "server: Could not bind to socket %i\n", mainsockfd);
		return -1;
	}

	// Free the result structure we don't need anymore
	freeaddrinfo(result);
	
	// Mark socket as accepting connections, up to 5 backlogged connections
	listen(mainsockfd, 5);
	
	return mainsockfd;

}

void initCommands(struct commandTable *commands) {
	int i;
	
	// Initialize all data to NULL
	for (i = 0; i < COMMAND_LIMIT; ++i) {
		commands[i].command = NULL;
		commands[i].functionPtr = NULL;
	}
	
	// Populate commands
	commands[0].command = "ping";
	commands[0].functionPtr = &command_ping;
	commands[1].command = "quit";
	commands[1].functionPtr = &command_quit;
	commands[2].command = "test";
	commands[2].functionPtr = &command_test;
}

int periodic(int mainsockfd,  fd_set *active_fd_set) {
	int i, n;

	// send ping command to all sockets
	for (i = 0; i < FD_SETSIZE; ++i) {
		if (i != mainsockfd && FD_ISSET(i, active_fd_set)) {

			// Send ping message
			n = write(i, "ping", 4);
			
			// Print error messsage if couldn't write data
			if (n < 0)
				return 1;
		}
	}
	
	// Check ping responses
	
	return 0;
}

void comp_cleanup(void) {
	// component cleanup here
}

int comp_type(void) {
	return SERVER;
}

int command_ping(int socket, char *payload, fd_set *active_fd_set) {
	printf("command_ping socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}

int command_quit(int socket, char *payload, fd_set *active_fd_set) {
	
	// Close socket
	close(socket);

	// remove socket from fd_set
	FD_CLR(socket, active_fd_set);
	
	return 0;
}

int command_test(int socket, char *payload, fd_set *active_fd_set) {
	printf("command_test socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}
