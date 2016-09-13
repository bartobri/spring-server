// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef READLIST_H
#define READLIST_H 1

/*
 * Function Declarations
 */
void readlist_init(void);
void readlist_add(int);
int readlist_wait(void);
void readlist_remove(int);
int readlist_check(int);
int readlist_get_next(void);
void readlist_reset_next(void);

#endif
