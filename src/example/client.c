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
	char o;
	int loop;

	(void)socket;
	(void)payload;
	
	printf("Table Info\n%s\n\n", payload);
	
	printf("Menu\n");
	printf("1 - Join\n");
	printf("2 - Leave\n\n");
	
	loop = 1;

	while (loop) {
		printf("Choose: ");
		o = getchar();
		
		switch(o) {
			case '1':
				network_write(socket, "join");
				loop = 0;
				break;
			case '2':
				network_write(socket, "quit");
				loop = 0;
				break;
			default:
				printf("Invalid option.\n");
		}
	}
	
	return 0;
}

COMMAND_RETURN command_aval(COMMAND_ARGS) {
	int sfw;
	int numTables;
	int numSeats;
	
	(void)socket;
	(void)payload;
	
	printf("%s\n", payload);
	
	sfw = *payload++ - '0';
	numTables = payload_next_int(&payload, sfw);
	numSeats = payload_next_int(&payload, sfw);
	
	printf("Field Width: %i\n", sfw);
	printf("Table Count: %i\n", numTables);
	printf("Seat Count: %i\n", numSeats);
	
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



