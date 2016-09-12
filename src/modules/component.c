// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "modules/command.h"
#include "modules/connectfunction.h"
#include "modules/disconnectfunction.h"
#include "modules/log.h"
#include "modules/mainsocket.h"
#include "modules/network.h"
#include "modules/periodic.h"
#include "modules/socketlist.h"
#include "modules/termflag.h"

// MODULE DESCRIPTION

/*
 * This module is meant to act as an abstraction layer that sits above
 * other modules contained in this project and provides user-friendly
 * access to lower level functionality that greatly simplifies the
 * customization process for the client and server modules.
 */

// FUNCTION DEFINITIONS

/*
 * The write_socket() function writes the command (char *command) and
 * payload (char *payload) data to the socket (int s).
 */
int write_socket(int s, char *command, char *payload) {
	int r;
	char *str;
	
	str = malloc(strlen(command) + strlen(payload) + 1);
	sprintf(str, "%s%s", command, payload);
	r = network_write(s, str);
	free(str);
	
	return r;
}

/*
 * The close_socket() function closes the network socket (int s) and
 * ensures that necessary cleanup tasks are executed.
 */

void close_socket(int s) {
	network_close(s);
	socketlist_remove(s);
	disconnectfunction_exec(s);
}

/*
 * The main_socket() function returns the integer value for the main
 * socket file descriptor. For the client, the main socket is that which
 * communicates to the server. For the server, the main socket is that
 * which listens for new client connections.
 */
int main_socket(void) {
	return mainsocket_get();
}

/*
 * The next_socket() function provides a iteration tool for traversing the
 * list of all active socket connections. Each call to next_socket()
 * returns the integer value for the next socket file descriptor in the
 * list. When the iteration reaches the end of the list, a negative
 * integer value is returned.
 */
int next_socket(void) {
	return socketlist_get_next();
}

/*
 * The reset_next_socket() function sets an internally maintained list
 * position pointer to the start of the socket list. This function
 * should be called whenever you want the next_socket() function to
 * return the first socket in the socket list, such as the start of a
 * full socket list traversal.
 */
void reset_next_socket(void) {
	socketlist_reset_next();
}

/*
 * The set_connect_function() function sets the given function pointer
 * (connectFunctionType functionPtr) as the "connect function". This
 * function is called for every new connection that is established.
 */
void set_connect_function(connectFunctionType functionPtr) {
	connectfunction_set(functionPtr);
}

/*
 * The set_disconnect_function() function sets the given function pointer
 * (disconnectFunctionType functionPtr) as the "disconnect function". This
 * function is called for every socket disconnection.
 */
void set_disconnect_function(disconnectFunctionType functionPtr) {
	disconnectfunction_set(functionPtr);
}

/*
 * The add_periodic_function() function adds the given function pointer
 * (prdFunctionType functionPtr) to a list of function pointers that are
 * executed at an interval defined by the symbolic constant
 * PERIODIC_SECONDS.
 */
void add_periodic_function(prdFunctionType functionPtr) {
	periodic_add(functionPtr);
}

/*
 * The add_command_function() function tells the client or server component
 * to execute the function pointer (comFunctionType functionPtr) when the
 * command (char *command) is received from a socket connection.
 */
void add_command_function(char *command, comFunctionType functionPtr) {
	command_add(command, functionPtr);
}

/*
 * The terminate() function tells the current client or server program
 * to terminate execution after the current function has returned
 * execution to the calling function.
 */
void terminate(void) {
	termflag_set();
}


