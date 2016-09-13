// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef SOCKETLIST_H
#define SOCKETLIST_H 1

/*
 * Function Declarations
 */
void socketlist_init(void);
void socketlist_add(int);
void socketlist_remove(int);
int socketlist_get_next(void);
void socketlist_reset_next(void);

#endif
