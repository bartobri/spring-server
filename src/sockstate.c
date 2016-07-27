#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/in.h>
#include "main.h"
#include "sockstate.h"

struct lastseen {
	int socket;
	time_t last_time;
	struct lastseen *next;
};

// Globals
struct lastseen *ls_start = NULL;

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
 // TODO - remove printf statements in this function
void set_last_seen(int socket) {
	struct lastseen *ls_pointer;
	
	// If list is empty, create first record.
	if (ls_start == NULL) {
		ls_start = malloc(sizeof(struct lastseen));
		ls_start->socket = socket;
		ls_start->last_time = time(NULL);
		ls_start->next = NULL;
		printf("updated lastseen for socket %i (first entry)\n", ls_start->socket);
		return;
	}
	
	// Loop over list and either update existing record of create new
	ls_pointer = ls_start;
	while (true) {
		
		//Update record if exists
		if (ls_pointer->socket == socket) {
			ls_pointer->last_time = time(NULL);
			printf("updated lastseen for socket %i (existing entry)\n", ls_pointer->socket);
			break;
		}
		
		// Append new record for socket
		if (ls_pointer->next == NULL) {
			ls_pointer->next = malloc(sizeof(struct lastseen));
			ls_pointer = ls_pointer->next;
			ls_pointer->socket = socket;
			ls_pointer->last_time = time(NULL);
			ls_pointer->next = NULL;
			printf("updated lastseen for socket %i (new entry)\n", ls_pointer->socket);
			break;
		}

		// Get next record
		printf("Slipping record for socket %i\n", ls_pointer->socket);
		ls_pointer = ls_pointer->next;
	}
}

int get_last_seen(int socket) {
	struct lastseen *ls_pointer = NULL;

	printf("get lastseen %i\n", socket);
	ls_pointer = ls_start;
	while (ls_pointer != NULL) {
		
		if (ls_pointer->socket == socket)
			return ls_pointer->last_time;
			
		ls_pointer = ls_pointer->next;
	}
	
	return 0;
}

void del_last_seen(int socket) {
	struct lastseen *ls_pointer = NULL;
	struct lastseen *ls_prev = NULL;
	struct lastseen *ls_temp = NULL;
	
	printf("del lastseen %i\n", socket);
	
	ls_pointer = ls_start;
	while (ls_pointer != NULL) {
		if (ls_pointer->socket == socket) {
			ls_temp = ls_pointer;
			if (ls_prev == NULL)
				ls_start = ls_pointer->next;
			else
				ls_prev->next = ls_pointer->next;
			free(ls_temp);
			break;
		}
		ls_prev = ls_pointer;
		ls_pointer = ls_pointer->next;
	}
}
