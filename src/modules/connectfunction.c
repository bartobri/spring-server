// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include "modules/connectfunction.h"

// MODULE DESCRIPTION

/*
 * The connectfunction module stores a pointer to a function that is
 * executed for every new connection established by the server or client.
 * For the client, there is only one connection ever established, which
 * is the connection to the server. For the server, there can
 * potentially be thousands of connections over any given period of time.
 * 
 * The connect function is defined and set inside the server and client
 * module files.
 * 
 * The purpose of the function is to initiate a process with the
 * connecting entity.
 * 
 * In most cases, only the client or the server will have a connect
 * function set. Rarely will they ever both need to have this set. It is
 * also not necessary for either of them to have this set, as it may be
 * more sensible for a periodic function to initiate a process after a
 * particular condition is met.
 */

// STATIC VARIABLES

/*
 * Function pointer to the connect function.
 */
static connectFunctionType connectFunction;

// FUNCTION DEFINITIONS

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
