// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "if/ftable.h"

#define FTABLE_COMMAND  1
#define FTABLE_PERIODIC 2
#define FTABLE_LIMIT    10

struct functionTbl {
	int functionType;
	const char *command;
	FUNCTION_RETURN (*cmdFunctionPtr)(COMMAND_ARGS);
	FUNCTION_RETURN (*prdFunctionPtr)(PERIODIC_ARGS);
};

// Static variables
static struct functionTbl functions[FTABLE_LIMIT];

void ftable_init(void) {
	int i;
	
	for (i = 0; i < FTABLE_LIMIT; ++i) {
		functions[i].functionType = 0;
		functions[i].command = NULL;
		functions[i].cmdFunctionPtr = NULL;
		functions[i].prdFunctionPtr = NULL;
	}
}

void ftable_add_command(char *command, FUNCTION_RETURN (*functionPtr)(COMMAND_ARGS)) {
	int i;
	
	// Add passed parameters to end of commands[] array
	for (i = 0; i < FTABLE_LIMIT; ++i) {
		if (functions[i].functionType == 0) {
			functions[i].functionType = FTABLE_COMMAND;
			functions[i].command = command;
			functions[i].cmdFunctionPtr = functionPtr;
			break;
		}
	}
	
	// TODO - what to do if we reach FTABLE_LIMIT?
}

void ftable_add_periodic(FUNCTION_RETURN (*functionPtr)(PERIODIC_ARGS)) {
	int i;
	
	// Add passed parameters to end of commands[] array
	for (i = 0; i < FTABLE_LIMIT; ++i) {
		if (functions[i].functionType == 0) {
			functions[i].functionType = FTABLE_PERIODIC;
			functions[i].prdFunctionPtr = functionPtr;
			break;
		}
	}
	
	// TODO - what to do if we reach FTABLE_LIMIT?
}

bool ftable_check_command(char *command) {
	int i;
	
	for (i = 0; i < FTABLE_LIMIT; ++i) {
		
		// If we reached the end of the list, break out of the loop.
		if (functions[i].functionType == 0)
			break;

		// If we have a match, return true.
		if (functions[i].functionType == FTABLE_COMMAND)
			if (strcmp(functions[i].command, command) == 0)
				return true;
	}
	
	// No match found, return false.
	return false;
}

int ftable_exec_command(char *command, char *payload, int socket) {
	int i, r;
	
	r = 0;
	
	for (i = 0; i < FTABLE_LIMIT; ++i) {
		
		// If we reached the end of the list, break out of the loop.
		if (functions[i].functionType == 0)
			break;

		// If we have a match, return true.
		if (functions[i].functionType == FTABLE_COMMAND) {
			if (strcmp(functions[i].command, command) == 0) {
				r = functions[i].cmdFunctionPtr(socket, payload);
				break;
			}
		}
	}
	
	return r;
}

int ftable_exec_periodic(void) {
	int i, r;
	
	r = 0;
	
	for (i = 0; i < FTABLE_LIMIT; ++i) {
		
		// If we reached the end of the list, break out of the loop.
		if (functions[i].functionType == 0)
			break;

		// If we have a match, return true.
		if (functions[i].functionType == FTABLE_PERIODIC) {
			r = functions[i].prdFunctionPtr();
			break;
		}
	}
	
	return r;
}
