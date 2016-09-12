// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef INPUTCOMMAND_H
#define INPUTCOMMAND_H 1

/*
 * Function Declarations
 */
void inputcommand_init(void);
void inputcommand_set(char *);
char *inputcommand_get(void);
void inputcommand_parse(char *);

#endif
