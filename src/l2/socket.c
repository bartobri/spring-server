// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <time.h>
#include "l1/mainsocket.h"
#include "l1/sockettimes.h"

// TODO add socket_init() once I merge this with logic/socket.c

void socket_set_main(int s) {
	mainsocket_set(s);
}

int socket_get_main(void) {
	return mainsocket_get();
}
