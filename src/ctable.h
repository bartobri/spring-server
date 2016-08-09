#ifndef COMMAND_H
#define COMMAND_H 1

#include <stdbool.h>
#include "main.h"

void ctable_init(void);
void ctable_add(char *command, COMMAND_RETURN (*functionPtr)(COMMAND_ARGS));
bool ctable_check(char *);
void ctable_execute(char *, char *, int);

#endif
