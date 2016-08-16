// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "l1/command.h"

// Static Variables
static char command[COMMAND_SIZE + 1];

void command_set(char *data) {
	strncpy(command, data, COMMAND_SIZE);
}

char *command_get(void) {
	return command;
}
