#ifndef INPUTPAYLOAD_H
#define INPUTPAYLOAD_H 1

#define INPUTPAYLOAD_SIZE   1000

void inputpayload_init(void);
void inputpayload_set(char *);
char *inputpayload_get(void);

#endif
