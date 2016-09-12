// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include "modules/connectfunction.h"

/*
 * MODULE DESCRIPTION
 * 
 * The connectfunction module manages a pointer to a function referred
 * to as the "connect function". This function is executed by the main
 * module every time a new connection established.
 */

/*
 * Static Variables
 */
static connectFunctionType connectFunction;

/*
 * Set the connect function pointer to NULL.
 */
void connectfunction_init(void) {
	connectFunction = NULL;
}

/*
 * Set the connect function to the given function pointer (functionPtr)
 */
void connectfunction_set(connectFunctionType functionPtr) {
	connectFunction = functionPtr;
}

/*
 * Check if the connect function has been set. Return a true value if it
 * has. Return a false value if it has not.
 */
int connectfunction_exists(void) {
	if (connectFunction != NULL)
		return 1;
	
	return 0;
}

/*
 * Execute the connect function if the poiter has been set. Pass as a
 * parameter the connecting entity's socket assignment.
 */
void connectfunction_exec(int socket) {
	if (connectFunction != NULL)
		connectFunction(socket);
}
