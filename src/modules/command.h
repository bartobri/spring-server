#ifndef COMMAND_H
#define COMMAND_H 1

#define COMMAND_ARGS        int socket, char *payload
#define COMMAND_RETURN      void

typedef COMMAND_RETURN (*comFunctionType)(COMMAND_ARGS);

struct commandTbl {
	char *command;
	comFunctionType functionPtr;
};

void command_init(void);
void command_add(char *, comFunctionType);
int command_exists(char *command);
void command_exec(char *command, char *payload, int socket);

#endif
