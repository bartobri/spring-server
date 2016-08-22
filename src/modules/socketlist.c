// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <sys/select.h>
#include "modules/socketlist.h"

// Static variables
static fd_set active_fd_set;
static int list_position;

void socketlist_init(void) {
	FD_ZERO(&active_fd_set);
	list_position = 0;
}

void socketlist_add(int socket) {
	FD_SET(socket, &active_fd_set);
}

void socketlist_remove(int socket) {
	FD_CLR(socket, &active_fd_set);
}

int socketlist_get_next(void) {

	while (++list_position < FD_SETSIZE)
		if (FD_ISSET (list_position, &active_fd_set))
			return list_position;

	socketlist_reset_next();

	return -1;
}

void socketlist_reset_next(void) {
	list_position = 0;
}
