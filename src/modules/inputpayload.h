#ifndef INPUTPAYLOAD_H
#define INPUTPAYLOAD_H 1

void inputpayload_init(void);
void inputpayload_set(char *);
char *inputpayload_get(void);
void inputpayload_parse(char *);

#endif
