// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <time.h>
#include "main.h"
#include "ptime.h"

//Static Variables
static time_t ptime;

void ptime_init(void) {
	ptime = time(NULL);
}

void ptime_set(int t) {
	ptime = t;
}

int ptime_get(void) {
	return (int)ptime;
}
