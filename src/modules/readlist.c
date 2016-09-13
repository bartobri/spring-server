// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <sys/select.h>
#include <time.h>
#include "modules/readlist.h"

/*
 * MODULE DESCRIPTION
 * 
 * The readlist module maintains a list of socket file descriptors for
 * the specific purpose of determining which have data to be read. 
 * Initially the main loop adds all active sockets to this list. Then
 * after readlist_wait() is called, the list is reduced to only those
 * with data available. Functions are provided to access access/modify
 * this list pre and post readlist_wait().
 */
 
/*
 * Static Variables
 */
static fd_set read_fd_set;
static int list_position;

/*
 * Set all static variable values to zero.
 */
void readlist_init(void) {
	FD_ZERO(&read_fd_set);
	list_position = 0;
}

/*
 * Add the given socket to the readlist.
 */
void readlist_add(int socket) {
	FD_SET(socket, &read_fd_set);
}

/*
 * Wait for data to arrive at one of the sockets in the readlist. A
 * timeout value is used that is equal to the number of seconds configured
 * for PERIODIC_SECONDS. On success, the total number of file descriptors
 * that have data available for reading is returned, and the realist is
 * reduced to contain only those sockets. On timeout, zero is returned.
 * On failure, a negative integer is returned.
 */
int readlist_wait(unsigned int t) {
	int r;
	struct timeval timeout;
		
	// Set select() timeout value.
	// This needs to be inside the loop so it is reset for each loop interation.
	timeout.tv_sec  = t;
	timeout.tv_usec = 0;
		
	// Block until input arrives on one or more active sockets
	r = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout);
	
	return r;
}

/*
 * Remove the given socket from the readlist.
 */
void readlist_remove(int socket) {
	FD_CLR(socket, &read_fd_set);
}

/*
 * Check if the given socket exists in the readlist.
 */
int readlist_check(int socket) {
	if (FD_ISSET(socket, &read_fd_set))
		return 1;
	
	return 0;
}

/*
 * Return the next socket value from the readlist. Return a negative
 * value when the end of the list is reached.
 */
int readlist_get_next(void) {

	while (++list_position < FD_SETSIZE)
		if (FD_ISSET (list_position, &read_fd_set))
			return list_position;

	readlist_reset_next();

	return -1;
}

/*
 * Reset the list position used for traversing the readlist.
 */
void readlist_reset_next(void) {
	list_position = 0;
}
