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
#include "readlist.h"
#include "sockmain.h"
#include "buffer.h"

// Function prototypes
void handle_sigint(int);

/*
 * int main(int, char *)
 *
 * DESCR:
 * Main program function. Watches and accepts new conections. 
 *
 */
int main(int argc, char *argv[]) {
	int o, r, i;
	int mainsockfd, newsockfd;
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
	mainsockfd = netio_startup(hostname, portno);
	
	// Print connection message
	printf("%s on port %s\n", comp_type() == SERVER ? "Listening" : "Connected", portno);
	
	// Make main socket accessible to other modules via sockmain module
	sockmain_set(mainsockfd);
		
	// Initialize socket list and add main socket
	socklist_init();
	socklist_add(mainsockfd);

	// Load client/server commands
	load_commands();
	
	while (true) {
		
		readlist_set(socklist_get());
		
		r = netio_wait(readlist_getptr());

		if (r > 0) {

			if (readlist_check(mainsockfd) && comp_type() == SERVER) {
				newsockfd = netio_accept(mainsockfd);
				socklist_add(newsockfd);
				socktime_set(newsockfd);
				readlist_remove(mainsockfd);
			}

			while ((i = readlist_next()) > 0) {

				socktime_set(i);

				// TODO - change this to return contents of buffer once I have a
				//        error module set up.
				r = netio_read(i);

				if (r == 0) {

					if (comp_type() == SERVER) {
						close(i);
						socklist_remove(i);
						socktime_clear(i);
					} else {
						// TODO - replace this once I figure out a error-and-shutdown process
						fprintf(stderr, "Server terminated connection.\n");
						netio_shutdown();
					}
					
					continue;
				}
				
				// Copy data read from netio_read in to buffer module
				buffer_set(netio_get());
				
				// Validate and execute command
				if (command_valid(buffer_get_command()) == true) {
					command_execute(buffer_get_command(), buffer_get_payload(), i);
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

