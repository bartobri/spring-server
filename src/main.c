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

#include "if/readlist.h"
#include "if/ptime.h"

#include "logic/netio.h"
#include "logic/socket.h"
#include "logic/comfunction.h"
#include "logic/prdfunction.h"

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
	comfunction_init();
	prdfunction_init();
	socket_init();
	readlist_init();
	ptime_init();

	// Execute startup proceedure
	mainsockfd = netio_startup(hostname, portno);
	
	// Check for error at startup
	if (mainsockfd < 0)
		main_shutdown(netio_get_errmsg());
	
	// Print connection message
	printf("%s on port %s\n", comp_type() == SERVER ? "Listening" : "Connected", portno);
	
	// Add main socket
	socket_add_main(mainsockfd);

	// Load client/server custom functions
	load_functions();
	
	while (true) {
		
		readlist_set(socket_get_list());
		
		r = netio_wait(readlist_getptr());
		
		if (r < 0)
			main_shutdown("select() error");

		if (r > 0) {

			if (readlist_check(mainsockfd) && comp_type() == SERVER) {

				newsockfd = netio_accept(mainsockfd);

				if (newsockfd < 0)
					main_shutdown("accept() error");

				socket_add(newsockfd);
				readlist_remove(mainsockfd);
			}

			while ((i = readlist_next()) > 0) {

				r = socket_read(i);
				
				if (r < 0)
					main_shutdown("read() error");

				if (r == 0) {

					if (comp_type() == SERVER) {
						close(i);
						socket_remove(i);
					} else {
						main_shutdown("Server terminated connection.");
					}
					
					continue;
				}
				
				// Validate and execute command
				if (comfunction_exists(socket_get_command()) == true) {
					comfunction_exec(socket_get_command(), socket_get_payload(), i);
				}
			}
		}
		
		// Run periodic function if expired
		if (ptime_expired() == true) {
			prdfunction_exec();
			ptime_reset();
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
	
	printf("%s\n", errmsg);
	printf("Shutting down... ");

	socket_shutdown_all();

	printf("Done\n");
	
	// Shutdown
	exit(1);
}

