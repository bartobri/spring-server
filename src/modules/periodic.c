// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include "config.h"
#include "modules/periodic.h"

/*
 * MODULE DESCRIPTION
 * 
 * The periodic module manages an array of function pointers referred to
 * as "periodic functions". It is responsible for initializing the array,
 * adding new functions to the array, and executing all functions
 * contained in the array.
 */

/*
 * Static Variables
 */
prdFunctionType functions[PERIODIC_LIMIT];

/*
 * Initialize the functions array with all NULL values.
 */
void periodic_init(void) {
	int i;
	
	// prdfunction array init
	for (i = 0; i < PERIODIC_LIMIT; ++i)
		functions[i] = NULL;
}

/*
 * Add a function pointer to the next available slot in the functions
 * array.
 */
void periodic_add(prdFunctionType functionPtr) {
	int i;

	for (i = 0; i < PERIODIC_LIMIT; ++i) {
		if (functions[i] == NULL) {
			functions[i] = functionPtr;
			break;
		}
	}
}

/*
 * Execute all function pointers contained in the functions array.
 */
void periodic_exec(void) {
	int i;
	
	for (i = 0; i < PERIODIC_LIMIT; ++i)
		if (functions[i] != NULL)
			functions[i]();
}
