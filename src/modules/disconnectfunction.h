#ifndef DISCONNECTFUNCTION_H
#define DISCONNECTFUNCTION_H 1

#define DISCONNECTFUNCTION_ARGS        int socket
#define DISCONNECTFUNCTION_RETURN      void

typedef DISCONNECTFUNCTION_RETURN (*disconnectFunctionType)(DISCONNECTFUNCTION_ARGS);

void disconnectfunction_init(void);
void disconnectfunction_set(disconnectFunctionType);
int disconnectfunction_exists(void);
void disconnectfunction_exec(int);

#endif
