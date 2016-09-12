// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef COMPONENT_H
#define COMPONENT_H 1

/*
 * Module headers that have declarations that this header needs.
 */
#include "modules/command.h"
#include "modules/connectfunction.h"
#include "modules/disconnectfunction.h"
#include "modules/periodic.h"
#include "modules/log.h"

/*
 * Using a macro to wrap log_write() and log_print() since they are
 * variadic functions, which makes creating a regular function wrapper
 * slightly more difficult.
 */
#define write_log(...) log_write(__VA_ARGS__)
#define print_log(...) log_print(__VA_ARGS__)

/*
 * Function declarations.
 */
int write_socket(int, char *, char *);
void close_socket(int);
int main_socket(void);
int next_socket(void);
void reset_next_socket(void);
void set_connect_function(connectFunctionType);
void set_disconnect_function(disconnectFunctionType);
void add_periodic_function(prdFunctionType);
void add_command_function(char *, comFunctionType);
void terminate(void);

#endif
