// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef SOCKETTIME_H
#define SOCKETTIME_H 1

/*
 * Function Declarations
 */
void sockettime_init(void);
void sockettime_set(int);
int sockettime_get(int);
int sockettime_elapsed(int);

#endif
