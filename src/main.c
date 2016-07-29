// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "main.h"
#include "sockstate.h"
#include "commands.h"

// Function prototypes
int startup(char *, char *);
void accept_new_connection(void);
void error(const char *);
void handle_sigint(int);
void cleanup(void);

/*
 * int main(int, char *)
 *
 * DESCR:
 * Main program function. Watches and accepts new conections. 
 *
 */
int main(int argc, char *argv[]) {
	int o, r, i;
	char *hostname, *portno;
	fd_set read_fd_set;
	struct timeval timeout;
	time_t last_periodic_time = time(NULL);
	
	// Set SIGINT handler
	signal(SIGINT, handle_sigint);

	// Set default port
	portno = malloc(6);
	sprintf(portno, "%i", DEFAULT_PORT);

	// Set default hostname
	hostname = malloc(65);
	hostname = DEFAULT_HOST;

	// Check arguments
	while ((o = getopt(argc, argv, "h:p:")) != -1) {
		switch (o) {
			case 'p':
				portno = optarg;
				break;
			case 'h':
				hostname = optarg;
				break;
			case '?':
				if (isprint(optopt))
					printf ("Unknown option '-%c'.\n", optopt);
				else
					printf ("Unknown option character '\\x%x'.\n", optopt);
				exit(1);
		}
	}

	// Execute startup proceedure
	mainsockfd = startup(hostname, portno);
	
	// Print connection message
	printf("Connected on port %s\n", portno);
		
	// Initialize fd set and add main socket
	FD_ZERO (&active_fd_set);
	FD_SET (mainsockfd, &active_fd_set);

	// Load client/server commands
	load_commands();

	// Main connection handling loop
	while (true) {
		read_fd_set = active_fd_set;
		
		// Set select() timeout value.
		// This needs to be inside the loop so it is reset for each loop interation.
		timeout.tv_sec  = (unsigned int)PERIODIC_SECONDS;
		timeout.tv_usec = 0;
		
		// Block until input arrives on one or more active sockets
		r = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout);
		
		// Check for select() error
		if (r < 0)
			error("select() error.");

		// If input is pending, check all sockets and service those with input
		if (r > 0) {
			for (i = 0; i < FD_SETSIZE; ++i) {
				if (FD_ISSET (i, &read_fd_set)) {
					
					// Update last_time in sockstate table
					set_sockstate_last_time(i);

					// Check if this is a new connection request on the
					// server's main socket and accept it.
					if (i == mainsockfd && comp_type() == SERVER) {
						accept_new_connection();
					
					// Otherwise, parse and evaluate incoming data
					} else {
						
						int n;
						char buffer[BUFFER_SIZE];
						char command[COMMAND_SIZE + 1];
						char payload[BUFFER_SIZE - COMMAND_SIZE + 1];
						
						// Initialize the buffer with all integer zeros ('\0')
						memset(buffer, 0, BUFFER_SIZE);
						
						// Read from connection
						n = read(i, buffer, BUFFER_SIZE - 1);
						
						// Error if can't read socket
						if (n < 0) {
							fprintf(stderr, "%s: Could not read from socket.\n", comp_type() == SERVER ? "server" : "client");
							error("read() error");
						}
						
						// EOF means the other side terminated connection. Handle apropriately.
						if (n == 0) {
							if (comp_type() == SERVER) {
								close(i);
								FD_CLR(i, &active_fd_set);
								del_sockstate_record(i);
							} else {
								error("Server terminated connection.");
							}

							continue;
						}
						
						// Get incoming command
						memset(command, 0, sizeof(command));
						strncpy(command, buffer, COMMAND_SIZE);

						// Get incoming payload
						memset(payload, 0, sizeof(payload));
						strncpy(payload, buffer + COMMAND_SIZE, BUFFER_SIZE - COMMAND_SIZE);

						// Validate and execute command
						if (valid_command(command) == true) {
							execute_command(command, payload, i);
						}
					}
				}
			}
		}
		
		// Run periodic function if PERIODIC_SECONDS has elapsed
		if (last_periodic_time <= time(NULL) - PERIODIC_SECONDS) {
			periodic();
			// TODO - evaluate return value of periodic
			last_periodic_time = time(NULL);
		}
	}

	return 0;
}

int startup(char *hostname, char *portno) {
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
			fprintf(stderr, "server: Could not obtain internet address info.\n");
			error("Unsuccessful startup.");
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
			error("Unsuccessful startup.");
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
			error("Unsuccessful startup.");
		}
		
		// Set up a socket in the AF_INET domain (Internet Protocol v4 addresses)
		startsockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (startsockfd < 0) {
			fprintf(stderr, "client: Could not create socket\n");
			error("Unsuccessful startup.");
		}
		
		// Get a pointer to 'hostent' containing info about host.
		server = gethostbyname(hostname);
		if (server == NULL) {
			fprintf(stderr, "client: no such host: %s\n", hostname);
			error("Unsuccessful startup.");
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
			error("Unsuccessful startup.");
		}
	} else {
		error("Unknown component type.");
	}
	
	return startsockfd;
}

void accept_new_connection(void) {
	int newsockfd;
	struct sockaddr cliaddr;
	socklen_t clilen;
	
	clilen = sizeof(cliaddr);
	newsockfd = accept(mainsockfd, &cliaddr, &clilen);

	if (newsockfd < 0) {
		fprintf(stderr, "server: Could not accept new connection.\n");
		error("accept() error.");
	}
	
	// Adding new connection to fd set
	FD_SET(newsockfd, &active_fd_set);
	
	// Update last_time in sockstate table
	set_sockstate_last_time(newsockfd);
	
	// TODO - handshake here?
}

/*
 * void error(const char *)
 *
 * DESCR:
 * Print passed error string and exit
 *
 * ARGS:
 * const char *msg - Error string
 *
 */
void error(const char *msg) {
	printf("%s Shutting down.\n", msg);
	cleanup();
	exit(1);
}

/*
 * void handle_sigint(int)
 *
 * DESCR:
 *
 */
void handle_sigint(int e) {
	printf("Caught sigint (%i). Shutting down.\n", e);
	cleanup();
	exit(1);
}

/*
 * void cleanup(void)
 *
 * DESCR:
 *
 */
void cleanup(void) {

	// main cleanup
	close(mainsockfd);
	
	// TODO - close and free sockstate table
}
