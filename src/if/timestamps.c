// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "if/timestamps.h"

struct timestampTbl {
	int socket;
	int timestamp;
	struct timestampTbl *next;
};

// Static vars
static struct timestampTbl *tt_start;

void timestamps_init(void) {
	tt_start = NULL;
}

void timestamps_add(int socket, int timestamp) {
	struct timestampTbl *tt_pointer;
	
	if (tt_start == NULL) {
		tt_start = malloc(sizeof(struct timestampTbl));
		tt_start->socket = socket;
		tt_start->timestamp = timestamp;
		tt_start->next = NULL;
		return;
	}

	tt_pointer = tt_start;
	while (tt_pointer->next != NULL)
		tt_pointer = tt_pointer->next;

	tt_pointer->next = malloc(sizeof(struct timestampTbl));
	tt_pointer->next->socket = socket;
	tt_pointer->next->timestamp = timestamp;
	tt_pointer->next->next = NULL;
	
	/*
	tt_pointer = tt_start;
	while (tt_pointer != NULL) {
		printf("timestamp for socket %i is %i\n", tt_pointer->socket, tt_pointer->timestamp);
		tt_pointer = tt_pointer->next;
	}
	*/
}

void timestamps_set(int socket, int timestamp) {
	struct timestampTbl *tt_pointer;
	
	// Loop over list to find and update record
	tt_pointer = tt_start;
	while (tt_pointer != NULL) {
		
		//Update record
		if (tt_pointer->socket == socket) {
			tt_pointer->timestamp = timestamp;
			return;
		}

		// Get next record
		tt_pointer = tt_pointer->next;
	}
}

int timestamps_get(int socket) {
	struct timestampTbl *tt_pointer;

	tt_pointer = tt_start;
	while (tt_pointer != NULL) {
		
		if (tt_pointer->socket == socket)
			break;
			
		tt_pointer = tt_pointer->next;
	}
	
	if (tt_pointer != NULL)
		return (int)tt_pointer->timestamp;
	else
		return -1;
}

void timestamps_remove(int socket) {
	struct timestampTbl *tt_pointer = NULL;
	struct timestampTbl *tt_prev    = NULL;
	struct timestampTbl *tt_temp    = NULL;
	
	tt_pointer = tt_start;
	while (tt_pointer != NULL) {

		if (tt_pointer->socket == socket) {
			tt_temp = tt_pointer;

			if (tt_prev == NULL)
				tt_start = tt_pointer->next;
			else
				tt_prev->next = tt_pointer->next;

			free(tt_temp);

			break;
		}

		tt_prev = tt_pointer;
		tt_pointer = tt_pointer->next;
	}
}
