// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

/*
 * MODULE DESCRIPTION
 * 
 * The mainsocket module stores and provides access to the main socket
 * file descriptor, as an integer value. For the client, this is the
 * socket used to communicate with the server. For the server, this is
 * the socket used to listen for new connections.
 */
 
/*
 * Static Variables
 */
static int mainsocket;

/*
 * Initialize the mainsocket static variable to NULL (zero).
 */
void mainsocket_init(void) {
	mainsocket = 0;
}

/*
 * Store the integer value for the main socket file descriptor.
 */
void mainsocket_set(int n) {
	mainsocket = n;
}

/*
 * Return the integer value for the main socket file descriptor.
 */
int mainsocket_get(void) {
	return mainsocket;
}
