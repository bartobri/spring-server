// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.


static int termflag;

void termflag_init(void) {
	termflag = 0;
}

void termflag_set(void) {
	termflag = 1;
}

int termflag_isset(void) {
	return termflag;
} 
