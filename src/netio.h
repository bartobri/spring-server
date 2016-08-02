#ifndef NETIO_H
#define NETIO_H 1

int netio_startup(char *, char *);
int netio_wait(void);
int netio_accept(int);
int netio_read(void);

#endif
