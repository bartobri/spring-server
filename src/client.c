// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "if/comfunctions.h"
#include "if/prdfunctions.h"
#include "logic/socket.h"

PRDFUNCTIONS_RETURN periodic(PRDFUNCTIONS_ARGS) {
	int r;

	r = socket_write(socket_get_main(), "beat");
	
	return r;
}

void load_functions(void) {
	prdfunctions_add(&periodic);
}

int comp_type(void) {
	return CLIENT;
}
