#ifndef COMMAND_H
#define COMMAND_H 1

#define COMMAND_SIZE         4

void command_init(void);
void command_set(char *);
char *command_get(void);

#endif
