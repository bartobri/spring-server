// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];

int socket_read(int socket) {
	int r;
		
	// Reset the buffer with all integer zeros ('\0')
	memset(buffer, 0, BUFFER_SIZE);
	
	// Read from socket
	r = read(socket, buffer, BUFFER_SIZE - 1);
	
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
