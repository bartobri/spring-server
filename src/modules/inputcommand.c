// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "config.h"

// Static Variables
static char command[COMMAND_SIZE + 1];

void inputcommand_init(void) {
	memset(command, 0, sizeof(COMMAND_SIZE + 1));
}

void inputcommand_set(char *data) {
	strncpy(command, data, COMMAND_SIZE);
}

char *inputcommand_get(void) {
	return command;
}

void inputcommand_parse(char *data) {
	strncpy(command, data, COMMAND_SIZE);
}
