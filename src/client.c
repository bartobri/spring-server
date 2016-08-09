// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "netio.h"
#include "ctable.h"
#include "socklist.h"

void load_commands(void) {
	// Populate commands
}

int periodic(void) {
	int r;

	r = netio_write(socklist_get_mainsock(), "beat");
	
	return r;
}

int comp_type(void) {
	return CLIENT;
}
