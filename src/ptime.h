#ifndef PTIME_H
#define PTIME_H 1

#include <stdbool.h>

void ptime_init(void);
int ptime_get(void);
void ptime_set(int);
void ptime_reset(void);
bool ptime_expired(void);

#endif
