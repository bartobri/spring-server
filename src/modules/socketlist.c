// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <sys/select.h>
#include "modules/socketlist.h"

/*
 * MODULE DESCRIPTION
 * 
 * The socketlist module maintains and provides access to a list of all
 * active socket file descriptors.
 */

/*
 * Static Variables
 */
static fd_set active_fd_set;
static int list_position;

/*
 * Set all static variable values to zero.
 */
void socketlist_init(void) {
	FD_ZERO(&active_fd_set);
	list_position = 0;
}

/*
 * Add the given socket to the socketlist.
 */
void socketlist_add(int socket) {
	FD_SET(socket, &active_fd_set);
}

/*
 * Remove the given socket from the socketlist.
 */
void socketlist_remove(int socket) {
	FD_CLR(socket, &active_fd_set);
}

/*
 * Return the next socket value from the socketlist. Return a negative
 * value when the end of the list is reached.
 */
int socketlist_get_next(void) {

	while (++list_position < FD_SETSIZE)
		if (FD_ISSET (list_position, &active_fd_set))
			return list_position;

	socketlist_reset_next();

	return -1;
}

/*
 * Reset the list position used for traversing the socketlist.
 */
void socketlist_reset_next(void) {
	list_position = 0;
}
