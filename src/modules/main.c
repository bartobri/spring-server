// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include "modules/network.h"
#include "modules/periodic.h"
#include "modules/command.h"
#include "modules/connectfunction.h"
#include "modules/disconnectfunction.h"
#include "modules/socketlist.h"
#include "modules/readlist.h"
#include "modules/sockettime.h"
#include "modules/nextperiodic.h"
#include "modules/inputcommand.h"
#include "modules/inputpayload.h"
#include "modules/mainsocket.h"
#include "modules/termflag.h"
#include "modules/log.h"
#include "modules/main.h"
#include "config.h"

// Function prototypes
void main_init(void);
void main_sigint(int);
void main_shutdown(const char *);

/*
 * The main function handles program initialization and network connection,
 * argument checking, and manages the main program loop. Inside the loop
 * it coordinates the flow of data between the modules, does error checking,
 * and logs pertinent info.
 */
int main(int argc, char *argv[]) {
	int o, r, s, i;
	char *hostname, *portno;
	
	// Initialization functions
	main_init();
	
	// Open log file
	if (IS_SERVER)
		r = log_open_server();
	else
		r = log_open_client();
	
	// Ensure log was successfully opened
	if (r < 0)
		main_shutdown(log_get_errmsg());
	
	// Log startup message
	log_write("Starting Up");
	
	// Set SIGINT handler
	signal(SIGINT, main_sigint);

	// Set default port
	portno = malloc(6);
	sprintf(portno, "%i", DEFAULT_PORT);

	// Set default hostname
	hostname = malloc(65);
	if (strlen(DEFAULT_HOST) == 0)
		hostname = NULL;
	else
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
					log_print("Unknown option '-%c'.", optopt);
				else
					log_print("Unknown option character '\\x%x'.", optopt);
				main_shutdown("Invalid command option(s).");
		}
	}
	
	// Log host and port
	log_write("host=%s port=%s", hostname, portno);

	// Execute network startup proceedure
	if (IS_SERVER)
		r = network_start_server(hostname, portno);
	else
		r = network_start_client(hostname, portno);
	
	// Check for error at startup
	if (r < 0)
		main_shutdown(network_get_errmsg());
	
	// Print connection message
	log_print("%s on port %s", IS_SERVER ? "Listening" : "Connected", portno);
	
	// Set main socket
	mainsocket_set(r);

	// Add main socket to socket list and track it's time
	socketlist_add(mainsocket_get());
	sockettime_set(mainsocket_get());
	
	// main program loop
	while (1) {
		
		// Initialize (empty) readlist
		readlist_init();

		// Copy all active socket numbers to readlist
		while ((s = socketlist_get_next()) > 0)
			readlist_add(s);
		
		// Wait for incoming data on one of the readlist sockets or
		// timeout after the number of seconds configured for PERIODIC_SECONDS
		r = readlist_wait();
		
		// Shutdown if we get an error code
		if (r < 0)
			main_shutdown("select() error");

		// If we get incoming data on the main socket for the server
		// then we have a new client attempting to connect.
		if (IS_SERVER && readlist_check(mainsocket_get())) {
			
			// Update main socket time
			sockettime_set(mainsocket_get());

			// Accept new connection
			r = network_accept(mainsocket_get());

			// Shut down if we get an error code
			if (r < 0)
				main_shutdown("accept() error");

			// Add new socket to our socket list
			socketlist_add(r);
			sockettime_set(r);
			
			// Remove main socket from our list now that we just serviced it
			readlist_remove(mainsocket_get());
			
			// Log socket assignment and IP
			log_write("Client connected from %s. Assigned socket %i.", network_get_ipaddress(), r);
			
			// Execute connect function for new socket/client
			connectfunction_exec(r);
			
			// Check if termflag was set in connect function
			if (termflag_isset())
				main_shutdown("Terminated.");
		}

		// Loop over readlist and service each socket
		while ((s = readlist_get_next()) > 0) {

			// Read data from socket
			r = network_read(s);
			
			// Shut down if we get an error code
			if (r < 0)
				main_shutdown("read() error");

			// Check if socket terminated the connection
			if (r == 0) {

				// Log and close socket on the server side. Shut down on the
				// client side.
				if (IS_SERVER) {
					log_write("Client terminated connection. Closing socket %i.", s);
					network_close(s);
					socketlist_remove(s);
				} else
					main_shutdown("Server terminated connection.");
				
				// Execute disconnect function
				disconnectfunction_exec(s);
				
				// Check if termflag was set in disconnect function
				if (termflag_isset())
					main_shutdown("Terminated.");
				
				// Force next loop iteration.
				continue;
			}
			
			// Update socket time
			sockettime_set(s);

			// Loop over each command/payload pair sent from the socket
			for (i = 0; strlen(network_get_readdata(i)) > 0; ++i) {

				// Parse out the command and payload from the data
				inputcommand_parse(network_get_readdata(i));
				inputpayload_parse(network_get_readdata(i));

				// Log the command we received
				log_write("Received command %s from socket %i", inputcommand_get(), s);
				
				// Validate and execute command
				if (command_exists(inputcommand_get()))
					command_exec(inputcommand_get(), inputpayload_get(), s);
					
				// Check if termflag was set in command function
				if (termflag_isset())
					main_shutdown("Terminated.");
			}
		}
		
		// Close all sockets whos idle time elapsed (server only)
		while (IS_SERVER && (s = socketlist_get_next()) > 0) {
			if (s != mainsocket_get() && sockettime_elapsed(s)) {
				network_close(s);
				socketlist_remove(s);
				disconnectfunction_exec(s);
			}
		}
		
		// Run periodic function if time elapsed
		if (nextperiodic_elapsed()) {
			periodic_exec();
			nextperiodic_reset();
		}
		
		// Check if termflag was set in periodic function
		if (termflag_isset())
			main_shutdown("Terminated.");
		
	}

	return 0;
}

/*
 * The main_init() function initializes all the modules. Each of the
 * module's init functions simply reserve and scrub memory needed for
 * static data storage.
 */
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
	log_init();
	termflag_init();

	if (IS_SERVER) {
		server_init();
	} else {
		client_init();
	}
}

/*
 * The main_sigint() function serves as the SIGINT handler. This function
 * is called when the user presses CTRL-C. It's job is to simply call the
 * main_shutdown() function to ensure a proper shutdown.
 */
void main_sigint(int e) {
	(void)e;
	main_shutdown("Caught sigint.");
}

/*
 * The main_shutdown() function ensures all open file descriptors are closed
 * cleanly, and that memory resources are freed. It also calls the disconnect
 * function for each socket ensuring any customized cleanup tasks are also
 * completed.
 */
void main_shutdown(const char *errmsg) {
	int i;
	
	// Log a shutdown message
	log_print("Shutting down. Reason: %s", errmsg);
	
	// Close each socket. Run the disconnect function for each socket.
	while ((i = socketlist_get_next()) > 0) {
		network_close(i);
		socketlist_remove(i);
		if (i != mainsocket_get())
			disconnectfunction_exec(i);
	}
	
	// Close log file
	log_close();
	
	// Shutdown
	exit(1);
}

