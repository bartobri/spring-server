// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "modules/command.h"

// Static Variable Definitions

/*
 * commands is a static structure array that serves as a lookup table
 * containing command-to-function pairings. It is traversed to determine
 * what function to execute for a given command.
 */
static struct {
	char *command;
	comFunctionType functionPtr;
} commands[COMMAND_LIMIT];

// Function Definitions

/*
 * The command_init() function initializes all member values in the
 * commands structure array to NULL.
 */
void command_init(void) {
	int i;

	for (i = 0; i < COMMAND_LIMIT; ++i) {
		commands[i].command = NULL;
		commands[i].functionPtr = NULL;
	}
}

/*
 * The command_add() function adds a given char and function pointer to
 * the commands structure array in the next available slot determined by
 * a value of NULL.
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
 * The command_exists() function traverses the commands structure array
 * looking to match a member command string with the given command string.
 * A true value is returned if a match is found. A false value is returned
 * if no match is found.
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
 * The command_exec() function traverses the commands structure array
 * looking to match a member command string with the given command string.
 * If a match is found, the associated function is executed with the
 * given socket and payload as parameters.
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
