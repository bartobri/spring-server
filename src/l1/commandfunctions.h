#ifndef COMMANDFUNCTIONS_H
#define COMMANDFUNCTIONS_H 1

#define COMMANDFUNCTIONS_LIMIT       10
#define COMMANDFUNCTIONS_ARGS        int socket, char *payload
#define COMMANDFUNCTIONS_RETURN      int

typedef COMMANDFUNCTIONS_RETURN (*comFunctionType)(COMMANDFUNCTIONS_ARGS);

struct commandTbl {
	char *command;
	comFunctionType functionPtr;
};

struct commandTbl commandfunctions_get(int);
void commandfunctions_set(int, struct commandTbl);

#endif
