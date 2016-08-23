// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "config.h"

// Static Variables
static char payload[PAYLOAD_SIZE + 1];

void inputpayload_init(void) {
	memset(payload, 0, sizeof(PAYLOAD_SIZE + 1));
}

void inputpayload_set(char *data) {
	strncpy(payload, data, PAYLOAD_SIZE);
}

char *inputpayload_get(void) {
	return payload;
}

void inputpayload_parse(char *data) {
	strncpy(payload, data + COMMAND_SIZE, PAYLOAD_SIZE);
}
