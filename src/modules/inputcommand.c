// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>
#include "config.h"

/*
 * MODULE DESCRIPTION
 * 
 * The inputcommand module manages a character array that contains the
 * command string from the last time network_read() was executed.
 */

/*
 * Static Variables
 */
static char command[MAX_COMMAND_SIZE + 1];

/*
 * Set the command character array to all null characters
 */
void inputcommand_init(void) {
	memset(command, 0, sizeof(command));
}

/*
 * Return the command character array.
 */
char *inputcommand_get(void) {
	return command;
}

/*
 * Parse out the first number of characters equal to the command length
 * from the given data string and store them in the command character
 * array.
 */
void inputcommand_parse(char *data) {
	int len = 0;
	
	memset(command, 0, sizeof(command));
	
	while (*data >= '0' && *data <= '9')
		len = (len * 10) + *data++ - '0';
	
	++data;
	
	if (len > MAX_COMMAND_SIZE)
		len = MAX_COMMAND_SIZE;

	strncpy(command, data, len);
}
