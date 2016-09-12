// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "modules/command.h"

/*
 * MODULE DESCRIPTION
 * 
 * The command module manages a lookup table containing command-to-function
 * pairings. It is responsible for initializing the table, adding new
 * command/function pairs, and executing a function for a given command
 * string.
 */

/*
 * Static Variables
 */
static struct {
	char *command;
	comFunctionType functionPtr;
} commands[COMMAND_LIMIT];

/*
 * Initializes all member values in the commands structure array to NULL.
 */
void command_init(void) {
	int i;

	for (i = 0; i < COMMAND_LIMIT; ++i) {
		commands[i].command = NULL;
		commands[i].functionPtr = NULL;
	}
}

/*
 * Add a given char and function pointer to the commands structure array
 * in the next available slot.
 */
void command_add(char *command, comFunctionType functionPtr) {
	int i;

	for (i = 0; i < COMMAND_LIMIT; ++i) {
		if (commands[i].command == NULL) {
			commands[i].command = command;
			commands[i].functionPtr = functionPtr;
			break;
		}
	}
}

/*
 * Traverse the commands structure array looking to match a given command
 * string. A true value is returned if a match is found. A false value is
 * returned if no match is found.
 */
int command_exists(char *command) {
	int i;

	for (i = 0; i < COMMAND_LIMIT; ++i) {
		if (commands[i].command == NULL)
			return 0;

		if (strcmp(commands[i].command, command) == 0)
			return 1;
	}
	
	return 0;
}

/*
 * Traverse the commands structure array looking to match a given command
 * string. If a match is found, the associated function is executed with
 * the socket and payload as parameters.
 */
void command_exec(char *command, char *payload, int socket) {
	int i;

	for (i = 0; i < COMMAND_LIMIT; ++i) {
		if (commands[i].command == NULL)
			break;

		if (strcmp(commands[i].command, command) == 0) {
			commands[i].functionPtr(socket, payload);
			break;
		}
	}
}
