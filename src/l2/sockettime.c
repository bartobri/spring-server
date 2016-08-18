// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <sys/select.h>
#include <time.h>
#include "l1/sockettimes.h"

void sockettime_init(void) {
	int i;
	struct timestampTbl entry;
	
	// Init timestamp table with all zeros
	entry.socket = 0;
	entry.timestamp = 0;
	for (i = 0; i < FD_SETSIZE; ++i)
		sockettimes_set(i, entry);
}

void sockettime_set(int socket) {
	int i;
	struct timestampTbl entry;
	
	for (i = 0; i < FD_SETSIZE; ++i) {
		entry = sockettimes_get(i);
		if (entry.socket == socket) {
			entry.timestamp = (int)time(NULL);
			sockettimes_set(i, entry);
			break;
		}
		if (entry.socket == 0) {
			entry.socket = socket;
			entry.timestamp = (int)time(NULL);
			sockettimes_set(i, entry);
			break;
		}
	}
}

int sockettime_get(int socket) {
	int i;
	struct timestampTbl entry;
	
	for (i = 0; i < FD_SETSIZE; ++i) {
		entry = sockettimes_get(i);

		if (entry.socket == socket)
			return entry.timestamp;

		if (entry.socket == 0)
			return 0;
	}
	
	return 0;
}
