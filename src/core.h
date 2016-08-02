#ifndef CORE_H
#define CORE_H 1

int core_startup(char *, char *);
int core_wait(void);
int core_accept(int);
int core_read(void);

#endif
