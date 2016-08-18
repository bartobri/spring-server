// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include "l1/periodictime.h"

//Static Variables
static int periodictime;

int periodictime_get(void) {
	return periodictime;
}

void periodictime_set(int t) {
	periodictime = t;
}
