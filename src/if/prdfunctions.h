#ifndef PRDFUNCTIONS_H
#define PRDFUNCTIONS_H 1

#define PRDFUNCTIONS_LIMIT      10
#define PRDFUNCTIONS_ARGS       void
#define PRDFUNCTIONS_RETURN     int

struct prdFunctionsTbl {
	PRDFUNCTIONS_RETURN (*functionPtr)(PRDFUNCTIONS_ARGS);
};

void prdfunctions_init(void);
void prdfunctions_add(PRDFUNCTIONS_RETURN (*)(PRDFUNCTIONS_ARGS));
struct prdFunctionsTbl *prdfunctions_get (void);

#endif
