#ifndef FTABLE_H
#define FTABLE_H 1

#include <stdbool.h>
#include "main.h"

void ftable_init(void);
void ftable_add_command(char *command, FUNCTION_RETURN (*functionPtr)(COMMAND_ARGS));
void ftable_add_periodic(FUNCTION_RETURN (*functionPtr)(PERIODIC_ARGS));
bool ftable_check_command(char *);
int ftable_exec_command(char *, char *, int);
int ftable_exec_periodic(void);

#endif
