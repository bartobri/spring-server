// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "if/input.h"

#define COMMAND_SIZE        4
#define PAYLOAD_SIZE        100

// Static Variables
static char command[COMMAND_SIZE + 1];
static char payload[PAYLOAD_SIZE + 1];;

void input_init(void) {
	memset(command, 0, sizeof(command));
	memset(payload, 0, sizeof(payload));
}

void input_set(char *data) {

	// Reset the command/payload with all integer zeros ('\0')
	memset(command, 0, sizeof(command));
	memset(payload, 0, sizeof(payload));
	
	// Copy to buffer and also parse out command and payload
	strncpy(command, data, COMMAND_SIZE);
	strncpy(payload, data + COMMAND_SIZE, PAYLOAD_SIZE);
}

char *input_get_command(void) {
	return command;
}

char *input_get_payload(void) {
	return payload;
}
