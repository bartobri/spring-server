#ifndef SOCKETTIME_H
#define SOCKETTIME_H 1

#include "l1/sockettimes.h"

void sockettime_init(void);
void sockettime_set(int);
int sockettime_get(int);

#endif
