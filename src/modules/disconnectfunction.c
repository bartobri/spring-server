// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include "modules/disconnectfunction.h"

// MODULE DESCRIPTION

/*
 * The disconnectfunction module manages a pointer to a function referred
 * to as the "disconnect function". This function is executed by the main
 * module every time it detected a terminated connection, or terminates
 * one itself.
 */

// STATIC VARIABLES

/*
 * Function pointer to the disconnect function.
 */
disconnectFunctionType disconnectFunction;

/*
 * Set the disconnect function pointer to NULL.
 */
void disconnectfunction_init(void) {
	disconnectFunction = NULL;
}

/*
 * Set the disconnect function to the given function pointer (functionPtr)
 */
void disconnectfunction_set(disconnectFunctionType functionPtr) {
	disconnectFunction = functionPtr;
}

/*
 * Check if the disconnect function has been set. Return a true value if
 * it has. Return a false value if it has not.
 */
int disconnectfunction_exists(void) {
	if (disconnectFunction != NULL)
		return 1;
	
	return 0;
}

/*
 * Execute the connect function if the poiter has been set. Pass as a
 * parameter the connecting entity's socket assignment.
 */
void disconnectfunction_exec(int socket) {
	if (disconnectFunction != NULL)
		disconnectFunction(socket);
}
