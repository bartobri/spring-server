// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include "modules/connectfunction.h"

// Static variables
connectFunctionType connectFunction;

void connectfunction_init(void) {
	connectFunction = NULL;
}

void connectfunction_set(connectFunctionType functionPtr) {
	connectFunction = functionPtr;
}

int connectfunction_exists(void) {
	if (connectFunction != NULL)
		return 1;
	
	return 0;
}

void connectfunction_exec(int socket) {
	if (connectFunction != NULL)
		connectFunction(socket);
}
