#ifndef COMFUNCTION_H
#define COMFUNCTION 1

#include "l1/comfunctions.h"

void comfunction_init(void);
void comfunction_add(char *, comFunctionType);
int comfunction_exists(char *command);
int comfunction_exec(char *command, char *payload, int socket);

#endif
