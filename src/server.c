// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "main.h"

#include "l2/periodic.h"
#include "l2/command.h"
#include "l2/socketlist.h"
#include "l2/sockettime.h"
#include "l2/mainsocket.h"

/*
 * Define functions here
 */
COMMANDFUNCTIONS_RETURN command_quit(COMMANDFUNCTIONS_ARGS) {
	
	// Suppress "unused parameter" warning for payload
	(void)payload;
	
	// Close socket
	close(socket);
	socketlist_remove(socket);
	
	return 0;
}

COMMANDFUNCTIONS_RETURN command_beat(COMMANDFUNCTIONS_ARGS) {
	printf("command_beat socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}

PERIODICFUNCTIONS_RETURN periodic(PERIODICFUNCTIONS_ARGS) {
	int i;
	
	// Check time for all sockets and close unresponsive ones
	while ((i = socketlist_get_next()) > 0) {
		if (i == mainsocket_get())
			continue;

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
