// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "l1/input.h"

// Static Variables
static char input[INPUT_SIZE];

void input_set(char *data) {
	strncpy(input, data, INPUT_SIZE - 1);
}

char *input_get(void) {
	return input;
}
