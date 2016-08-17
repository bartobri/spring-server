#ifndef READLIST_H
#define READLIST_H 1

#include <sys/select.h>

void readlist_init(void);
void readlist_add(int);
fd_set *readlist_getptr(void);
void readlist_remove(int);
int readlist_check(int);
int readlist_next(void);
void readlist_reset(void);

#endif
