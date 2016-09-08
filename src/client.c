// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include "modules/component.h"

COMMAND_RETURN command_helo(COMMAND_ARGS) {
	(void)socket;
	(void)payload;
	
	// Use helo command to display an initial message to the client
	// or to initiate a process.
	
	return 0;
}

PERIODIC_RETURN send_heartbeat(PERIODIC_ARGS) {
	int r;

	r = write_socket(main_socket(), "beat", "");
	
	return r;
}

void client_init(void) {
	add_command_function("helo", &command_helo);
	add_periodic_function(&send_heartbeat);
}
