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
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <sys/select.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT     51717
#define BUFFER_SIZE      256
#define COMMAND_SIZE     4

#define DROP_SECONDS     10    // Number of seconds to wait for ping response before dropping conn.

#define PING_SECONDS     5     // Approximate number of seconds between each ping request to the
                               // client. Actual number may vary slightly depending on amount of
                               // chatter from other clients.

// Client state structure. Linked list. Keeps track of each client
// socket and time of last ping response.
struct cstate {
	int socket;
	char ip[16];
	time_t last_ping_time;
	struct cstate *next;
};

// Function Prototypes
void error(const char *);
void drop_unresponsive_cons(void);
void shutdown_socket(int);
void ping_all_sockets(void);
void handle_sigint(int);
void cleanup(void);

// Globals
bool verbose = true;
int mainsockfd = 0;
struct cstate *cs_start = NULL;
fd_set active_fd_set;

/*
 * int main(int, char *)
 *
 * DESCR:
 * Main program function. Watches and accepts new conections. 
 *
 */
int main(int argc, char *argv[]) {
	int newsockfd;
	int o, n, r;
	char *portno;
	char buffer[BUFFER_SIZE];
	struct sockaddr cli_addr;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	struct timeval timeout;
	struct cstate *cs_pointer = NULL;
	time_t last_ping_time = time(NULL);
	fd_set read_fd_set;

	// Set SIGINT handler
	signal(SIGINT, handle_sigint);

	// Set default port
	portno = malloc(6)
	sprintf(portno, "%i", DEFAULT_PORT);

	// Check arguments
	while ((o = getopt(argc, argv, "p:q")) != -1) {
		switch (o) {
			case 'p':
				portno = optarg;
				break;
			case 'q':
				verbose = false;
				break;
			case '?':
				if (isprint(optopt))
					fprintf (stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character '\\x%x'.\n", optopt);
				exit(1);
		}
	}

	// Initializing serv_addr memory footprint to all integer zeros ('\0')
	memset((char *) &hints, 0, sizeof(struct addrinfo));

	// Get one or moe arrdinfo structures that conforms with that provided by 'hints'
	hints.ai_family = AF_UNSPEC;              // Return IPv4 and IPv6 choices
	hints.ai_socktype = SOCK_STREAM;          // We want a TCP socket
	hints.ai_flags = AI_PASSIVE;              // All interfaces
	if (getaddrinfo(NULL, portno, &hints, &result) != 0)
		error("getaddrinfo() error");

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
	if (rp == NULL)
		error("Couldn't bind\n");

	// Free the result structure we don't need anymore
	freeaddrinfo(result);

	// Mark socket as accepting connections, up to 5 backlogged connections
	listen(mainsockfd, 5);

	// Print "listening" message
	if (verbose)
		printf("Listening on port %s\n", portno);

	// Initialize fd set
	FD_ZERO (&active_fd_set);

	// Add server address socket (main socket) to fd set
	FD_SET (mainsockfd, &active_fd_set);

	while (true) {
		read_fd_set = active_fd_set;

		// Set select() timeout value.
		// This needs to be inside the loop so it is reset for each loop interation.
		timeout.tv_sec  = (unsigned int)PING_SECONDS;
		timeout.tv_usec = 0;

		// Block until input arrives on one or more active sockets
		r = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout);

		// Evaluate select return value
		if (r < 0) {
			error("select() error");
		} else if (r == 0) {
			// select() timeout

			// Check for unresponsive connections and drop
			drop_unresponsive_cons();

			// Send heartbeat to all sockets. We can assume that the heartbeat is due since the 
			// timeout used for select() is PING_SECONDS
			ping_all_sockets();

			// Update last ping time
			last_ping_time = time(NULL);
		} else {

			// Check all sockets and service those with input pending

			if (FD_ISSET(mainsockfd, &read_fd_set)) {
				// New connection request on main socket

				socklen_t clilen;
				char *ip;

				// accept() causes the process to block (sleep) until a client connects.
				// In this case we already know there is a new connection waiting (no wait).
				// Get new connection from the socket queue and return a new file descriptor. 
				//
				// cli_addr - is filled in with the address of the connecting entity
				// http://www.linuxhowtos.org/data/6/accept.txt
				clilen = sizeof(cli_addr);
				newsockfd = accept(mainsockfd, &cli_addr, &clilen);
				if (newsockfd < 0) 
					error("accept() error");

				// Get IP address
				struct sockaddr_in *addr_in = (struct sockaddr_in *)&cli_addr;
				ip =  inet_ntoa(addr_in->sin_addr);

				// Print "accept" message
				if (verbose)
					printf("[IP %s] [Socket %i] Connection Accepted\n", ip, newsockfd);

				// Add new connection to linked list
				cs_pointer = cs_start;
				if (cs_pointer == NULL) {
					cs_pointer = malloc(sizeof(struct cstate));
					cs_start = cs_pointer;
				} else {
					// Set list pointer to last member in list
					while (cs_pointer->next != NULL) {
						cs_pointer = cs_pointer->next;
					}

					// Add new member to end of list
					cs_pointer->next = malloc(sizeof(struct cstate));
					cs_pointer = cs_pointer->next;
				}

				// Initialize connection state
				cs_pointer->socket = newsockfd;
				strcpy(cs_pointer->ip, ip);
				cs_pointer->last_ping_time = time(NULL);
				cs_pointer->next = NULL;

				// Adding new connection to fd set
				FD_SET(newsockfd, &active_fd_set);

				// Send accept message
				n = write(newsockfd, "accept", 6);

				// Print error messsage if couldn't write data
				if (n < 0)
					error("write() error");
			}

			// Check for data arriving on already-connected socket

			cs_pointer = cs_start;
			while (cs_pointer != NULL) {
				if (FD_ISSET(cs_pointer->socket, &read_fd_set)) {

					// Initialize the buffer with all integer zeros ('\0')
					memset(buffer, 0, BUFFER_SIZE);

					// Read from connection
					n = read(cs_pointer->socket, buffer, BUFFER_SIZE - 1);

					// Error if can't read socket
					if (n < 0)
						error("read() error");

					// Shut down socket if we got EOF (other side terminated connection)
					else if (n == 0)
						shutdown_socket(cs_pointer->socket);

					// Evaluate command and respond
					else {

						// Get incoming command
						char command[COMMAND_SIZE + 1];
						memset(command, 0, COMMAND_SIZE + 1);
						strncpy(command, buffer, COMMAND_SIZE);

						// Print incoming command message
						if (verbose)
							printf("[IP %s] [Socket %i] <- %s\n", cs_pointer->ip, cs_pointer->socket, command);

						// Check for ping response and update ping time
						if (strcmp(command, "ping") == 0)
							cs_pointer->last_ping_time = time(NULL);

						// Client quit
						if (strcmp(command, "quit") == 0)
							shutdown_socket(cs_pointer->socket);
					}
				}
				cs_pointer = cs_pointer->next;
			}

			// Check for unresponsive connections and drop
			drop_unresponsive_cons();

			// Send ping to all sockets if we are past the PING_SECONDS time limit
			if (last_ping_time <= time(NULL) - PING_SECONDS) {
				ping_all_sockets();
				last_ping_time = time(NULL);
			}

		}
	}

	// Cleanup
	cleanup();

	return 0; 
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
	fprintf(stderr, "%s, errno: %i\n", msg, errno);
	exit(1);
}

