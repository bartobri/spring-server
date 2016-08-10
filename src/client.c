// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "if/netio.h"
#include "if/socklist.h"
#include "if/ftable.h"

FUNCTION_RETURN periodic(PERIODIC_ARGS) {
	int r;

	r = netio_write(socklist_get_mainsock(), "beat");
	
	return r;
}

void load_functions(void) {
	ftable_add_periodic(&periodic);
}

int comp_type(void) {
	return CLIENT;
}
