// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include "sockmain.h"

// Static vars
static int mainsockfd;

void sockmain_init(void) {
	mainsockfd = 0;
}

void sockmain_set(int socket) {
	mainsockfd = socket;
}

int sockmain_get(void) {
	return mainsockfd;
}
