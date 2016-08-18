// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include "l2/periodic.h"

void periodic_init(void) {
	int i;
	
	// prdfunction array init
	for (i = 0; i < PERIODICFUNCTIONS_LIMIT; ++i) {
		periodicfunctions_set(i, 0);
	}
}

void periodic_add(prdFunctionType functionPtr) {
	int i;

	for (i = 0; i < PERIODICFUNCTIONS_LIMIT; ++i) {
		if (periodicfunctions_get(i) == 0) {
			periodicfunctions_set(i, functionPtr);
			break;
		}
	}
}

void periodic_exec(void) {
	int i;
	prdFunctionType functionPtr;
	
	for (i = 0; i < PERIODICFUNCTIONS_LIMIT; ++i) {
		functionPtr = periodicfunctions_get(i);
		if (functionPtr != 0) {
			functionPtr();
		}
	}
}
