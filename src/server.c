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
//#include <sys/types.h> 
//#include <sys/socket.h>
//#include <sys/time.h>
//#include <netinet/in.h>

#define DEFAULT_PORT     "51717"
#define BUFFER_SIZE	     256

#define DROP_SECONDS     10    // Number of seconds to wait for ping response before dropping conn.

#define PING_SECONDS     5     // Approximate number of seconds between each ping request to the
                               // client. Actual number may vary slightly depending on amount of
                               // chatter from other clients.

// TODO - Investigate replacing select() with libevent (libevent.org)

// TODO - call cleanup_mem on ctrl-c and error()

struct cstate {
	int socket;
	int last_ping_time;
	struct cstate *next;
};

// Function Prototypes
void error(const char *);
void drop_unresponsive_cons(struct cstate **, fd_set *);
void shutdown_socket(struct cstate **, fd_set *, int);
void ping_all_sockets(fd_set *, int);
void handle_sigint(int);
void cleanup_mem(struct cstate **);

/*
 * main function
 */
int main(int argc, char *argv[])
{
	int sockfd, newsockfd;
	char *portno;
	fd_set active_fd_set, read_fd_set;
	char buffer[BUFFER_SIZE];
	struct sockaddr cli_addr;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	struct timeval timeout;
	struct cstate *cs_pointer = NULL;
	struct cstate *cs_start = NULL;
	int o, n, i, r;
	int last_ping_time = (int)time(NULL);

	signal(SIGINT, handle_sigint);

	// Set port
	portno = DEFAULT_PORT;

	// Check arguments
	while ((o = getopt(argc, argv, "p:")) != -1) {
		switch (o) {
			case 'p':
				portno = optarg;
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
	bzero((char *) &hints, sizeof(struct addrinfo));

	// Get one or moe arrdinfo structures that conforms with that provided by 'hints'
	hints.ai_family = AF_UNSPEC;              // Return IPv4 and IPv6 choices
	hints.ai_socktype = SOCK_STREAM;          // We want a TCP socket
	hints.ai_flags = AI_PASSIVE;              // All interfaces
	if (getaddrinfo(NULL, portno, &hints, &result) != 0)
		error("getaddrinfo() error");

	// Loop over results from getaddrinfo() and try to bind. Exit loop on first successful bind.
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1)
			continue;

		if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;                           // Successful binding

		close(sockfd);
	}

	// Error if we didn't bind to any sockets
	if (rp == NULL)
		error("Couldn't bind\n");

	// Free the result structure we don't need anymore
	freeaddrinfo(result);

	// Mark socket as accepting connections, up to 5 backlogged connections
	listen(sockfd, 5);

	// Initialize fd set
	FD_ZERO (&active_fd_set);

	// Add server address socket (main socket) to fd set
	FD_SET (sockfd, &active_fd_set);

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

			// Check for non-responsive connection and drop
			drop_unresponsive_cons(&cs_start, &active_fd_set);

			// Send ping to all sockets. We can assume that PING_SECONDS has passed since
			// it is used for the timeout.
			ping_all_sockets(&active_fd_set, sockfd);
			last_ping_time = (int)time(NULL);
		} else {

			// Check all sockets and service those with input pending
			for (i = 0; i < FD_SETSIZE; ++i) {
				if (FD_ISSET(i, &read_fd_set)) {
					if (i == sockfd) {
						// Connection request on original socket (main socket)

						socklen_t clilen;

						// accept() causes the process to block (sleep) until a client connects.
						// In this case we already know there is a new connection waiting (no wait).
						// Get new connection from the socket queue and return a new file descriptor. 
						//
						// cli_addr - is filled in with the address of the connecting entity
						// http://www.linuxhowtos.org/data/6/accept.txt
						clilen = sizeof(cli_addr);
						newsockfd = accept(sockfd, &cli_addr, &clilen);
						if (newsockfd < 0) 
							error("accept() error");

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
						cs_pointer->last_ping_time = (int)time(NULL);
						cs_pointer->next = NULL;

						// Adding new connection to fd set
						FD_SET(newsockfd, &active_fd_set);

						// Send ACCEPT message
						n = write(newsockfd, "ACCEPT", 6);

						// Print error messsage if couldn't write data
						if (n < 0)
							error("write() error");
					} else {
						// Data arriving on already-connected socket
					
						// Initialize the buffer with all integer zeros ('\0')
						bzero(buffer, BUFFER_SIZE);

						// Read from connection
						n = read(i, buffer, BUFFER_SIZE - 1);

						// Error if can't read socket
						if (n < 0)
							error("read() error");

						// Shut down socket if we got EOF (other side terminated connetcion)
						else if (n == 0)
							shutdown_socket(&cs_start, &active_fd_set, i);

						// Evaluate data read from socket
						else {

							// Check for ping response and update ping time
							if (strcmp(buffer, "ping") == 0) {
								cs_pointer = cs_start;

								while (cs_pointer != NULL && cs_pointer->socket != i)
									cs_pointer = cs_pointer->next;

								if (cs_pointer != NULL)
									cs_pointer->last_ping_time = (int)time(NULL);
							}

							// Client quit
							if (strcmp(buffer, "quit") == 0)
								shutdown_socket(&cs_start, &active_fd_set, i);
						}
					}
				}
			}

			// Check for non-responsive connection and drop
			drop_unresponsive_cons(&cs_start, &active_fd_set);

			// Send ping to all sockets if we are past the PING_SECONDS time limit
			if (last_ping_time <= (int)time(NULL) - PING_SECONDS) {
				ping_all_sockets(&active_fd_set, sockfd);
				last_ping_time = (int)time(NULL);
			}

		}
	}

	// Close main socket
	close(sockfd);

	// Memory cleanup
	cleanup_mem(&cs_start);

	return 0; 
}

