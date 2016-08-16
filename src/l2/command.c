// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "l1/commands.h"

void command_init(void) {
	int i;
	struct commandTbl entry;
	
	// Init comfunctions table with all nulls
	entry.command = NULL;
	entry.functionPtr = NULL;
	for (i = 0; i < COMMANDS_LIMIT; ++i)
		commands_set(i, entry);
}

void command_add(char *command, comFunctionType functionPtr) {
	int i;
	struct commandTbl entry;

	for (i = 0; i < COMMANDS_LIMIT; ++i) {
		entry = commands_get(i);
		if (entry.command == NULL) {
			entry.command = command;
			entry.functionPtr = functionPtr;
			commands_set(i, entry);
			break;
		}
	}
}

int command_exists(char *command) {
	int i;
	struct commandTbl entry;

	for (i = 0; i < COMMANDS_LIMIT; ++i) {
		entry = commands_get(i);
		if (entry.command == NULL)
			return 0;

		if (strcmp(entry.command, command) == 0)
			return 1;
	}
	
	return 0;
}

int command_exec(char *command, char *payload, int socket) {
	int i, r;
	struct commandTbl entry;
	
	r = 0;

	for (i = 0; i < COMMANDS_LIMIT; ++i) {
		entry = commands_get(i);
		if (entry.command == NULL)
			break;

		if (strcmp(entry.command, command) == 0) {
			r = entry.functionPtr(socket, payload);
			break;
		}
	}
	
	return r;
}
