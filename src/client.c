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
#include "sockmain.h"

void load_commands(void) {
	// Populate commands
}

int periodic(void) {
	int r;

	r = netio_write(sockmain_get(), "beat");
	
	return r;
}

int comp_type(void) {
	return CLIENT;
}
