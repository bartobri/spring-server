// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include "modules/network.h"
#include "modules/command.h"
#include "modules/periodic.h"
#include "modules/socketlist.h"
#include "modules/mainsocket.h"

COMMAND_RETURN command_helo(COMMAND_ARGS) {
	(void)socket;
	(void)payload;
	
	// Use helo command to display an initial message to the client
	// or to initiate a process.
	
	return 0;
}

PERIODIC_RETURN send_heartbeat(PERIODIC_ARGS) {
	int r;

	r = network_write(mainsocket_get(), "beat");
	
	return r;
}

void load_functions(void) {
	command_add("helo", &command_helo);
	periodic_add(&send_heartbeat);
}
