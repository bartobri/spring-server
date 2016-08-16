// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "l1/input.h"
#include "l2/inputparser.h"

static char command[COMMAND_SIZE + 1];
static char payload[PAYLOAD_SIZE + 1];

void inputparser_init(void) {
	char nullify[INPUT_SIZE];
	memset(nullify, 0, sizeof(nullify));
	input_set(nullify);
}

void inputparser_set_input(char *data) {
	input_set(data);
}

char *inputparser_get_command(void) {
	char *input;
	
	input = input_get();
	strncpy(command, input, COMMAND_SIZE);

	return command;
}

char *inputparser_get_payload(void) {
	char *input;
	
	input = input_get();

	strncpy(payload, input + COMMAND_SIZE, PAYLOAD_SIZE);
	
	return payload;
}
