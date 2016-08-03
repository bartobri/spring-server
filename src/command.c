// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "command.h"

struct commandTbl {
	const char *command;
	COMMAND_RETURN (*functionPtr)(COMMAND_ARGS);
};

// Static variables
static struct commandTbl commands[COMMAND_LIMIT];
static bool init_completed = false;

// TODO - Conform to data module naming standards

void command_load(char *command, COMMAND_RETURN (*functionPtr)(COMMAND_ARGS)) {
	int i;

	// Initialize commands table if not done yet
	if (init_completed == false) {
		for (i = 0; i < COMMAND_LIMIT; ++i) {
			commands[i].command = NULL;
			commands[i].functionPtr = NULL;
		}
		init_completed = true;
	}
	
	// Add passed parameters to end of commands[] array
	for (i = 0; i < COMMAND_LIMIT; ++i) {
		if (commands[i].command == NULL) {
			commands[i].command = command;
			commands[i].functionPtr = functionPtr;
			break;
		}
	}
	
	// TODO - what to do if we reach COMMAND_LIMIT?
}

bool command_valid(char *command) {
	int i;
	
	for (i = 0; i < COMMAND_LIMIT; ++i) {
		
		// If we reached the end of the list, break out of the loop.
		if (commands[i].command == NULL)
			break;

		// If we have a match, return true.
		if (strcmp(commands[i].command, command) == 0)
			return true;
	}
	
	// No match found, return false.
	return false;
}

void command_execute(char *command, char *payload, int socket) {
	int i;
	
	for (i = 0; i < COMMAND_LIMIT; ++i) {
		
		// If we reached the end of the list, break out of the loop.
		if (commands[i].command == NULL)
			break;

		// If we have a match, return true.
		if (strcmp(commands[i].command, command) == 0) {
			commands[i].functionPtr(socket, payload);
			break;
		}
	}
	
	// TODO - return output (int) from command??
}
