// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

// Required header for custom functions. Do not remove.
#include "modules/component.h"

// Copy and paste the function templates below to start creating your
// custom functions.

/***********************************************************************

CONNECT_FUNCTION(function_name) {
	(void)socket;
	
	// Code here
}

DISCONNECT_FUNCTION(function_name) {
	(void)socket;
	
	// Code here
}

PERIODIC_FUNCTION(function_name) {
	// Code here
}

COMMAND_FUNCTION(function_name) {
	(void)socket;
	(void)payload;
	
	// Code here
}

***********************************************************************/

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

}
