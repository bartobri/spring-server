// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef NETWORK_H
#define NETWORK_H 1

/*
 * Function Declarations
 */
void network_init(void);
int network_start_server(char *, char *);
int network_start_client(char *, char *);
int network_accept(int);
int network_read(int);
char *network_get_readdata(int);
int network_write(int, char *);
void network_close(int);
char *network_get_errmsg(void);
char *network_get_ipaddress(void);

#endif
