#ifndef SOCKLIST_H
#define SOCKLIST_H 1

#include <sys/select.h>

void socklist_init(void);
void socklist_add(int);
void socklist_remove(int);
fd_set socklist_get(void);
int socklist_next(void);
void socklist_reset(void);
void socklist_add_mainsock(int);
int socklist_get_mainsock(void);

#endif
