// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <time.h>
#include "l1/prdtime.h"
#include "l2/periodic.h"

void periodic_init(void) {
	prdtime_set((int)time(NULL));
}

void periodic_update_time(void) {
	prdtime_set((int)time(NULL));
}

int periodic_time_expired(void) {
	int curtime;
	
	curtime = prdtime_get();
	
	if (curtime <= time(NULL) - PERIODIC_SECONDS)
		return 1;
	
	return 0;
}
