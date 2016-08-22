// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include "modules/periodic.h"

// Static variables
prdFunctionType functions[PERIODIC_LIMIT];

void periodic_init(void) {
	int i;
	
	// prdfunction array init
	for (i = 0; i < PERIODIC_LIMIT; ++i)
		functions[i] = NULL;
}

void periodic_add(prdFunctionType functionPtr) {
	int i;

	for (i = 0; i < PERIODIC_LIMIT; ++i) {
		if (functions[i] == NULL) {
			functions[i] = functionPtr;
			break;
		}
	}
}

void periodic_exec(void) {
	int i;
	
	for (i = 0; i < PERIODIC_LIMIT; ++i)
		if (functions[i] != NULL)
			functions[i]();
}
