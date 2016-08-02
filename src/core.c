// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "main.h"
#include "core.h"
#include "command.h"
#include "socktime.h"
#include "socklist.h"

// Static vars
static fd_set read_fd_set;

int core_startup(char *hostname, char *portno) {
	int startsockfd = 0;
	
	if (comp_type() == SERVER) {
		struct addrinfo hints;
		struct addrinfo *result, *rp;
		
		// Initializing serv_addr memory footprint to all integer zeros ('\0')
		memset((char *) &hints, 0, sizeof(struct addrinfo));
		
		// Get one or more arrdinfo structures that conforms with that provided by 'hints'
		hints.ai_family = AF_UNSPEC;              // Return IPv4 and IPv6 choices
		hints.ai_socktype = SOCK_STREAM;          // We want a TCP socket
		hints.ai_flags = AI_PASSIVE;              // All interfaces
		if (getaddrinfo(hostname, portno, &hints, &result) != 0) {
			return -1;
			// TODO - error module to handle printing error messages and setting global error code
			//fprintf(stderr, "server: Could not obtain internet address info.\n");
		}
	
		// Loop over results from getaddrinfo() and try to bind. Exit loop on first successful bind.
		for (rp = result; rp != NULL; rp = rp->ai_next) {
			startsockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			if (startsockfd == -1)
				continue;
	
			if (bind(startsockfd, rp->ai_addr, rp->ai_addrlen) == 0)
				break;                           // Successful binding
	
			close(startsockfd);
		}
		
		// Error if we didn't bind to any sockets
		if (rp == NULL) {
			//fprintf(stderr, "server: Could not bind to socket %i\n", startsockfd);
			return -1;
		}
	
		// Free the result structure we don't need anymore
		freeaddrinfo(result);
		
		// Mark socket as accepting connections, up to 5 backlogged connections
		listen(startsockfd, 5);
	} else if (comp_type() == CLIENT) {
		struct hostent *server;
		struct sockaddr_in serv_addr;
		
		// Require hostname and port
		if (hostname == NULL) {
			//fprintf(stderr, "client: hostname can not be NULL.\n");
			return -1;
		}
		
		// Set up a socket in the AF_INET domain (Internet Protocol v4 addresses)
		startsockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (startsockfd < 0) {
			//fprintf(stderr, "client: Could not create socket\n");
			return -1;
		}
		
		// Get a pointer to 'hostent' containing info about host.
		server = gethostbyname(hostname);
		if (server == NULL) {
			//fprintf(stderr, "client: no such host: %s\n", hostname);
			return -1;
		}
		
		// Initializing serv_addr memory footprint to all integer zeros ('\0')
		memset(&serv_addr, 0, sizeof(serv_addr));
		
		// Setting up our serv_addr structure
		serv_addr.sin_family = AF_INET;            // Internet Protocol v4 addresses
		memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length); // Copy address
		serv_addr.sin_port = htons(atoi(portno));  // Convert port byte order to 'network byte order'
		
		// Connect to server. Error if can't connect.
		if (connect(startsockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
			//fprintf(stderr, "client: error conecting to host %s port %s\n", hostname, portno);
			return -1;
		}
	} else {
		return -1;
		//error("Unknown component type.");
	}
	
	return startsockfd;
}

int core_wait(void) {
	int r;
	struct timeval timeout;
	
	read_fd_set = socklist_get();
		
	// Set select() timeout value.
	// This needs to be inside the loop so it is reset for each loop interation.
	timeout.tv_sec  = (unsigned int)PERIODIC_SECONDS;
	timeout.tv_usec = 0;
		
	// Block until input arrives on one or more active sockets
	r = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout);
	
	return r;
}

int core_accept(int socket) {
	int newsockfd;
	struct sockaddr cliaddr;
	socklen_t clilen;
	
	if (FD_ISSET(socket, &read_fd_set)) {
		clilen = sizeof(cliaddr);
		newsockfd = accept(socket, &cliaddr, &clilen);
		if (newsockfd > 0) {
			socklist_add(newsockfd);
			socktime_set(newsockfd);
			FD_CLR(socket, &read_fd_set);
		}
		return newsockfd;
	}
	
	return 0;
	
	// TODO - handshake here? Or somewhere else?
}

int core_read(void) {
	int i, n;
	char buffer[BUFFER_SIZE];
	char command[COMMAND_SIZE + 1];
	char payload[BUFFER_SIZE - COMMAND_SIZE + 1];
	
	for (i = 0; i < FD_SETSIZE; ++i) {
		if (FD_ISSET (i, &read_fd_set)) {
			socktime_set(i);
			
			// Initialize the buffer with all integer zeros ('\0')
			memset(buffer, 0, BUFFER_SIZE);
			
			// Read from socket
			n = read(i, buffer, BUFFER_SIZE - 1);
			
			// Return error code if can't read socket
			if (n < 0)
				return -1;
			
			// EOF (0) means the other side terminated connection. Handle apropriately.
			if (n == 0) {
				if (comp_type() == SERVER) {
					close(i);
					socklist_remove(i);
					socktime_unset(i);
				} else {
					return -1;
				}

				continue;
			}
			
			// TODO - Do we want to parse and execute commands in core??
			//        Maybe create a command list/execution module.

			// Get incoming command
			memset(command, 0, sizeof(command));
			strncpy(command, buffer, COMMAND_SIZE);

			// Get incoming payload
			memset(payload, 0, sizeof(payload));
			strncpy(payload, buffer + COMMAND_SIZE, BUFFER_SIZE - COMMAND_SIZE);

			// Validate and execute command
			if (command_valid(command) == true) {
				command_execute(command, payload, i);
			}
		}
	}
	
	return 0;
}
