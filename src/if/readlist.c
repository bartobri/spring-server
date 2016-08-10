// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdbool.h>
#include <sys/select.h>
#include "main.h"
#include "if/readlist.h"

// Static vars
static fd_set read_fd_set;
static int list_position;

void readlist_init(void) {
	FD_ZERO(&read_fd_set);
	list_position = 0;
}

void readlist_set(fd_set socklist) {
	read_fd_set = socklist;
}

fd_set readlist_get(void) {
	return read_fd_set;
}

fd_set *readlist_getptr(void) {
	return &read_fd_set;
}

bool readlist_check(int socket) {
	if (FD_ISSET(socket, &read_fd_set))
		return true;
	
	return false;
}

void readlist_remove(int socket) {
	FD_CLR(socket, &read_fd_set);
}

int readlist_next(void) {

	while (++list_position < FD_SETSIZE)
		if (FD_ISSET (list_position, &read_fd_set))
			return list_position;

	readlist_reset();

	return -1;
}

void readlist_reset(void) {
	list_position = 0;
}
