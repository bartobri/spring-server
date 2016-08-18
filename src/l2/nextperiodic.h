#ifndef NEXTPERIODIC_H
#define NEXTPERIODIC_H 1

#include "l1/periodictime.h"

#define PERIODIC_SECONDS    5

void nextperiodic_init(void);
void nextperiodic_reset(void);
int nextperiodic_elapsed(void);

#endif
