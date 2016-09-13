// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef DISCONNECTFUNCTION_H
#define DISCONNECTFUNCTION_H 1

/*
 * These symbolic constants define the parameters and the return value for
 * the functions executed in response to a disconnection.
 */
#define DISCONNECTFUNCTION_PARAMS      int socket
#define DISCONNECTFUNCTION_RETURN      void

/*
 * Define a data type for a function pointer whos paramaters are
 * DISCONNECTFUNCTION_PARAMS and return value is DISCONNECTFUNCTION_RETURN.
 */
typedef DISCONNECTFUNCTION_RETURN (*disconnectFunctionType)(DISCONNECTFUNCTION_PARAMS);

/*
 * Function declarations.
 */
void disconnectfunction_init(void);
void disconnectfunction_set(disconnectFunctionType);
int disconnectfunction_exists(void);
void disconnectfunction_exec(int);

#endif
