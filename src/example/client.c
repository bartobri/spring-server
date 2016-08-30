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

int payload_next_int(char **, int);

COMMAND_RETURN command_helo(COMMAND_ARGS) {
	(void)socket;
	(void)payload;
	
	network_write(socket, "info");
	
	return 0;
}

COMMAND_RETURN command_info(COMMAND_ARGS) {
	int i, c, v;

	(void)socket;
	(void)payload;
	
	printf("Table Info\n%s\n\n", payload);
	
	printf("Menu\n");
	printf("1 - Join\n");
	printf("2 - Leave\n\n");

	printf("Choose: ");
	while (1) {

		// Get user input. Invalid if any non-digits.
		for (v = 0, i = 0; (c = getchar()) != '\n'; ++i) {
			if (c >= '0' && c <= '9')
				v = (v * 10) + c - '0';
			else {
				while (getchar() != '\n')
					v = 0;
				break;
			}
		}
		
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
	int numTables;
	int numSeats;
	
	(void)socket;
	(void)payload;
	
	//printf("%s\n", payload);
	
	sfw = *payload++ - '0';
	numTables = payload_next_int(&payload, sfw);
	numSeats = payload_next_int(&payload, sfw);
	
	//printf("Field Width: %i\n", sfw);
	//printf("Table Count: %i\n", numTables);
	//printf("Seat Count: %i\n", numSeats);
	
	for (t = 0; t < numTables; ++t) {
	
		int tid = payload_next_int(&payload, sfw);
		printf("Table %i: ", tid);

		for (s = 0; s < numSeats; ++s) {
			
			int sid = payload_next_int(&payload, sfw);
			int occ = payload_next_int(&payload, sfw);
			printf("%i(%i) ", sid, occ);
		}
		
		printf("\n");
		
	}
	
	//int tc, sc;
	
	//printf("Chosoe Table: ");
	//while (scanf("%i", &tc) > 0) {
	//}
	
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
	periodic_add(&send_heartbeat);
}

int payload_next_int(char **payload, int len) {
	int r = 0;
	
	while (len-- > 0)
		if (**payload != '\0')
			r += (*(*payload)++ - '0') * pow(10, len);
	
	return r;
}



