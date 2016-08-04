#ifndef READLIST_H
#define READLIST_H 1

void readlist_set(fd_set);
fd_set readlist_get(void);
fd_set *readlist_getptr(void);
void readlist_remove(int);
bool readlist_check(int);
int readlist_next(void);
void readlist_reset(void);

#endif
