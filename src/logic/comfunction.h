#ifndef COMFUNCTION_H
#define COMFUNCTION 1

#include <stdbool.h>

void comfunction_init(void);
bool comfunction_exists(char *command);
int  comfunction_exec(char *command, char *payload, int socket);

#endif
