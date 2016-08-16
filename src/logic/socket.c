// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "if/timestamps.h"
#include "if/socklist.h"

#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];

void socket_init(void) {
	timestamps_init();
	socklist_init();
}

int socket_read(int socket) {
	int r;
		
	// Reset the buffer with all integer zeros ('\0')
	memset(buffer, 0, BUFFER_SIZE);
	
	// Read from socket
	r = read(socket, buffer, BUFFER_SIZE - 1);
	
	// Parse and store buffer
	if (r >= 0) {
		timestamps_set(socket, (int)time(NULL));
	}
	
	return r;
}

int socket_write(int socket, char *data) {
	int r;

	r = write(socket, data, strlen(data));

	return r;
}

char *socket_get_buffer(void) {
	return buffer;
}

void socket_add(int socket) {
	timestamps_add(socket, (int)time(NULL));
	socklist_add(socket);
}

void socket_add_main(int socket) {
	timestamps_add(socket, (int)time(NULL));
	socklist_add_mainsock(socket);
}

void socket_remove(int socket) {
	timestamps_remove(socket);
	socklist_remove(socket);
}

fd_set socket_get_list(void) {
	return socklist_get();
}

int socket_get_main(void) {
	return socklist_get_mainsock();
}

int socket_get_timestamp(int socket) {
	return timestamps_get(socket);
}

void socket_shutdown_all(void) {
	int i;
	
	while ((i = socklist_next()) > 0) {
		close(i);
		socket_remove(i);
	}
}

void socket_shutdown(int socket) {
	close(socket);
	socket_remove(socket);
}

int socket_next(void) {
	return socklist_next();
}
