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
#include <signal.h>
#include <time.h>
#include "main.h"
#include "netio.h"
#include "socktime.h"
#include "ctable.h"
#include "socklist.h"
#include "readlist.h"
#include "sockmain.h"
#include "buffer.h"
#include "ptime.h"

// Function prototypes
void main_sigint(int);
void main_shutdown(const char *);

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
	
	// Set SIGINT handler
	signal(SIGINT, main_sigint);

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
	
	// Initialization functions
	netio_init();
	ctable_init();
	sockmain_init();
	socktime_init();
	socklist_init();
	readlist_init();
	buffer_init();
	ptime_init();

	// Execute startup proceedure
	mainsockfd = netio_startup(hostname, portno);
	
	// Check for error at startup
	if (mainsockfd < 0)
		main_shutdown(netio_get_errmsg());
	
	// Print connection message
	printf("%s on port %s\n", comp_type() == SERVER ? "Listening" : "Connected", portno);
	
	// Make main socket accessible to other modules via sockmain module
	sockmain_set(mainsockfd);
		
	// Add main socket to socket list
	socklist_add(mainsockfd);

	// Load client/server commands
	load_commands();
	
	while (true) {
		
		readlist_set(socklist_get());
		
		r = netio_wait(readlist_getptr());
		
		if (r < 0)
			main_shutdown("select() error\n");

		if (r > 0) {

			if (readlist_check(mainsockfd) && comp_type() == SERVER) {

				newsockfd = netio_accept(mainsockfd);

				if (newsockfd < 0)
					main_shutdown("accept() error\n");

				socklist_add(newsockfd);
				socktime_set(newsockfd);
				readlist_remove(mainsockfd);
			}

			while ((i = readlist_next()) > 0) {

				socktime_set(i);

				// TODO - change this to return contents of buffer once I have a
				//        error module set up.
				r = netio_read(i);
				
				if (r < 0)
					main_shutdown("read() error\n");

				if (r == 0) {

					if (comp_type() == SERVER) {
						close(i);
						socklist_remove(i);
						socktime_clear(i);
					} else {
						main_shutdown("Server terminated connection.\n");
					}
					
					continue;
				}
				
				// Copy data read from netio_read in to buffer module
				buffer_set(netio_get());
				
				// Validate and execute command
				if (ctable_check(buffer_get_command()) == true) {
					ctable_execute(buffer_get_command(), buffer_get_payload(), i);
				}
			}
		}
		
		// Run periodic function if PERIODIC_SECONDS has elapsed
		if (ptime_get() <= time(NULL) - PERIODIC_SECONDS) {
			periodic();
			ptime_set(time(NULL));
		}
		
	}

	return 0;
}

/*
 * void main_sigint(int)
 *
 * DESCR:
 *
 */
void main_sigint(int e) {
	(void)e;
	main_shutdown("Caught sigint.");
}

void main_shutdown(const char *errmsg) {
	int i;
	
	printf("%s\n", errmsg);
	printf("Shutting down... ");

	// Cleanup tasks
	while ((i = socklist_next()) > 0) {
		close(i);
		socklist_remove(i);
		socktime_clear(i);
	}

	printf("Done\n");
	
	// Shutdown
	exit(1);
}

