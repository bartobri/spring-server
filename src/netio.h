#ifndef NETIO_H
#define NETIO_H 1

int netio_startup(char *, char *);
int netio_wait(fd_set *);
int netio_accept(int);
void netio_read(void);
void netio_shutdown(void);

#endif
