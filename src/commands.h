#ifndef COMMANDS_H
#define COMMANDS_H 1

#include <stdbool.h>

void load_command(char *command, int (*functionPtr)(int, char *));
bool valid_command(char *);
void execute_command(char *, char *, int);

#endif
