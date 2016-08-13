#ifndef TIMESTAMPS_H
#define TIMESTAMPS_H 1

void timestamps_init(void);
void timestamps_add(int, int);
void timestamps_set(int, int);
int  timestamps_get(int);
void timestamps_remove(int);

#endif
