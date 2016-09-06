// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "modules/periodic.h"
#include "modules/command.h"
#include "modules/connectfunction.h"
#include "modules/socketlist.h"
#include "modules/sockettime.h"
#include "modules/mainsocket.h"
#include "modules/network.h"

/*
 * Define functions here
 */
CONNECTFUNCTION_RETURN send_greeting(CONNECTFUNCTION_ARGS) {
	(void)socket;
	
	network_write(socket, "helo");
	
	return 0;
}
 
COMMAND_RETURN command_quit(COMMAND_ARGS) {
	
	// Suppress "unused parameter" warning for payload
	(void)payload;
	
	// Close socket
	close(socket);
	socketlist_remove(socket);
	
	return 0;
}

COMMAND_RETURN command_beat(COMMAND_ARGS) {
	printf("Received beat command on socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}

PERIODIC_RETURN periodic(PERIODIC_ARGS) {
	int i;
	
	// Check time for all sockets and close unresponsive ones
	while ((i = socketlist_get_next()) > 0) {
		if (i == mainsocket_get())
			continue;

		// Close socket if idle time elapsed
		if (sockettime_elapsed(i)) {
			close(i);
			socketlist_remove(i);
		}
	}
	
	return 0;
}

/*
 * Load functions here
 */
void server_init(void) {
	connectfunction_set(&send_greeting);
	command_add("beat", &command_beat);
	command_add("quit", &command_quit);
	periodic_add(&periodic);
}
