// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>

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
COMMAND_RETURN receive_hello(COMMAND_PARAMS) {
	(void)socket;
	(void)payload;
	
	printf("Received hello from server\n");
}

PERIODIC_RETURN send_heartbeat(PERIODIC_PARAMS) {
	static int c = 0;
	
	if (c++ < 5)
		write_socket(main_socket(), "beat", "");
}

/*
 * The client_init() function is executed when the client starts. This
 * function should be used to load all your custom functions. Further,
 * any other initialization tasks needed for your custom application can
 * be added to this function e.g. connecting to a database.
 */
void client_init(void) {
	
	// Load your custom functions here.
	//
	// e.g.
	// set_connect_function(&function_name);
	// set_disconnect_function(&function_name);
	// add_periodic_function(&function_name);
	// add_command_function("cmnd", &function_name);
	
	add_command_function("helo", &receive_hello);
	add_periodic_function(&send_heartbeat);
}
