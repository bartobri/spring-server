#ifndef SOCKETLIST_H
#define SOCKETLIST_H 1

void socketlist_init(void);
void socketlist_add(int);
void socketlist_remove(int);
int socketlist_get_next(void);
void socketlist_reset_next(void);

#endif
