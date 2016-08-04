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
#include "netio.h"
#include "command.h"
#include "socktime.h"
#include "socklist.h"
#include "sockmain.h"
#include "readlist.h"

static char buffer[BUFFER_SIZE];

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
			fprintf(stderr, "Could not obtain internet address info.\n");
			netio_shutdown();
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
			fprintf(stderr, "server: Could not bind to socket %i\n", startsockfd);
			netio_shutdown();
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
			fprintf(stderr, "client: hostname can not be NULL.\n");
			netio_shutdown();
		}
		
		// Set up a socket in the AF_INET domain (Internet Protocol v4 addresses)
		startsockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (startsockfd < 0) {
			fprintf(stderr, "client: Could not create socket\n");
			netio_shutdown();
		}
		
		// Get a pointer to 'hostent' containing info about host.
		server = gethostbyname(hostname);
		if (server == NULL) {
			fprintf(stderr, "client: no such host: %s\n", hostname);
			netio_shutdown();
		}
		
		// Initializing serv_addr memory footprint to all integer zeros ('\0')
		memset(&serv_addr, 0, sizeof(serv_addr));
		
		// Setting up our serv_addr structure
		serv_addr.sin_family = AF_INET;            // Internet Protocol v4 addresses
		memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length); // Copy address
		serv_addr.sin_port = htons(atoi(portno));  // Convert port byte order to 'network byte order'
		
		// Connect to server. Error if can't connect.
		if (connect(startsockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
			fprintf(stderr, "client: error conecting to host %s port %s\n", hostname, portno);
			netio_shutdown();
		}
	}
	
	return startsockfd;
}

int netio_wait(fd_set *read_fd_set) {
	int r;
	struct timeval timeout;
		
	// Set select() timeout value.
	// This needs to be inside the loop so it is reset for each loop interation.
	timeout.tv_sec  = (unsigned int)PERIODIC_SECONDS;
	timeout.tv_usec = 0;
		
	// Block until input arrives on one or more active sockets
	r = select(FD_SETSIZE, read_fd_set, NULL, NULL, &timeout);
	
	if (r < 0) {
		fprintf(stderr, "select() error.\n");
		netio_shutdown();
	}
	
	return r;
}

int netio_accept(int socket) {
	int newsockfd;
	struct sockaddr cliaddr;
	socklen_t clilen;
	
	clilen = sizeof(cliaddr);
	
	newsockfd = accept(socket, &cliaddr, &clilen);
	
	if (newsockfd < 0) {
		fprintf(stderr, "accept() error.\n");
		netio_shutdown();
	}
	
	return newsockfd;
	// TODO - handshake here? Or somewhere else?
}

int netio_read(int socket) {
	int n;
		
	// Reset the buffer with all integer zeros ('\0')
	memset(buffer, 0, BUFFER_SIZE);
	
	// Read from socket
	n = read(socket, buffer, BUFFER_SIZE - 1);
	
	// Return error code if can't read socket
	if (n < 0) {
		fprintf(stderr, "read() error.\n");
		netio_shutdown();
	}
	
	return n;
}

char *netio_get(void) {
	return buffer;
}

void netio_shutdown(void) {
	int i;
	
	fprintf(stderr, "Shutting down... ");

	// Cleanup tasks
	while ((i = socklist_next()) > 0) {
		close(i);
		socklist_remove(i);
		socktime_clear(i);
	}

	fprintf(stderr, "Done\n");
	
	// Shutdown
	exit(1);
}
