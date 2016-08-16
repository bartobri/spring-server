// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include "main.h"
#include "l1/comfunctions.h"

// Static variables
static struct comFunctionsTbl functions[COMFUNCTIONS_LIMIT];

struct comFunctionsTbl comfunctions_get(int i) {
	return functions[i];
}

void comfunctions_set(int i, struct comFunctionsTbl entry) {
	functions[i] = entry;
}
