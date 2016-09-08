#ifndef COMPONENT_H
#define COMPONENT_H 1

#include "modules/command.h"
#include "modules/connectfunction.h"
#include "modules/disconnectfunction.h"
#include "modules/periodic.h"
#include "modules/log.h"

#define write_log(...) log_write(__VA_ARGS__)
#define print_log(...) log_print(__VA_ARGS__)

int write_socket(int, char *, char *);
void close_socket(int);
int main_socket(void);
int next_socket(void);
void reset_next_socket(void);
void set_connect_function(connectFunctionType);
void set_disconnect_function(disconnectFunctionType);
void add_periodic_function(prdFunctionType);
void add_command_function(char *, comFunctionType);

#endif
