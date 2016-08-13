// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "if/payload.h"

// Static Variables
static char payload[PAYLOAD_SIZE + 1];

void payload_init(void) {
	memset(payload, 0, sizeof(payload));
}

void payload_set(char *data) {

	// Reset the buffer with all integer zeros ('\0')
	memset(payload, 0, sizeof(payload));
	
	// Copy to buffer and also parse out command and payload
	strncpy(payload, data, PAYLOAD_SIZE);
}

char *payload_get(void) {
	return payload;
}
