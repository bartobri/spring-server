// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "modules/network.h"
#include "modules/periodic.h"
#include "modules/command.h"
#include "modules/socketlist.h"
#include "modules/sockettime.h"
#include "modules/mainsocket.h"
#include "config.h"

#include "example/blackjack.h"

// serialized field width (must be single digit)
#define SFW 5

int payload_next_int(char **, int);


/*
 * Define functions here
 */
 
COMMAND_RETURN command_info(COMMAND_ARGS) {
	(void)socket;
	(void)payload;

	network_write(socket, "info" "Info about table rules here.");
	
	return 0;
}

COMMAND_RETURN command_join(COMMAND_ARGS) {
	int i, s;
	int tid, sid, taken;
	char *serialized_data;

	(void)socket;
	(void)payload;
	
	serialized_data = malloc(COMMAND_SIZE + 1 + SFW + SFW + ((SFW + (SEAT_MAX * SFW) + (SEAT_MAX * SFW)) * TABLE_MAX) + 1);
	
	// Write command
	sprintf(serialized_data, "aval");
	
	// Write serialized field width
	sprintf(serialized_data + strlen(serialized_data), "%i", SFW);
	
	// Write number of tables
	sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, TABLE_MAX);
	
	// Write number of seats per table
	sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, SEAT_MAX);
	
	for (i = 0; i < TABLE_MAX; ++i) {
		
		tid = blackjack_get_table_id(i);
		sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, tid);
		
		for (s = 0; s < SEAT_MAX; ++s) {
			
			// Serialize seat id
			sid = blackjack_get_seat_id(i, s);
			sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, sid);
			
			// serialize seat status (0 = open, 1 = occupied)
			taken = blackjack_get_seat_socket(i, s);
			if (taken)
				sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, 1);
			else
				sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, 0);
		}
	}

	network_write(socket, serialized_data);
	
	free(serialized_data);
	
	return 0;
}

COMMAND_RETURN command_sitt(COMMAND_ARGS) {
	int t, s;
	int tc, sc;
	
	(void)socket;
	(void)payload;
	
	// Get client choices
	tc = payload_next_int(&payload, SFW);
	sc = payload_next_int(&payload, SFW);
	
	// Make sure table and seat choice are valid. Set to 0 if not.
	for (t = 0; t < TABLE_MAX; ++t)
		if (tc == blackjack_get_table_id(t))
			break;
	if (t == TABLE_MAX)
		tc = 0;
	else {
		for (s = 0; s < SEAT_MAX; ++s)
			if (sc == blackjack_get_seat_id(t, s) && !blackjack_get_seat_socket(t, s))
				break;
		if (s == SEAT_MAX)
			sc = 0;
	}
	
	// If error, send error code/message to client.
	if (tc == 0) {
		network_write(socket, "sitt" "0" "Invalid Table Choice");
		return 0;
	}
	
	if (sc == 0) {
		network_write(socket, "sitt" "0" "Invalid Seat Choice");
		return 0;
	}
	
	// Send success code
	network_write(socket, "sitt" "1");
	
	// Set seat socket to occupy the seat
	blackjack_set_seat_socket(t, s, socket);
	
	return 0;
}

COMMAND_RETURN command_quit(COMMAND_ARGS) {
	
	// Suppress "unused parameter" warning for payload
	(void)payload;
	
	// Close socket
	close(socket);
	socketlist_remove(socket);
	
	return 0;
}

COMMAND_RETURN command_beat(COMMAND_ARGS) {
	printf("Received beat command on socket: %i, payload: %s\n", socket, payload);
	
	return 0;
}

PERIODIC_RETURN periodic_close(PERIODIC_ARGS) {
	int i;
	
	// Check time for all sockets and close unresponsive ones
	while ((i = socketlist_get_next()) > 0) {
		if (i == mainsocket_get())
			continue;

		// Close socket if idle time elapsed
		if (sockettime_elapsed(i)) {
			// TODO - remove player from blackjack table if seated
			close(i);
			socketlist_remove(i);
		}
	}
	
	return 0;
}

PERIODIC_RETURN periodic_table_update(PERIODIC_ARGS) {
	int t, s, h, c, socket;
	char *serialized_data;
	int seat_dat_pos;
	
	// TODO - Periodic for now. Change this to a regular function
	// that is called when a table status is changed from player input.
	// Only update that specific table.
	
	serialized_data = malloc(COMMAND_SIZE + 1 + SFW + SFW + SFW + (HAND_CARD_MAX * SFW) + (SEAT_MAX * SFW * 2) + (HAND_MAX * SEAT_MAX * SFW) + (HAND_CARD_MAX * HAND_MAX * SEAT_MAX * SFW) + 1);
	
	// Write command
	sprintf(serialized_data, "tbst");
	
	// Write serialized field width
	sprintf(serialized_data + strlen(serialized_data), "%i", SFW);
	
	// Write number of seats per table
	sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, SEAT_MAX);
	
	// Write number of hands per seat
	sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, HAND_MAX);
	
	// Write number of cards per hand
	sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, HAND_CARD_MAX);
	
	seat_dat_pos = strlen(serialized_data);
	
	for (t = 0; t < TABLE_MAX; ++t) {
		
		// reset string length by setting string terminator here
		serialized_data[seat_dat_pos] = '\0';
		
		// Write dealer hand
		// TODO - Don't show dealer's second card until it is the dealer's turn.
		for (c = 0; c < HAND_CARD_MAX; ++c) {
			int cid = blackjack_get_dealer_card_id(t, c);
			sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, cid);
		}
		
		// TODO - seat ID of current turn
		
		for (s = 0; s < SEAT_MAX; ++s) {
			
			int sid = blackjack_get_seat_id(t, s);
			sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, sid);

			// Seat Occupied Status
			if (blackjack_get_seat_socket(t, s))
				sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, 1);
			else
				sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, 0);
				
			// TODO - If occupied, skip all other bullshit and save space.
			
			for (h = 0; h < HAND_MAX; ++h) {
				
				sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, h + 1);
				
				for (c = 0; c < HAND_CARD_MAX; ++c) {
					int cid = blackjack_get_seat_hand_card_id(t, s, h, c);
					sprintf(serialized_data + strlen(serialized_data), "%.*i", SFW, cid);
				}
			}
		}
		
		//printf("%s\n\n", serialized_data);
		
		// Loop over seats again
		for (s = 0; s < SEAT_MAX; ++s) {
			if ((socket = blackjack_get_seat_socket(t, s)) > 0) {
				network_write(socket, serialized_data);
			}
		}
	}
	
	free(serialized_data);
	
	return 0;
}

/*
 * Load functions here
 */
void server_init(void) {
	command_add("beat", &command_beat);
	command_add("info", &command_info);
	command_add("join", &command_join);
	command_add("sitt", &command_sitt);
	command_add("quit", &command_quit);
	periodic_add(&periodic_close);
	periodic_add(&periodic_table_update);
	
	// Initialize blackjack game
	blackjack_init();
}

int payload_next_int(char **payload, int len) {
	int r = 0;
	
	while (len-- > 0)
		if (**payload != '\0')
			r += (*(*payload)++ - '0') * pow(10, len);
	
	return r;
}
