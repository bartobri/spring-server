#ifndef COMMAND_H
#define COMMAND_H 1

#include <stdbool.h>
#include "main.h"

void command_add(char *command, COMMAND_RETURN (*functionPtr)(COMMAND_ARGS));
bool command_check(char *);
void command_execute(char *, char *, int);

#endif
