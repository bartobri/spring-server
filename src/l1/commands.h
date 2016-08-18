#ifndef COMMANDS_H
#define COMMANDS_H 1

#define COMMANDS_LIMIT       10
#define COMMANDS_ARGS        int socket, char *payload
#define COMMANDS_RETURN      int

typedef COMMANDS_RETURN (*comFunctionType)(COMMANDS_ARGS);

struct commandTbl {
	const char *command;
	comFunctionType functionPtr;
};

struct commandTbl commands_get(int);
void commands_set(int, struct commandTbl);

#endif
