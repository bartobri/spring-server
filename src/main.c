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
#include "netio.h"
#include "socktime.h"
#include "command.h"
#include "socklist.h"
#include "sockmain.h"

// Function prototypes
void error(const char *);
void handle_sigint(int);

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
	netio_startup(hostname, portno);
	
	// TODO - check for startup error once error module implemented
	
	// Print connection message
	printf("%s on port %s\n", comp_type() == SERVER ? "Listening" : "Connected", portno);
		
	// Initialize socket list and add main socket
	socklist_init();
	socklist_add(sockmain_get());

	// Load client/server commands
	load_commands();
	
	while (true) {
		waitVal = netio_wait();

		if (waitVal < 0)
			error("netio_wait() error.");
		
		if (waitVal > 0) {
			if (comp_type() == SERVER)
				if (netio_accept() < 0)
					error("netio_accept() error.");

			if (netio_read() < 0) {
				// TODO - When server closes socket on client, this msg displays.
				error("netio_read() error.");
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
	netio_shutdown();
}

/*
 * void handle_sigint(int)
 *
 * DESCR:
 *
 */
void handle_sigint(int e) {
       printf("Caught sigint (%i).\n", e);
       netio_shutdown();
}

