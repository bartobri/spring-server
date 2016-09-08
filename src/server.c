// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include "modules/component.h"

/*
 * Define functions here
 */
CONNECTFUNCTION_RETURN send_greeting(CONNECTFUNCTION_ARGS) {
	(void)socket;
	
	write_socket(socket, "helo", "");
}

DISCONNECTFUNCTION_RETURN client_disconnect(DISCONNECTFUNCTION_ARGS) {
	(void)socket;
	
	// custom code that runs when a client disconnects
	printf("Socket %i disconnected\n", socket);
}
 
COMMAND_RETURN command_quit(COMMAND_ARGS) {
	(void)socket;
	(void)payload;
	
	// Close socket
	close_socket(socket);
}

COMMAND_RETURN command_beat(COMMAND_ARGS) {
	(void)socket;
	(void)payload;

	printf("Received beat command on socket: %i, payload: %s\n", socket, payload);
}

PERIODIC_RETURN periodic(PERIODIC_ARGS) {
	// Periodic code here.
}

/*
 * Load functions here
 */
void server_init(void) {
	set_connect_function(&send_greeting);
	add_command_function("beat", &command_beat);
	add_command_function("quit", &command_quit);
	add_periodic_function(&periodic);
	set_disconnect_function(&client_disconnect);
}
