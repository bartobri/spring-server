#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/in.h>
#include "main.h"
#include "sockstate.h"

struct sockstate {
	int socket;
	time_t last_time;
	struct sockstate *next;
};

// Static vars
static struct sockstate *ss_start = NULL;

/*
 * void set_last_seen(int)
 *
 * DESCR:
 * Updates a table of all sockets with the last time communication
 * was received for a given socket number.
 *
 * ARGS:
 * int socket - socket to be updated
 *
 */
void set_sockstate_last_time(int socket) {
	struct sockstate *ss_pointer;
	
	// If list is empty, create first record.
	if (ss_start == NULL) {
		ss_start = malloc(sizeof(struct sockstate));
		ss_start->socket = socket;
		ss_start->last_time = time(NULL);
		ss_start->next = NULL;
		return;
	}
	
	// Loop over list and either update existing record of create new
	ss_pointer = ss_start;
	while (true) {
		
		//Update record if exists
		if (ss_pointer->socket == socket) {
			ss_pointer->last_time = time(NULL);
			return;
		}
		
		// Append new record for socket
		if (ss_pointer->next == NULL) {
			ss_pointer->next = malloc(sizeof(struct sockstate));
			ss_pointer = ss_pointer->next;
			ss_pointer->socket = socket;
			ss_pointer->last_time = time(NULL);
			ss_pointer->next = NULL;
			return;
		}

		// Get next record
		ss_pointer = ss_pointer->next;
	}
}

int get_sockstate_last_time(int socket) {
	struct sockstate *ss_pointer = NULL;

	ss_pointer = ss_start;
	while (ss_pointer != NULL) {
		
		if (ss_pointer->socket == socket)
			return ss_pointer->last_time;
			
		ss_pointer = ss_pointer->next;
	}
	
	return 0;
}

void del_sockstate_record(int socket) {
	struct sockstate *ss_pointer = NULL;
	struct sockstate *ss_prev    = NULL;
	struct sockstate *ss_temp    = NULL;
	
	ss_pointer = ss_start;
	while (ss_pointer != NULL) {
		if (ss_pointer->socket == socket) {
			ss_temp = ss_pointer;
			if (ss_prev == NULL)
				ss_start = ss_pointer->next;
			else
				ss_prev->next = ss_pointer->next;
			free(ss_temp);
			return;
		}
		ss_prev = ss_pointer;
		ss_pointer = ss_pointer->next;
	}
}
