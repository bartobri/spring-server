// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include <stdlib.h>
#include "l1/inputpayload.h"
#include "l2/inputcommand.h"

void inputparser_init(void) {
	char *nullify;
	
	// nullify payload
	nullify = malloc(INPUTPAYLOAD_SIZE);
	memset(nullify, 0, sizeof(INPUTPAYLOAD_SIZE));
	inputpayload_set(nullify);
	
	// Free memory
	free(nullify);
}

void inputparser_parse_input(char *data) {
	char payload[INPUTPAYLOAD_SIZE + 1];
	
	strncpy(payload, data + INPUTCOMMAND_SIZE, INPUTPAYLOAD_SIZE);

	inputpayload_set(payload);
}

char *inputparser_get_payload(void) {
	return inputpayload_get();
}