/*
 * void drop_unresponsive_cons(void)
 *
 * DECSR:
 * Check for and drop connections that have not responded to a ping.
 *
 */
void drop_unresponsive_cons(void) {
	struct cstate *cs_pointer = NULL;
	int droptime;

	droptime = time(NULL) - DROP_SECONDS;

	// Itterate over all list members
	cs_pointer = cs_start;
	while (cs_pointer != NULL) {

		// If hasn't responded to ping for DROP_SECONDS, kill it
		if (cs_pointer->last_ping_time < droptime)
			shutdown_socket(cs_pointer->socket);

		// Next member
		cs_pointer = cs_pointer->next;
	}
}

/*
 * void shutdown_socket(int)
 *
 * DECSR:
 * Shut down a socket. Includes removing it from linked list, removing it from
 * the fd set, and closing the socket. Must also free up memory used for list 
 * element.
 *
 */
void shutdown_socket(int socket) {
	struct cstate *cs_pointer = NULL;
	struct cstate *cs_temp = NULL;
	struct cstate *cs_prev = NULL;

	// Close socket
	close(socket);

	// remove socket from fd_set
	FD_CLR(socket, &active_fd_set);

	cs_pointer = cs_start;
	while (cs_pointer != NULL) {
		if (cs_pointer->socket == socket) {

			// Print "closing" message
			if (verbose)
				printf("[IP %s] [Socket %i] Connection Closed\n", cs_pointer->ip, socket);

			// Copy to temp pointer
			cs_temp = cs_pointer;

			// splice out this list member
			if (cs_prev == NULL)
				cs_start = cs_pointer->next;
			else
				cs_prev->next = cs_pointer->next;

			// Free allocated memory
			free(cs_temp);

			// break from loop
			break;
		} else {
			cs_prev = cs_pointer;
			cs_pointer = cs_pointer->next;
		}

	}
}

/*
 * void ping_all_sockets(void)
 *
 * DESCR:
 * Ping all client connnections. NOTE: does not handle responses.
 *
 */
void ping_all_sockets(void) {
	struct cstate *cs_pointer = NULL;
	int n;

	cs_pointer = cs_start;
	while (cs_pointer != NULL) {
		if (FD_ISSET(cs_pointer->socket, &active_fd_set)) {

			// Send ping message
			n = write(cs_pointer->socket, "ping", 4);

			// Print outgoing command message
			if (verbose)
				printf("[IP %s] [Socket %i] -> %s\n", cs_pointer->ip, cs_pointer->socket, "ping");

			// Print error messsage if couldn't write data
			if (n < 0)
				error("write() error");

		}
		cs_pointer = cs_pointer->next;
	}
}

/*
 * void handle_sigint(int)
 *
 * DESCR:
 * Overrides SIGINT (CTR-C) signal and executes block of code before exiting.
 * This mainly exists to run cleanup tasks.
 *
 */
void handle_sigint(int e) {
	fprintf(stderr, "Caught sigint (%i). Exiting\n", e);

	cleanup();

	exit(1);
}

/*
 * void cleanup(void)
 *
 * DESCR:
 * Perform cleanup tasks prior to terminating program:
 *    1. Close all client sockets.
 *    2. Free up memory used by linked list.
 *    3. Close primary socket (for new connections).
 *
 */
void cleanup(void) {
	struct cstate *cs_pointer = NULL;

	// Itterate over all list members
	cs_pointer = cs_start;
	while (cs_pointer != NULL) {
		shutdown_socket(cs_pointer->socket);
		cs_pointer = cs_pointer->next;
	}

	// Close main socket if it was established
	if (mainsockfd > 0)
		close(mainsockfd);
}
