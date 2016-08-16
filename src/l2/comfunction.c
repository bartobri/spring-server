// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "l1/comfunctions.h"

void comfunction_init(void) {
	int i;
	struct comFunctionsTbl entry;
	
	// Init comfunctions table with all nulls
	entry.command = NULL;
	entry.functionPtr = NULL;
	for (i = 0; i < COMFUNCTIONS_LIMIT; ++i)
		comfunctions_set(i, entry);
}

void comfunction_add(char *command, comFunctionType functionPtr) {
	int i;
	struct comFunctionsTbl entry;

	for (i = 0; i < COMFUNCTIONS_LIMIT; ++i) {
		entry = comfunctions_get(i);
		if (entry.command == NULL) {
			entry.command = command;
			entry.functionPtr = functionPtr;
			comfunctions_set(i, entry);
			break;
		}
	}
}

int comfunction_exists(char *command) {
	int i;
	struct comFunctionsTbl entry;

	for (i = 0; i < COMFUNCTIONS_LIMIT; ++i) {
		entry = comfunctions_get(i);
		if (entry.command == NULL)
			return 0;

		if (strcmp(entry.command, command) == 0)
			return 1;
	}
	
	return 0;
}

int comfunction_exec(char *command, char *payload, int socket) {
	int i, r;
	struct comFunctionsTbl entry;
	
	r = 0;

	for (i = 0; i < COMFUNCTIONS_LIMIT; ++i) {
		entry = comfunctions_get(i);
		if (entry.command == NULL)
			break;

		if (strcmp(entry.command, command) == 0) {
			r = entry.functionPtr(socket, payload);
			break;
		}
	}
	
	return r;
}
