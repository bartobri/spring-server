// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include "prdtime.h"

//Static Variables
static int prdtime;

int prdtime_get(void) {
	return prdtime;
}

void prdtime_set(int t) {
	prdtime = t;
}
