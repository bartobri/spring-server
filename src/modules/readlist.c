// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <sys/select.h>
#include <time.h>
#include "config.h"
#include "modules/readlist.h"

// Static vars
static fd_set read_fd_set;
static int list_position;

void readlist_init(void) {
	FD_ZERO(&read_fd_set);
	list_position = 0;
}

void readlist_add(int socket) {
	FD_SET(socket, &read_fd_set);
}

int readlist_wait(void) {
	int r;
	struct timeval timeout;
		
	// Set select() timeout value.
	// This needs to be inside the loop so it is reset for each loop interation.
	timeout.tv_sec  = (unsigned int)PERIODIC_SECONDS;
	timeout.tv_usec = 0;
		
	// Block until input arrives on one or more active sockets
	r = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout);
	
	return r;
}

void readlist_remove(int socket) {
	FD_CLR(socket, &read_fd_set);
}

int readlist_check(int socket) {
	if (FD_ISSET(socket, &read_fd_set))
		return 1;
	
	return 0;
}

int readlist_get_next(void) {

	while (++list_position < FD_SETSIZE)
		if (FD_ISSET (list_position, &read_fd_set))
			return list_position;

	readlist_reset_next();

	return -1;
}

void readlist_reset_next(void) {
	list_position = 0;
}
