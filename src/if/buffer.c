// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "if/buffer.h"

// Static Variables
static char buffer[BUFFER_SIZE];

void buffer_init(void) {
	memset(buffer, 0, sizeof(buffer));
}

void buffer_set(char *data) {

	// Reset the buffer with all integer zeros ('\0')
	memset(buffer, 0, sizeof(buffer));
	
	// Copy to buffer and also parse out command and payload
	strncpy(buffer, data, BUFFER_SIZE - 1);
}

char *buffer_get(void) {
	return buffer;
}
