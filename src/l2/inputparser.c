// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include <stdlib.h>
#include "l1/command.h"
#include "l1/payload.h"

void inputparser_init(void) {
	char *nullify;
	
	// nullify command
	nullify = malloc(COMMAND_SIZE);
	memset(nullify, 0, sizeof(COMMAND_SIZE));
	command_set(nullify);
	
	// nullify payload
	nullify = realloc(nullify, PAYLOAD_SIZE);
	memset(nullify, 0, sizeof(PAYLOAD_SIZE));
	payload_set(nullify);
	
	// Free memory
	free(nullify);
}

void inputparser_parse_input(char *data) {
	char command[COMMAND_SIZE + 1];
	char payload[PAYLOAD_SIZE + 1];
	
	strncpy(command, data, COMMAND_SIZE);
	strncpy(payload, data + COMMAND_SIZE, PAYLOAD_SIZE);
	
	command_set(command);
	payload_set(payload);
}

char *inputparser_get_command(void) {
	return command_get();
}

char *inputparser_get_payload(void) {
	return payload_get();
}
