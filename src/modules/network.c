// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include "modules/network.h"
#include "config.h"

#define IPV4_ADDRESS_LENGTH 15
#define ERRMSG_SIZE         100

// Static Variables
static char ipaddress[IPV4_ADDRESS_LENGTH + 1];
static char errmsg[ERRMSG_SIZE];
static char inputqueue[INPUT_QUEUE_SIZE][COMMAND_SIZE + PAYLOAD_SIZE + 1];

void network_init(void) {
	int i;

	memset(ipaddress, 0, IPV4_ADDRESS_LENGTH + 1);
	memset(errmsg, 0, ERRMSG_SIZE);

	for (i = 0; i < INPUT_QUEUE_SIZE; i++)
		memset(inputqueue[i], 0, COMMAND_SIZE + PAYLOAD_SIZE + 1);
}

int network_start_server(char *hostname, char *portno) {
	int startsockfd = 0;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	
	// Initializing serv_addr memory footprint to all integer zeros ('\0')
	memset((char *) &hints, 0, sizeof(struct addrinfo));
	
	// Get one or more arrdinfo structures that conforms with that provided by 'hints'
	hints.ai_family = AF_INET;                // Return IPv4 choices
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
	
	return startsockfd;
}

int network_start_client(char *hostname, char *portno) {
	int startsockfd = 0;
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
	
	return startsockfd;
}

int network_accept(int socket) {
	int newsockfd;
	struct sockaddr_in cliaddr;
	socklen_t clilen;
	
	clilen = sizeof(cliaddr);
	
	memset(ipaddress, 0, IPV4_ADDRESS_LENGTH + 1);
	
	newsockfd = accept(socket, (struct sockaddr *)&cliaddr, &clilen);

	if (newsockfd) {
		sprintf(ipaddress, "%d.%d.%d.%d",
			(int)(cliaddr.sin_addr.s_addr&0xFF),
			(int)((cliaddr.sin_addr.s_addr&0xFF00)>>8),
			(int)((cliaddr.sin_addr.s_addr&0xFF0000)>>16),
			(int)((cliaddr.sin_addr.s_addr&0xFF000000)>>24));
	}

	return newsockfd;
}

int network_read(int socket) {
	unsigned int len;
	int i, r;
	int q = 0;
	char *buffer, *bufferstart;
	
	// Set the buffer and input queue with all integer zeros ('\0')
	for (i = 0; i < INPUT_QUEUE_SIZE; i++)
		memset(inputqueue[i], 0, COMMAND_SIZE + PAYLOAD_SIZE + 1);

	// Get length of data waiting to be read
	ioctl(socket, FIONREAD, &r);

	if (r > 0) {
		// Set input buffer to length (plus 1) of waiting data
		buffer = malloc(r + 1);
		
		// Keep pointer to buffer start so we can free it
		bufferstart = buffer;
		
		// read incoming data in to buffer
		r = read(socket, buffer, r);
	
		// Parse out the incoming data and store in inputqueue array
		if (r > 0) {
			while (*buffer != '\0') {
				len = 0;
				while (*buffer >= '0' && *buffer <= '9')
					len = (len * 10) + *buffer++ - '0';
				
				if (len > 0 && *buffer == '\n' && strlen(++buffer) >= len)
					if (len <= COMMAND_SIZE + PAYLOAD_SIZE)
						strncpy(inputqueue[q++], buffer, len);
					else
						strncpy(inputqueue[q++], buffer, COMMAND_SIZE + PAYLOAD_SIZE);
				else
					break;
	
				buffer += len;
			}
		}
		
		// Free buffer
		free(bufferstart);
	}
	
	return r;
}

char *network_get_readdata(int r) {
	if (r < INPUT_QUEUE_SIZE)
		return inputqueue[r];
	else
		return "";
}

int network_write(int socket, char *data) {
	int r, l;
	char *lstr;
	
	l = strlen(data);
	
	lstr = malloc(20 + l);
	sprintf(lstr, "%i\n%s", l, data);
	
	r = write(socket, lstr, strlen(lstr));
	
	free(lstr);

	return r;
}

char *network_get_errmsg(void) {
	return errmsg;
}

char *network_get_ipaddress(void) {
	return ipaddress;
}
