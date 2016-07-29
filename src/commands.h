#ifndef COMMANDS_H
#define COMMANDS_H 1

#include <stdbool.h>
#include "main.h"

void load_command(char *command, int (*functionPtr)(COMMAND_ARGS));
bool valid_command(char *);
void execute_command(char *, char *, int);

#endif
