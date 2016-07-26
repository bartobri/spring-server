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
#include "main.h"

// Function prototypes
void set_last_seen(int);
void error(const char *);
void handle_sigint(int);
void cleanup(void);

// Globals
int mainsockfd;

/*
 * int main(int, char *)
 *
 * DESCR:
 * Main program function. Watches and accepts new conections. 
 *
 */
int main(int argc, char *argv[]) {
	int o, r, i, c;
	char *hostname, *portno;
	fd_set active_fd_set, read_fd_set;
	struct timeval timeout;
	time_t last_periodic_time = time(NULL);
	
	// Create and initialize command table
	struct commandTable commands[COMMAND_LIMIT];
	initCommands(commands);
	
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
	
	// Initialize our lastseen table pointer
	ls_start = NULL;

	// Execute startup proceedure
	mainsockfd = startup(hostname, portno);
	if (mainsockfd > 0)
		printf("Connected on port %s\n", portno);
	else
		error("Unsuccessful startup.");
		
	// Initialize fd set and add main socket
	FD_ZERO (&active_fd_set);
	FD_SET (mainsockfd, &active_fd_set);

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
					
					// Update lastseen table
					set_last_seen(i);

					// Handle new server connections here in main.c
					if (i == mainsockfd && comp_type() == SERVER) {
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
						
						// Update lastseen table
						set_last_seen(newsockfd);
						
						// TODO - handshake here?
					
					// Parse incoming data
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
								// TODO - remove from socket_state list
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

						// Loop over registered commands and execute the matching one
						for (c = 0; c < COMMAND_LIMIT; ++c) {
							if (commands[c].command == NULL)
								continue;

							if (strcmp(commands[c].command, command) == 0) {
								commands[c].functionPtr(i, payload, &active_fd_set);
								break;
							} 
						}
					}
				}
			}
		}
		
		// Run periodic function if PERIODIC_SECONDS has elapsed
		if (last_periodic_time <= time(NULL) - PERIODIC_SECONDS) {
			periodic(mainsockfd, &active_fd_set);
			// TODO - evaluate return value of periodic
			last_periodic_time = time(NULL);
		}
	}

	return 0;
}

/*
 * void update_last_seen(int)
 *
 * DESCR:
 * Updates a table of all sockets with the last time communication
 * was received for a given socket number.
 *
 * ARGS:
 * int socket - socket to be updated
 *
 */
 // TODO - remove printf statements in this function
void set_last_seen(int socket) {
	struct lastseen *ls_pointer;
	
	// If list is empty, create first record.
	if (ls_start == NULL) {
		ls_start = malloc(sizeof(struct lastseen));
		ls_start->socket = socket;
		ls_start->last_time = time(NULL);
		ls_start->next = NULL;
		//printf("updated lastseen for socket %i (first entry)\n", ls_start->socket);
		return;
	}
	
	// Loop over list and either update existing record of create new
	ls_pointer = ls_start;
	while (true) {
		
		//Update record if exists
		if (ls_pointer->socket == socket) {
			ls_pointer->last_time = time(NULL);
			//printf("updated lastseen for socket %i (existing entry)\n", ls_pointer->socket);
			break;
		}
		
		// Append new record for socket
		if (ls_pointer->next == NULL) {
			ls_pointer->next = malloc(sizeof(struct lastseen));
			ls_pointer = ls_pointer->next;
			ls_pointer->socket = socket;
			ls_pointer->last_time = time(NULL);
			ls_pointer->next = NULL;
			//printf("updated lastseen for socket %i (new entry)\n", ls_pointer->socket);
			break;
		}

		// Get next record
		//printf("Slipping record for socket %i\n", ls_pointer->socket);
		ls_pointer = ls_pointer->next;
	}
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
	// Execute component cleanup routine
	comp_cleanup();

	// main cleanup
	close(mainsockfd);
}
