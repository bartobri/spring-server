// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include <stdlib.h>
#include "l1/inputcommand.h"
#include "l1/inputpayload.h"

void inputparser_init(void) {
	char *nullify;
	
	// nullify command
	nullify = malloc(INPUTCOMMAND_SIZE);
	memset(nullify, 0, sizeof(INPUTCOMMAND_SIZE));
	inputcommand_set(nullify);
	
	// nullify payload
	nullify = realloc(nullify, INPUTPAYLOAD_SIZE);
	memset(nullify, 0, sizeof(INPUTPAYLOAD_SIZE));
	inputpayload_set(nullify);
	
	// Free memory
	free(nullify);
}

void inputparser_parse_input(char *data) {
	char command[INPUTCOMMAND_SIZE + 1];
	char payload[INPUTPAYLOAD_SIZE + 1];
	
	strncpy(command, data, INPUTCOMMAND_SIZE);
	strncpy(payload, data + INPUTCOMMAND_SIZE, INPUTPAYLOAD_SIZE);
	
	inputcommand_set(command);
	inputpayload_set(payload);
}

char *inputparser_get_command(void) {
	return inputcommand_get();
}

char *inputparser_get_payload(void) {
	return inputpayload_get();
}
