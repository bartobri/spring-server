// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "modules/command.h"

// Static vars
static struct commandTbl commands[COMMAND_LIMIT];

void command_init(void) {
	int i;
	
	// Init commandfunctions table with all nulls
	for (i = 0; i < COMMAND_LIMIT; ++i) {
		commands[i].command = NULL;
		commands[i].functionPtr = NULL;
	}
}

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
