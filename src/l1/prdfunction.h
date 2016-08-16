#ifndef PRDFUNCTION_H
#define PRDFUNCTION_H 1

#define PRDFUNCTION_LIMIT      10
#define PRDFUNCTION_ARGS       void
#define PRDFUNCTION_RETURN     int

struct prdFunctionTbl {
	PRDFUNCTION_RETURN (*functionPtr)(PRDFUNCTION_ARGS);
};

typedef PRDFUNCTION_RETURN (*prdFunctionType)(PRDFUNCTION_ARGS);

void prdfunction_set(int, prdFunctionType);
prdFunctionType prdfunction_get (int);

#endif
