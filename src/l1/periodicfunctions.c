// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include "l1/periodicfunctions.h"

// Static variables
prdFunctionType functions[PERIODICFUNCTIONS_LIMIT];

void periodicfunctions_set(int i, prdFunctionType functionPtr) {
	functions[i] = functionPtr;
}

prdFunctionType periodicfunctions_get (int i) {
	return functions[i];
}
