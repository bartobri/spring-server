// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/in.h>
#include "main.h"
#include "socktime.h"

struct timeTbl {
	int socket;
	time_t last_active;
	struct timeTbl *next;
};

// Static vars
static struct timeTbl *tt_start = NULL;


void socktime_set(int socket) {
	struct timeTbl *tt_pointer;
	
	// If list is empty, create first record.
	if (tt_start == NULL) {
		tt_start = malloc(sizeof(struct timeTbl));
		tt_start->socket = socket;
		tt_start->last_active = time(NULL);
		tt_start->next = NULL;
		return;
	}
	
	// Loop over list and either update existing record of create new
	tt_pointer = tt_start;
	while (true) {
		
		//Update record if exists
		if (tt_pointer->socket == socket) {
			tt_pointer->last_active = time(NULL);
			return;
		}
		
		// Append new record for socket
		if (tt_pointer->next == NULL) {
			tt_pointer->next = malloc(sizeof(struct timeTbl));
			tt_pointer = tt_pointer->next;
			tt_pointer->socket = socket;
			tt_pointer->last_active = time(NULL);
			tt_pointer->next = NULL;
			return;
		}

		// Get next record
		tt_pointer = tt_pointer->next;
	}
}

int socktime_get(int socket) {
	struct timeTbl *tt_pointer = NULL;

	tt_pointer = tt_start;
	while (tt_pointer != NULL) {
		
		if (tt_pointer->socket == socket)
			return tt_pointer->last_active;
			
		tt_pointer = tt_pointer->next;
	}
	
	return 0;
}

void socktime_unset(int socket) {
	struct timeTbl *tt_pointer = NULL;
	struct timeTbl *tt_prev    = NULL;
	struct timeTbl *tt_temp    = NULL;
	
	tt_pointer = tt_start;
	while (tt_pointer != NULL) {
		if (tt_pointer->socket == socket) {
			tt_temp = tt_pointer;
			if (tt_prev == NULL)
				tt_start = tt_pointer->next;
			else
				tt_prev->next = tt_pointer->next;
			free(tt_temp);
			return;
		}
		tt_prev = tt_pointer;
		tt_pointer = tt_pointer->next;
	}
}
