// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "if/command.h"

// Static Variables
static char command[COMMAND_SIZE + 1];

void command_init(void) {
	memset(command, 0, sizeof(command));
}

void command_set(char *data) {

	// Reset the buffer with all integer zeros ('\0')
	memset(command, 0, sizeof(command));
	
	// Copy to buffer and also parse out command and payload
	strncpy(command, data, COMMAND_SIZE);
}

char *command_get(void) {
	return command;
}
