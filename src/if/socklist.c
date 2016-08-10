// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <sys/select.h>
#include "main.h"
#include "if/socklist.h"

// Static variables
static fd_set active_fd_set;
static int list_position;
static int mainsock;

void socklist_init(void) {
	FD_ZERO(&active_fd_set);
	list_position = 0;
	mainsock = 0;
}

void socklist_add(int socket) {
	FD_SET(socket, &active_fd_set);
}

void socklist_remove(int socket) {
	FD_CLR(socket, &active_fd_set);
}

fd_set socklist_get(void) {
	return active_fd_set;
}

int socklist_next(void) {

	while (++list_position < FD_SETSIZE)
		if (FD_ISSET (list_position, &active_fd_set))
			return list_position;

	socklist_reset();

	return -1;
}

void socklist_reset(void) {
	list_position = 0;
}

void socklist_add_mainsock(int socket) {
	mainsock = socket;
	socklist_add(socket);
}
int socklist_get_mainsock(void) {
	return mainsock;
}
