// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <time.h>
#include <sys/select.h>

static struct {
	int socket;
	int timestamp;
} times[FD_SETSIZE];

void sockettime_init(void) {
	int i;
	
	// Init timestamp table with all zeros
	for (i = 0; i < FD_SETSIZE; ++i) {
		times[i].socket = 0;
		times[i].timestamp = 0;
	}
}

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
