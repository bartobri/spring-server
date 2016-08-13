// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include "main.h"
#include "if/prdfunctions.h"

// Static variables
static struct prdFunctionsTbl functions[PRDFUNCTIONS_LIMIT];

void prdfunctions_init(void) {
	int i;
	
	for (i = 0; i < PRDFUNCTIONS_LIMIT; ++i) {
		functions[i].functionPtr = NULL;
	}
}

void prdfunctions_add(PRDFUNCTIONS_RETURN (*functionPtr)(PRDFUNCTIONS_ARGS)) {
	int i;
	
	// Add passed parameters to end of commands[] array
	for (i = 0; i < PRDFUNCTIONS_LIMIT; ++i) {
		if (functions[i].functionPtr == NULL) {
			functions[i].functionPtr = functionPtr;
			break;
		}
	}
}

struct prdFunctionsTbl *prdfunctions_get (void) {
	return functions;
}
