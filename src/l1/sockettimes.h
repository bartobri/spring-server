#ifndef SOCKETTIMES_H
#define SOCKETTIMES_H 1

struct timestampTbl {
	int socket;
	int timestamp;
};

struct timestampTbl sockettimes_get(int);
void sockettimes_set(int, struct timestampTbl);

#endif
