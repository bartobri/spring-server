// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

/*
 * MODULE DESCRIPTION
 * 
 * The termflag module stores and provides access to a simple boolean
 * flag. This flag can be set from inside a custom function to tell the
 * main program loop to terminate the program.
 */

/*
 * Static Variables
 */
static int termflag;

/*
 * Initialize the flag to a false value.
 */
void termflag_init(void) {
	termflag = 0;
}

/*
 * Set the flag to a true value.
 */
void termflag_set(void) {
	termflag = 1;
}

/*
 * Return the flag value.
 */
int termflag_isset(void) {
	return termflag;
} 
