// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

static int mainsocket;

void mainsocket_set(int n) {
	mainsocket = n;
}

int mainsocket_get(void) {
	return mainsocket;
}
