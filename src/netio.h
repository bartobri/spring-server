#ifndef NETIO_H
#define NETIO_H 1

void netio_startup(char *, char *);
int netio_wait(void);
int netio_accept(void);
int netio_read(void);
void netio_shutdown(void);

#endif
