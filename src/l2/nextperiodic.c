// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <time.h>
#include "l2/nextperiodic.h"

//Static Variables
static int periodictime;

void nextperiodic_init(void) {
	periodictime = (int)time(NULL);
}

void nextperiodic_reset(void) {
	periodictime = (int)time(NULL);
}

int nextperiodic_elapsed(void) {
	if (periodictime <= time(NULL) - PERIODIC_SECONDS)
		return 1;
	
	return 0;
}
