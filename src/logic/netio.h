#ifndef NETIO_H
#define NETIO_H 1

void netio_init(void);
int netio_startup(char *, char *);
int netio_accept(int);
char *netio_get_errmsg(void);

#endif
