// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef INPUTPAYLOAD_H
#define INPUTPAYLOAD_H 1

/*
 * Function Declarations
 */
void inputpayload_init(void);
char *inputpayload_get(void);
void inputpayload_parse(char *);

#endif
