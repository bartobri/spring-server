// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "main.h"
#include "sockstate.h"

int command_quit(int, char *, fd_set *);
int command_beat(int, char *, fd_set *);

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

void populate_commands(struct commandTable *commands) {
	// Populate commands
	commands[0].command = "quit";
	commands[0].functionPtr = &command_quit;
	commands[1].command = "beat";
	commands[1].functionPtr = &command_beat;
}

int periodic(int mainsockfd,  fd_set *active_fd_set) {
	
	// Check time for all sockets and close unresponsive ones
	int i;
	for (i = 0; i < FD_SETSIZE; ++i) {
		if (FD_ISSET (i, active_fd_set) && i != mainsockfd) {
			if (get_sockstate_last_time(i) < time(NULL) - (PERIODIC_SECONDS * 2)) {
				close(i);
				FD_CLR(i, active_fd_set);
				del_sockstate_record(i);
			}
		}
	}
	
	return 0;
}

int comp_type(void) {
	return SERVER;
}

int command_quit(int socket, char *payload, fd_set *active_fd_set) {
	
	// Close socket
	close(socket);

	// remove socket from fd_set
	FD_CLR(socket, active_fd_set);
	
	return 0;
}

int command_beat(int socket, char *payload, fd_set *active_fd_set) {
	printf("command_beat socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}
