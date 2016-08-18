#ifndef PERIODICFUNCTIONS_H
#define PERIODICFUNCTIONS_H 1

#define PERIODICFUNCTIONS_LIMIT      10
#define PERIODICFUNCTIONS_ARGS       void
#define PERIODICFUNCTIONS_RETURN     int

typedef PERIODICFUNCTIONS_RETURN (*prdFunctionType)(PERIODICFUNCTIONS_ARGS);

void periodicfunctions_set(int, prdFunctionType);
prdFunctionType periodicfunctions_get (int);

#endif
