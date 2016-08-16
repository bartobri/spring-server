// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "l1/inputcommand.h"

// Static Variables
static char command[INPUTCOMMAND_SIZE + 1];

void inputcommand_set(char *data) {
	strncpy(command, data, INPUTCOMMAND_SIZE);
}

char *inputcommand_get(void) {
	return command;
}
