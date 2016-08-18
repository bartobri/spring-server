// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

#include "l2/network.h"
#include "l2/command.h"
#include "l2/periodic.h"
#include "l2/socketlist.h"
#include "l2/mainsocket.h"

PERIODICFUNCTIONS_RETURN periodic(PERIODICFUNCTIONS_ARGS) {
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
