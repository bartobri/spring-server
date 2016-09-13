// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <time.h>
#include "config.h"

/*
 * MODULE DESCRIPTION
 * 
 * The periodictime module manages a timestamp of the last time the
 * periodic functions were run.
 */

/*
 * Static Variables
 */
static int periodictime;

/*
 * Initialize periodictime to the current time.
 */
void nextperiodic_init(void) {
	periodictime = (int)time(NULL);
}

/*
 * Set periodictime to the current time.
 */
void nextperiodic_reset(void) {
	periodictime = (int)time(NULL);
}

/*
 * Check if the number of seconds configured for PERIODIC_SECONDS has
 * elapsed since the last time periodictime was set. Return a true value
 * if it has. Return a false value if it has not.
 */
int nextperiodic_elapsed(void) {
	if (periodictime <= time(NULL) - PERIODIC_SECONDS)
		return 1;
	
	return 0;
}
