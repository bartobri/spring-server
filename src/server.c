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
#include "logic/socket.h"

#include "l2/periodic.h"
#include "l2/command.h"
#include "l2/socket.h"
#include "l2/socketlist.h"

/*
 * Define functions here
 */
COMMANDS_RETURN command_quit(COMMANDS_ARGS) {
	
	// Suppress "unused parameter" warning for payload
	(void)payload;
	
	// Close socket
	close(socket);
	socket_remove(socket);

	socketlist_remove(socket);
	
	return 0;
}

COMMANDS_RETURN command_beat(COMMANDS_ARGS) {
	printf("command_beat socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}

PRDFUNCTION_RETURN periodic(PRDFUNCTION_ARGS) {
	int i;
	
	// Check time for all sockets and close unresponsive ones
	while ((i = socketlist_get_next()) > 0) {
		if (i == socket_get_main())
			continue;

		if (socket_get_timestamp(i) < time(NULL) - 10) {
			socket_remove(i);
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
