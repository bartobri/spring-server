// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef CONNECTFUNCTION_H
#define CONNECTFUNCTION_H 1

/*
 * These symbolic constants define the parameters and the return value for
 * the functions executed in response to a new connection.
 */
#define CONNECTFUNCTION_PARAMS      int socket
#define CONNECTFUNCTION_RETURN      void

/*
 * Define a data type for a function pointer whos paramaters are
 * CONNECTFUNCTION_PARAMS and return value is CONNECTFUNCTION_RETURN.
 */
typedef CONNECTFUNCTION_RETURN (*connectFunctionType)(CONNECTFUNCTION_PARAMS);

/*
 * Function Declarations.
 */
void connectfunction_init(void);
void connectfunction_set(connectFunctionType);
int connectfunction_exists(void);
void connectfunction_exec(int);

#endif
