// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include "main.h"

int startup(char *hostname, char *portno) {
	int mainsockfd;
	struct hostent *server;
	struct sockaddr_in serv_addr;
	
	// Require hostname and port
	if (hostname == NULL) {
		fprintf(stderr, "client: hostname can not be NULL.\n");
		return -1;
	}
	
	// Set up a socket in the AF_INET domain (Internet Protocol v4 addresses)
	mainsockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (mainsockfd < 0) {
		fprintf(stderr, "client: Could not create socket\n");
		return -1;
	}
	
	// Get a pointer to 'hostent' containing info about host.
	server = gethostbyname(hostname);
	if (server == NULL) {
		fprintf(stderr, "client: no such host: %s\n", hostname);
		return -1;
	}
	
	// Initializing serv_addr memory footprint to all integer zeros ('\0')
	memset(&serv_addr, 0, sizeof(serv_addr));
	
	// Setting up our serv_addr structure
	serv_addr.sin_family = AF_INET;            // Internet Protocol v4 addresses
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length); // Copy address
	serv_addr.sin_port = htons(atoi(portno));  // Convert port byte order to 'network byte order'
	
	// Connect to server. Error if can't connect.
	if (connect(mainsockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		fprintf(stderr, "client: error conecting to host %s port %s\n", hostname, portno);
		return -1;
	}
		
	return mainsockfd;
}

void populate_commands(struct commandTable *commands) {
	// Populate commands
}

int periodic(int mainsockfd,  fd_set *active_fd_set) {

	if (write(mainsockfd, "beat", 4) < 0)
		return -1;
	
	return 0;
}


void comp_cleanup(void) {
	// component cleanup here
}

int comp_type(void) {
	return CLIENT;
}
