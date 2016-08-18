// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <sys/select.h>
#include "l1/sockettimes.h"

// Static vars
static struct timestampTbl times[FD_SETSIZE];

struct timestampTbl sockettimes_get(int i) {
	return times[i];
}

void sockettimes_set(int i, struct timestampTbl entry) {
	times[i] = entry;
}
