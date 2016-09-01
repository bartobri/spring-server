// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "modules/network.h"
#include "modules/command.h"
#include "modules/periodic.h"
#include "modules/socketlist.h"
#include "modules/mainsocket.h"
#include "config.h"

int payload_next_int(char **, int);
int get_user_int(void);

COMMAND_RETURN command_helo(COMMAND_ARGS) {
	(void)socket;
	(void)payload;
	
	network_write(socket, "info");
	
	return 0;
}

COMMAND_RETURN command_info(COMMAND_ARGS) {
	int v;

	(void)socket;
	(void)payload;
	
	printf("Table Info\n%s\n\n", payload);
	
	printf("Menu\n");
	printf("1 - Join\n");
	printf("2 - Leave\n\n");

	printf("Choose: ");
	while (1) {

		v = get_user_int();
		
		switch(v) {
			case 1:
				network_write(socket, "join");
				break;
			case 2:
				network_write(socket, "quit");
				break;
			default:
				printf("Invalid option. Choose again: ");
				continue;
		}
		
		break;
	}
	
	return 0;
}

COMMAND_RETURN command_aval(COMMAND_ARGS) {
	int t, s;
	int sfw;
	int numTables, numSeats;
	int tc, sc;
	
	(void)socket;
	(void)payload;
	
	//printf("%s\n", payload);
	
	sfw = *payload++ - '0';
	numTables = payload_next_int(&payload, sfw);
	numSeats = payload_next_int(&payload, sfw);
	
	//printf("Field Width: %i\n", sfw);
	//printf("Table Count: %i\n", numTables);
	//printf("Seat Count: %i\n", numSeats);
	
	int tableIDs[numTables];
	int seatIDs[numTables][numSeats];
	int seatOcc[numTables][numSeats];
	
	for (t = 0; t < numTables; ++t) {
	
		int tid = payload_next_int(&payload, sfw);
		printf("Table %i: ", tid);
		
		tableIDs[t] = tid;

		for (s = 0; s < numSeats; ++s) {
			
			int sid = payload_next_int(&payload, sfw);
			int occ = payload_next_int(&payload, sfw);
			printf("%i(%i) ", sid, occ);

			seatIDs[t][s] = sid;
			seatOcc[t][s] = occ;
		}
		
		printf("\n");
		
	}

	printf("Choose Table: ");
	while (1) {
		tc = get_user_int();
		
		int i;
		for (i = 0; i < numTables; ++i)
			if (tableIDs[i] == tc)
				break;
				
		if (i < numTables)
			break;
		else
			printf("Invalid Table ID. Choose Table: ");
	}
	
	printf("Choose Seat: ");
	while (1) {
		sc = get_user_int();
		
		int i;
		for(i = 0; i < numSeats; ++i)
			if (seatIDs[tc][i] == sc && seatOcc[tc][i] == 0)
				break;
				
		if (i < numSeats)
			break;
		else
			printf("Invalid Seat ID. Choose Seat: ");
	}
	
	char *serialized_data = malloc(COMMAND_SIZE + sfw  + sfw + 1);
	
	sprintf(serialized_data, "sitt");
	sprintf(serialized_data + strlen(serialized_data), "%.*i", sfw, tc);
	sprintf(serialized_data + strlen(serialized_data), "%.*i", sfw, sc);
	
	network_write(socket, serialized_data);
	
	free(serialized_data);
	
	//printf("Table: %i, Seat: %i\n", tc, sc);
	
	return 0;
}

COMMAND_RETURN command_sitt(COMMAND_ARGS) {
	(void)socket;
	(void)payload;
	
	if (*payload == '0') {
		// Display error message
		printf("\nError: %s\n\n", payload);
	
		// request table data again
		network_write(socket, "join");
	} else {
		// Display success message
		printf("\nYou are sitting at the table. Please wait...\n\n");
	}

	return 0;
}

COMMAND_RETURN command_tbst(COMMAND_ARGS) {
	int d, s, h, c;

	(void)socket;
	(void)payload;
	
	int sfw = *payload++ - '0';
	int numSeats = payload_next_int(&payload, sfw);
	int numHands = payload_next_int(&payload, sfw);
	int numCards = payload_next_int(&payload, sfw);
	
	printf("Dealer\n\t");
	for (d = 0; d < numCards; ++d) {
		int cardId = payload_next_int(&payload, sfw);
		printf("%i ", cardId);
	}
	printf("\n\n");
	
	for (s = 0; s < numSeats; ++s) {
		int seatId = payload_next_int(&payload, sfw);
		int occ = payload_next_int(&payload, sfw);
		printf("Seat %i (%s)\n", seatId, occ ? "occupied" : "unoccupied");
		for (h = 0; h < numHands; ++h) {
			int handNum = payload_next_int(&payload, sfw);
			printf("\t%i: ", handNum);
			for (c = 0; c < numCards; ++c) {
				int cardId = payload_next_int(&payload, sfw);
				printf("%i ", cardId);
			}
			printf("\n");
		}
		printf("\n");
	}
	
	//printf("%s\n", payload);

	return 0;
}

PERIODIC_RETURN send_heartbeat(PERIODIC_ARGS) {
	int r;

	r = network_write(mainsocket_get(), "beat");
	
	return r;
}

void client_init(void) {
	command_add("helo", &command_helo);
	command_add("info", &command_info);
	command_add("aval", &command_aval);
	command_add("sitt", &command_sitt);
	command_add("tbst", &command_tbst);
	periodic_add(&send_heartbeat);
}

int payload_next_int(char **payload, int len) {
	int r = 0;
	
	while (len-- > 0)
		if (**payload != '\0')
			r += (*(*payload)++ - '0') * pow(10, len);
	
	return r;
}

int get_user_int(void) {
	int c, v;
	
	v = 0;
	while ((c = getchar()) != '\n') {
		if (c >= '0' && c <= '9')
			v = (v * 10) + c - '0';
		else {
			while (getchar() != '\n')
				;
			v = 0;
			break;
		}
	}
	
	return v;
}



