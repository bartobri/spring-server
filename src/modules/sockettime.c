// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <time.h>
#include <sys/select.h>
#include "config.h"

/*
 * MODULE DESCRIPTION
 * 
 * The sockettime module stores and provides access to a list of sockets
 * file descriptors and the timestamp of the last time data was received.
 */

/*
 * Static Variables
 */
static struct {
	int socket;
	int timestamp;
} times[FD_SETSIZE];

/*
 * Initialize all static variable values to zero.
 */
void sockettime_init(void) {
	int i;
	
	// Init timestamp table with all zeros
	for (i = 0; i < FD_SETSIZE; ++i) {
		times[i].socket = 0;
		times[i].timestamp = 0;
	}
}

/*
 * Set the timestamp for the given socket number to the current time. If
 * the socket does not exists on the list, it is added.
 */
void sockettime_set(int socket) {
	int i;
	
	for (i = 0; i < FD_SETSIZE; ++i) {
		if (times[i].socket == socket) {
			times[i].timestamp = (int)time(NULL);
			break;
		}
		if (times[i].socket == 0) {
			times[i].socket = socket;
			times[i].timestamp = (int)time(NULL);
			break;
		}
	}
}

/*
 * Return the timestamp for the given socket number. If the socket is
 * not found, zero is returned.
 */
int sockettime_get(int socket) {
	int i;
	
	for (i = 0; i < FD_SETSIZE; ++i) {
		if (times[i].socket == socket)
			return times[i].timestamp;

		if (times[i].socket == 0)
			return 0;
	}
	
	return 0;
}

/*
 * Return a true value if the number of seconds configured for IDLE_SECONDS
 * has passed since the last time the given socket timestamp was updated.
 * Return a false value if not.
 */
int sockettime_elapsed(int socket) {
	int i;
	
	for (i = 0; i < FD_SETSIZE; ++i) {
		if (times[i].socket == socket) {
			if (times[i].timestamp < time(NULL) - IDLE_SECONDS)
				return 1;
			else
				return 0;
		}
	}
	
	return 0;
}
