// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <time.h>
#include "l1/prdtime.h"
#include "l1/prdfunction.h"
#include "l2/periodic.h"

void periodic_init(void) {
	int i;
	
	// prdtime init
	prdtime_set((int)time(NULL));
	
	// prdfunction array init
	for (i = 0; i < PRDFUNCTION_LIMIT; ++i) {
		prdfunction_set(i, NULL);
	}
}

void periodic_update_time(void) {
	prdtime_set((int)time(NULL));
}

int periodic_time_elapsed(void) {
	int curtime;
	
	curtime = prdtime_get();
	
	if (curtime <= time(NULL) - PERIODIC_SECONDS)
		return 1;
	
	return 0;
}

void periodic_add(prdFunctionType functionPtr) {
	int i;

	for (i = 0; i < PRDFUNCTION_LIMIT; ++i) {
		if (prdfunction_get(i) == NULL) {
			prdfunction_set(i, functionPtr);
			break;
		}
	}
}

void periodic_exec(void) {
	int i;
	prdFunctionType functionPtr;
	
	for (i = 0; i < PRDFUNCTION_LIMIT; ++i) {
		functionPtr = prdfunction_get(i);
		if (functionPtr != NULL) {
			functionPtr();
		}
	}
	
	periodic_update_time();
}
