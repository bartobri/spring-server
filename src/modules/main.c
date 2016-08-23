// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>
#include "modules/network.h"
#include "modules/periodic.h"
#include "modules/command.h"
#include "modules/socketlist.h"
#include "modules/readlist.h"
#include "modules/sockettime.h"
#include "modules/nextperiodic.h"
#include "modules/inputcommand.h"
#include "modules/inputpayload.h"
#include "modules/mainsocket.h"
#include "modules/main.h"
#include "config.h"

// Function prototypes
void main_init(void);
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
	main_init();

	// Execute startup proceedure
	if (IS_SERVER)
		mainsockfd = network_start_server(hostname, portno);
	else
		mainsockfd = network_start_client(hostname, portno);
	
	// Check for error at startup
	if (mainsockfd < 0)
		main_shutdown(network_get_errmsg());
	
	// Print connection message
	printf("%s on port %s\n", IS_SERVER ? "Listening" : "Connected", portno);
	
	// Add main socket
	mainsocket_set(mainsockfd);
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

			if (readlist_check(mainsockfd) && IS_SERVER) {
				
				sockettime_set(mainsockfd);

				newsockfd = network_accept(mainsockfd);

				if (newsockfd < 0)
					main_shutdown("accept() error");

				socketlist_add(newsockfd);
				sockettime_set(newsockfd);
				readlist_remove(mainsockfd);
				
				// Send greeting
				network_write(newsockfd, "helo");
			}

			while ((i = readlist_get_next()) > 0) {

				r = network_read(i);
				
				if (r < 0)
					main_shutdown("read() error");

				if (r == 0) {

					if (IS_SERVER) {
						close(i);
						socketlist_remove(i);
					} else {
						main_shutdown("Server terminated connection.");
					}
					
					continue;
				}
				
				sockettime_set(i);
				
				inputcommand_parse(network_get_readdata());
				inputpayload_parse(network_get_readdata());
				
				// Validate and execute command
				if (command_exists(inputcommand_get()))
					command_exec(inputcommand_get(), inputpayload_get(), i);
			}
		}
		
		// Run periodic function if time elapsed
		if (nextperiodic_elapsed()) {
			periodic_exec();
			nextperiodic_reset();
		}
		
	}

	return 0;
}


void main_init(void) {
	network_init();
	readlist_init();
	periodic_init();
	command_init();
	socketlist_init();
	readlist_init();
	sockettime_init();
	nextperiodic_init();
	inputcommand_init();
	inputpayload_init();
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

