// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include "l1/commandfunctions.h"

// Static variables
static struct commandTbl commands[COMMANDFUNCTIONS_LIMIT];

struct commandTbl commandfunctions_get(int i) {
	return commands[i];
}

void commandfunctions_set(int i, struct commandTbl entry) {
	commands[i] = entry;
}
