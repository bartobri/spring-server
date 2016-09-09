// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdlib.h>
#include "modules/disconnectfunction.h"

// Static variables
disconnectFunctionType disconnectFunction;

void disconnectfunction_init(void) {
	disconnectFunction = NULL;
}

void disconnectfunction_set(disconnectFunctionType functionPtr) {
	disconnectFunction = functionPtr;
}

int disconnectfunction_exists(void) {
	if (disconnectFunction != NULL)
		return 1;
	
	return 0;
}

void disconnectfunction_exec(int socket) {
	if (disconnectFunction != NULL)
		disconnectFunction(socket);
}