/*
 * Print error message and exit
 */
void error(const char *msg) {
	fprintf(stderr, "%s, errno: %i\n", msg, errno);
	exit(1);
}

/*
 * Check for and drop connections that have not responded to a ping
 */
void drop_unresponsive_cons(struct cstate **cs_start, fd_set *active_fd_set) {
	struct cstate *cs_pointer = NULL;
	int droptime;

	droptime = (int)time(NULL) - DROP_SECONDS;

	// Itterate over all list members
	cs_pointer = *cs_start;
	while (cs_pointer != NULL) {

		// If hasn't responded to ping for DROP_SECONDS, kill it
		if (cs_pointer->last_ping_time < droptime)
			shutdown_socket(cs_start, active_fd_set, cs_pointer->socket);

		// Next member
		cs_pointer = cs_pointer->next;
	}
}

void shutdown_socket(struct cstate **cs_start, fd_set *active_fd_set, int socket) {
	struct cstate *cs_pointer = NULL;
	struct cstate *cs_temp = NULL;
	struct cstate *cs_prev = NULL;

	// Close socket
	close(socket);

	// remove socket from fd_set
	FD_CLR(socket, active_fd_set);

	cs_pointer = *cs_start;
	while (cs_pointer != NULL) {
		if (cs_pointer->socket == socket) {

			// Copy to temp pointer
			cs_temp = cs_pointer;

			// splice out this list member
			if (cs_prev == NULL)
				*cs_start = cs_pointer->next;
			else
				cs_prev->next = cs_pointer->next;

			// Free allocated memory
			free(cs_temp);

			// break from loop
			cs_pointer = NULL;
		} else {
			cs_prev = cs_pointer;
			cs_pointer = cs_pointer->next;
		}

	}
}

/*
 * Ping all socket connnections except sockfd (main socket)
 */
void ping_all_sockets(fd_set *read_fd_set, int sockfd) {
	int i, n;

	for (i = 0; i < FD_SETSIZE; ++i) {
		if (FD_ISSET(i, read_fd_set)) {

			// Skip the original socket
			if (i == sockfd)
				continue;

			// Send ping message
			n = write(i, "ping", 4);

			// Print error messsage if couldn't write data
			if (n < 0)
				error("write() error");

		}
	}
}

/*
 * SIGINT handler. Mainly exists to do memory cleanup before exiting.
 */
void handle_sigint(int e) {
	fprintf(stderr, "Caught sigint (%i). Exiting\n", e);
	exit(1);
}

/*
 * Memory cleanup
 */
void cleanup_mem(struct cstate **cs_start) {
	struct cstate *cs_pointer = NULL;
	struct cstate *cs_temp = NULL;

	cs_pointer = *cs_start;
	while (cs_pointer != NULL) {
		cs_temp = cs_pointer;
		cs_pointer = cs_pointer->next;
		free(cs_temp);
	}

	*cs_start = NULL;
}
