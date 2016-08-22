#ifndef READLIST_H
#define READLIST_H 1

void readlist_init(void);
void readlist_add(int);
int readlist_wait(void);
void readlist_remove(int);
int readlist_check(int);
int readlist_get_next(void);
void readlist_reset_next(void);

#endif
