// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include "main.h"
#include "l1/prdfunction.h"

// Static variables
static struct prdFunctionTbl functions[PRDFUNCTION_LIMIT];

void prdfunction_set(int i, prdFunctionType functionPtr) {
	functions[i].functionPtr = functionPtr;
}

prdFunctionType prdfunction_get (int i) {
	return functions[i].functionPtr;
}
