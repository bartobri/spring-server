#ifndef CONNECTFUNCTION_H
#define CONNECTFUNCTION_H 1

#define CONNECTFUNCTION_ARGS        int socket
#define CONNECTFUNCTION_RETURN      void

typedef CONNECTFUNCTION_RETURN (*connectFunctionType)(CONNECTFUNCTION_ARGS);

void connectfunction_init(void);
void connectfunction_set(connectFunctionType);
int connectfunction_exists(void);
void connectfunction_exec(int);

#endif
