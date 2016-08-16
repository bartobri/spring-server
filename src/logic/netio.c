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
#include "logic/netio.h"

#define WAIT_SECONDS 5
#define ERRMSG_SIZE  100

// Static Variables
static char errmsg[ERRMSG_SIZE];

void netio_init(void) {
	memset(errmsg, 0, ERRMSG_SIZE);
}

int netio_startup(char *hostname, char *portno) {
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
			sprintf(errmsg, "Could not obtain internet address info.");
			return -1;
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
			sprintf(errmsg, "Could not bind to socket %i", startsockfd);
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
			sprintf(errmsg, "hostname can not be NULL");
			return -1;
		}
		
		// Set up a socket in the AF_INET domain (Internet Protocol v4 addresses)
		startsockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (startsockfd < 0) {
			sprintf(errmsg, "Could not create socket");
			return -1;
		}
		
		// Get a pointer to 'hostent' containing info about host.
		server = gethostbyname(hostname);
		if (server == NULL) {
			sprintf(errmsg, "no such host: %s", hostname);
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
			sprintf(errmsg, "error conecting to host %s port %s", hostname, portno);
			return -1;
		}
	}
	
	return startsockfd;
}

int netio_wait(fd_set *read_fd_set) {
	int r;
	struct timeval timeout;
		
	// Set select() timeout value.
	// This needs to be inside the loop so it is reset for each loop interation.
	timeout.tv_sec  = (unsigned int)WAIT_SECONDS;
	timeout.tv_usec = 0;
		
	// Block until input arrives on one or more active sockets
	r = select(FD_SETSIZE, read_fd_set, NULL, NULL, &timeout);
	
	return r;
}

int netio_accept(int socket) {
	int newsockfd;
	struct sockaddr cliaddr;
	socklen_t clilen;
	
	clilen = sizeof(cliaddr);
	
	newsockfd = accept(socket, &cliaddr, &clilen);
	
	return newsockfd;
	// TODO - handshake here? Or somewhere else?
}

char *netio_get_errmsg(void) {
	return errmsg;
}
