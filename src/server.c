// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "main.h"

#include "modules/periodic.h"
#include "modules/command.h"
#include "modules/socketlist.h"
#include "modules/sockettime.h"
#include "modules/mainsocket.h"

/*
 * Define functions here
 */
COMMAND_RETURN command_quit(COMMAND_ARGS) {
	
	// Suppress "unused parameter" warning for payload
	(void)payload;
	
	// Close socket
	close(socket);
	socketlist_remove(socket);
	
	return 0;
}

COMMAND_RETURN command_beat(COMMAND_ARGS) {
	printf("command_beat socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}

PERIODIC_RETURN periodic(PERIODIC_ARGS) {
	int i;
	
	// Check time for all sockets and close unresponsive ones
	while ((i = socketlist_get_next()) > 0) {
		if (i == mainsocket_get())
			continue;

		// TODO - replace "10" with a defined name
		if (sockettime_get(i) < time(NULL) - 10) {
			socketlist_remove(i);
			close(i);
		}
	}
	
	return 0;
}

/*
 * Load functions here
 */
void load_functions(void) {
	command_add("beat", &command_beat);
	command_add("quit", &command_quit);
	periodic_add(&periodic);
}

int comp_type(void) {
	return SERVER;
}
