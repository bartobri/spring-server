// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "buffer.h"

// Static Variables
static char buffer[BUFFER_SIZE];
static char command[COMMAND_SIZE + 1];
static char payload[BUFFER_SIZE - COMMAND_SIZE + 1];

char *buffer_get(void) {
	return buffer;
}

void buffer_set(char *data) {

	// Reset the buffer/command/payload with all integer zeros ('\0')
	memset(buffer, 0, sizeof(buffer));
	memset(command, 0, sizeof(command));
	memset(payload, 0, sizeof(payload));
	
	// Copy to buffer and also parse out command and payload
	strncpy(buffer, data, BUFFER_SIZE - 1);
	strncpy(command, buffer, COMMAND_SIZE);
	strncpy(payload, buffer + COMMAND_SIZE, BUFFER_SIZE - COMMAND_SIZE);
}

char *buffer_get_command(void) {
	return command;
}

char *buffer_get_payload(void) {
	return payload;
}
