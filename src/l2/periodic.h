#ifndef PERIODIC_H
#define PERIODIC_H 1

#include "l1/periodictime.h"
#include "l1/periodicfunctions.h"

#define PERIODIC_SECONDS    5

void periodic_init(void);
void periodic_update_time(void);
int periodic_time_elapsed(void);
void periodic_add(prdFunctionType);
void periodic_exec(void);

#endif
