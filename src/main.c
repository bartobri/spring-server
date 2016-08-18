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

#include "logic/netio.h"
#include "logic/socket.h"

#include "l2/inputparser.h"
#include "l2/periodic.h"
#include "l2/command.h"
#include "l2/socket.h"
#include "l2/socketlist.h"
#include "l2/readlist.h"
#include "l2/sockettime.h"

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
	readlist_init();
	periodic_init();
	inputparser_init();
	command_init();
	socketlist_init();
	readlist_init();
	sockettime_init();

	// Execute startup proceedure
	mainsockfd = netio_startup(hostname, portno);
	
	// Check for error at startup
	if (mainsockfd < 0)
		main_shutdown(netio_get_errmsg());
	
	// Print connection message
	printf("%s on port %s\n", comp_type() == SERVER ? "Listening" : "Connected", portno);
	
	// Add main socket
	socket_set_main(mainsockfd);
	socketlist_add(mainsockfd);
	sockettime_set(mainsockfd);

	// Load client/server custom functions
	load_functions();
	
	while (true) {
		
		readlist_init();

		while ((i = socketlist_get_next()) > 0)
			readlist_add(i);
		
		r = readlist_wait();
		
		if (r < 0)
			main_shutdown("select() error");

		if (r > 0) {

			if (readlist_check(mainsockfd) && comp_type() == SERVER) {

				newsockfd = netio_accept(mainsockfd);

				if (newsockfd < 0)
					main_shutdown("accept() error");

				socketlist_add(newsockfd);
				sockettime_set(newsockfd);
				sockettime_set(mainsockfd);
				readlist_remove(mainsockfd);
			}

			while ((i = readlist_get_next()) > 0) {

				r = socket_read(i);
				
				if (r < 0)
					main_shutdown("read() error");

				if (r == 0) {

					if (comp_type() == SERVER) {
						close(i);
						socketlist_remove(i);
					} else {
						main_shutdown("Server terminated connection.");
					}
					
					continue;
				}
				
				sockettime_set(i);
				
				// Store the input that we read from the socket
				inputparser_parse_input(socket_get_buffer());
				
				// Validate and execute command
				if (command_exists(inputparser_get_command()) == true)
					command_exec(inputparser_get_command(), inputparser_get_payload(), i);
			}
		}
		
		// Run periodic function if time elapsed
		if (periodic_time_elapsed())
			periodic_exec();
		
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
	
	while ((i = socketlist_get_next()) > 0) {
		close(i);
		socketlist_remove(i);
	}

	printf("Done\n");
	
	// Shutdown
	exit(1);
}

