#ifndef COMFUNCTIONS_H
#define COMFUNCTIONS 1

#define COMFUNCTIONS_LIMIT       10
#define COMFUNCTIONS_ARGS        int socket, char *payload
#define COMFUNCTIONS_RETURN      int

struct comFunctionsTbl {
	const char *command;
	COMFUNCTIONS_RETURN (*functionPtr)(COMFUNCTIONS_ARGS);
};

void comfunctions_init(void);
void comfunctions_add(char *, COMFUNCTIONS_RETURN (*)(COMFUNCTIONS_ARGS));
struct comFunctionsTbl *comfunctions_get (void);

#endif
