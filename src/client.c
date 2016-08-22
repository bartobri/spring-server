// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include "main.h"

#include "modules/network.h"
#include "modules/command.h"
#include "modules/periodic.h"
#include "modules/socketlist.h"
#include "modules/mainsocket.h"

PERIODIC_RETURN periodic(PERIODIC_ARGS) {
	int r;

	r = network_write(mainsocket_get(), "beat");
	
	return r;
}

void load_functions(void) {
	periodic_add(&periodic);
}

int comp_type(void) {
	return CLIENT;
}
