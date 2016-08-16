// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "l1/payload.h"

// Static Variables
static char payload[PAYLOAD_SIZE + 1];

void payload_set(char *data) {
	strncpy(payload, data, PAYLOAD_SIZE);
}

char *payload_get(void) {
	return payload;
}
