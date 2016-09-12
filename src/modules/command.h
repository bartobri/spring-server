// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef COMMAND_H
#define COMMAND_H 1

/*
 * These symbolic constants define the parameters and the return value for
 * functions executed in response to a given command.
 */
#define COMMAND_ARGS        int socket, char *payload
#define COMMAND_RETURN      void

/*
 * Define a data type for a function pointer whos paramaters are
 * COMMAND_ARGS and return value is COMMAND_RETURN.
 */
typedef COMMAND_RETURN (*comFunctionType)(COMMAND_ARGS);

/*
 * Function Declarations
 */
void command_init(void);
void command_add(char *, comFunctionType);
int command_exists(char *command);
void command_exec(char *command, char *payload, int socket);

#endif
