#ifndef PERIODIC_H
#define PERIODIC_H 1

#define PERIODIC_SECONDS    5

void periodic_init(void);
void periodic_update_time(void);
int periodic_time_expired(void);

#endif
