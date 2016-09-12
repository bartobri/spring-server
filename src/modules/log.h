// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef LOG_H
#define LOG_H 1

/*
 * Function Declarations
 */
 
void log_init(void);
int log_open(int);
int log_open_server(void);
int log_open_client(void);
void log_write(char *, ...);
void log_print(char *, ...);
void log_close(void);
char *log_get_errmsg(void);

#endif
