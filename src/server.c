// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

// Required header for custom functions. Do not remove.
#include "modules/component.h"

// Copy and paste the function templates below to start creating your
// custom functions.

/***********************************************************************
// Connect Function Template:

CONNECTFUNCTION_RETURN function_name(CONNECTFUNCTION_PARAMS) {
	(void)socket;
	
	// Code here
}

// Disconnect Function Template:

DISCONNECTFUNCTION_RETURN function_name(DISCONNECTFUNCTION_PARAMS) {
	(void)socket;
	
	// Code here
}

// Periodic Function Template:

PERIODIC_RETURN function_name(PERIODIC_PARAMS) {
	// Code here
}

// Command Function Template:

COMMAND_RETURN function_name(COMMAND_PARAMS) {
	(void)socket;
	(void)payload;
	
	// Code here
}
***********************************************************************/

// Custom Functions Definitions Here
CONNECTFUNCTION_RETURN say_hello(CONNECTFUNCTION_PARAMS) {
	(void)socket;
	
	write_socket(socket, "helo", "");
}

DISCONNECTFUNCTION_RETURN print_message(DISCONNECTFUNCTION_PARAMS) {
	(void)socket;
	
	print_log("Socket %i disconnected by server. Idle time elapsed.", socket);
}

/*
 * The server_init() function is executed when the server starts. This
 * function should be used to load all your custom functions. Further,
 * any other initialization tasks needed for your custom application can
 * be added to this function e.g. connecting to a database.
 */
void server_init(void) {
	
	// Load your custom functions here.
	//
	// e.g.
	// set_connect_function(&function_name);
	// set_disconnect_function(&function_name);
	// add_periodic_function(&function_name);
	// add_command_function("cmnd", &function_name);
	
	set_connect_function(&say_hello);
	set_disconnect_function(&print_message);
}
