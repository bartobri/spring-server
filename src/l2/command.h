#ifndef COMMAND_H
#define COMMAND_H 1

#include "l1/commandfunctions.h"

void command_init(void);
void command_add(char *, comFunctionType);
int command_exists(char *command);
int command_exec(char *command, char *payload, int socket);

#endif
