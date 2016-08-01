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
#include "sockstate.h"
#include "commands.h"
#include "socklist.h"

/*
 * Define commands here
 */
int command_quit(COMMAND_ARGS) {
	
	// Suppress "unused parameter" warning for payload
	(void)payload;
	
	// Close socket
	close(socket);

	// remove socket from fd_set
	socklist_remove(socket);
	
	return 0;
}

int command_beat(COMMAND_ARGS) {
	printf("command_beat socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}

/*
 * Load commands here
 */
void load_commands(void) {
	load_command("beat", &command_beat);
	load_command("quit", &command_quit);
}

// TODO - Do we need periodic() to be user configurable? Maybe not...
//        Maybe just have it check for inactive sockets and that's it...
int periodic(void) {
	int i;
	
	// Check time for all sockets and close unresponsive ones
	while ((i = socklist_next()) > 0) {
		if (i == mainsockfd)
			continue;
		if (get_sockstate_last_time(i) < time(NULL) - (PERIODIC_SECONDS * 2)) {
			close(i);
			socklist_remove(i);
			del_sockstate_record(i);
		}
	}
	
	return 0;
}

int comp_type(void) {
	return SERVER;
}
