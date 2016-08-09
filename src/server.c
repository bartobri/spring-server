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
#include "socktime.h"
#include "ftable.h"
#include "socklist.h"

/*
 * Define functions here
 */
FUNCTION_RETURN command_quit(COMMAND_ARGS) {
	
	// Suppress "unused parameter" warning for payload
	(void)payload;
	
	// Close socket
	close(socket);
	socklist_remove(socket);
	socktime_clear(socket);
	
	return 0;
}

FUNCTION_RETURN command_beat(COMMAND_ARGS) {
	printf("command_beat socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}

FUNCTION_RETURN periodic(PERIODIC_ARGS) {
	int i;
	
	// Check time for all sockets and close unresponsive ones
	while ((i = socklist_next()) > 0) {
		if (i == socklist_get_mainsock())
			continue;
		if (socktime_get(i) < time(NULL) - 10) {
			close(i);
			socklist_remove(i);
			socktime_clear(i);
		}
	}
	
	return 0;
}

/*
 * Load functions here
 */
void load_functions(void) {
	ftable_add_command("beat", &command_beat);
	ftable_add_command("quit", &command_quit);
	ftable_add_periodic(&periodic);
}

int comp_type(void) {
	return SERVER;
}
