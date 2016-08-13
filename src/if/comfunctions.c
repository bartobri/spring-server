// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include "main.h"
#include "if/comfunctions.h"

// Static variables
static struct comFunctionsTbl functions[COMFUNCTIONS_LIMIT];

void comfunctions_init(void) {
	int i;
	
	for (i = 0; i < COMFUNCTIONS_LIMIT; ++i) {
		functions[i].command = NULL;
		functions[i].functionPtr = NULL;
	}
}

void comfunctions_add(char *command, COMFUNCTIONS_RETURN (*functionPtr)(COMFUNCTIONS_ARGS)) {
	int i;
	
	// Add passed parameters to end of commands[] array
	for (i = 0; i < COMFUNCTIONS_LIMIT; ++i) {
		if (functions[i].command == NULL) {
			functions[i].command = command;
			functions[i].functionPtr = functionPtr;
			break;
		}
	}
}

struct comFunctionsTbl *comfunctions_get (void) {
	return functions;
}
