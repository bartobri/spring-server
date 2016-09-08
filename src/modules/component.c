// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "modules/command.h"
#include "modules/connectfunction.h"
#include "modules/disconnectfunction.h"
#include "modules/log.h"
#include "modules/mainsocket.h"
#include "modules/network.h"
#include "modules/periodic.h"
#include "modules/socketlist.h"

int write_socket(int s, char *command, char *payload) {
	int r;
	char *str;
	
	str = malloc(strlen(command) + strlen(payload) + 1);
	sprintf(str, "%s%s", command, payload);
	r = network_write(s, str);
	free(str);
	
	return r;
}

void close_socket(int s) {
	close(s);
	socketlist_remove(s);
}

int main_socket(void) {
	return mainsocket_get();
}

int next_socket(void) {
	return socketlist_get_next();
}

void reset_next_socket(void) {
	socketlist_reset_next();
}

void set_connect_function(connectFunctionType functionPtr) {
	connectfunction_set(functionPtr);
}

void set_disconnect_function(disconnectFunctionType functionPtr) {
	disconnectfunction_set(functionPtr);
}

void add_periodic_function(prdFunctionType functionPtr) {
	periodic_add(functionPtr);
}

void add_command_function(char *command, comFunctionType functionPtr) {
	command_add(command, functionPtr);
}



