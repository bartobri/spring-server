#ifndef COMFUNCTIONS_H
#define COMFUNCTIONS 1

#define COMFUNCTIONS_LIMIT       10
#define COMFUNCTIONS_ARGS        int socket, char *payload
#define COMFUNCTIONS_RETURN      int

typedef COMFUNCTIONS_RETURN (*comFunctionType)(COMFUNCTIONS_ARGS);

struct comFunctionsTbl {
	const char *command;
	comFunctionType functionPtr;
};

struct comFunctionsTbl comfunctions_get(int);
void comfunctions_set(int i, struct comFunctionsTbl entry);

#endif
