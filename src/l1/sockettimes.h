#ifndef SOCKETTIMES_H
#define SOCKETTIMES_H 1

// To ensure we have the FD_SETSIZE definition
#include <sys/select.h>

struct timestampTbl {
	int socket;
	int timestamp;
};

struct timestampTbl sockettimes_get(int);
void sockettimes_set(int, struct timestampTbl);

#endif
