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

/*
 * Define commands here
 */

int command_quit(int socket, char *payload) {
	
	// Suppress "unused parameter" warning for payload
	(void)payload;
	
	// Close socket
	close(socket);

	// remove socket from fd_set
	FD_CLR(socket, &active_fd_set);
	
	return 0;
}

int command_beat(int socket, char *payload) {
	printf("command_beat socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}

/*
 * Load commands here
 */
void load_commands(struct commandTable *commands) {
	// Populate commands
	commands[0].command = "quit";
	commands[0].functionPtr = &command_quit;
	commands[1].command = "beat";
	commands[1].functionPtr = &command_beat;
}


int periodic(void) {
	
	// Check time for all sockets and close unresponsive ones
	int i;
	for (i = 0; i < FD_SETSIZE; ++i) {
		if (FD_ISSET (i, &active_fd_set) && i != mainsockfd) {
			if (get_sockstate_last_time(i) < time(NULL) - (PERIODIC_SECONDS * 2)) {
				close(i);
				FD_CLR(i, &active_fd_set);
				del_sockstate_record(i);
			}
		}
	}
	
	return 0;
}

int comp_type(void) {
	return SERVER;
}
