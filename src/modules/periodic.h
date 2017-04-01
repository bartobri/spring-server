// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef PERIODIC_H
#define PERIODIC_H 1

/*
 * These symbolic constants define the parameters and the return value for
 * functions executed at a timed interval.
 */
#define PERIODIC_PARAMS     void
#define PERIODIC_RETURN     void

/*
 * Define a data type for a function pointer whos paramaters are
 * PERIODIC_PARAMS and return value is PERIODIC_RETURN.
 */
typedef PERIODIC_RETURN (*prdFunctionType)(PERIODIC_PARAMS);

/*
 * Function Declarations
 */
void periodic_init(void);
void periodic_add(prdFunctionType);
void periodic_exec(void);

#endif
