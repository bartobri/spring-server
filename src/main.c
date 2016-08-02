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
#include "main.h"
#include "core.h"
#include "socktime.h"
#include "command.h"
#include "socklist.h"

// Function prototypes
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
	int o;
	int waitVal;
	char *hostname, *portno;
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
	// TODO - Remove return val after error module implemented.
	//        Have core module throw errors. Remove error checking from main.
	mainsockfd = core_startup(hostname, portno);
	
	// Ensure we have a valid socket
	if (mainsockfd < 0)
		error("Unsuccessful startup.");
	
	// Print connection message
	printf("%s on port %s\n", comp_type() == SERVER ? "Listening" : "Connected", portno);
		
	// Initialize socket list and add main socket
	socklist_init();
	socklist_add(mainsockfd);

	// Load client/server commands
	load_commands();
	
	while (true) {
		waitVal = core_wait();

		if (waitVal < 0)
			error("core_wait() error.");
		
		if (waitVal > 0) {
			if (comp_type() == SERVER)
				if (core_accept(mainsockfd) < 0)
					error("core_accept() error.");

			if (core_read() < 0) {
				// TODO - When server closes socket on client, this msg displays.
				error("core_read() error.");
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
	int i;

	while ((i = socklist_next()) > 0) {
		close(i);
		socklist_remove(i);
		socktime_unset(i);
	}
}
