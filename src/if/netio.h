#ifndef NETIO_H
#define NETIO_H 1

void netio_init(void);
int netio_startup(char *, char *);
int netio_wait(fd_set *);
int netio_accept(int);
int netio_read(int);
int netio_write(int, char *);
char *netio_get_buffer(void);
char *netio_get_errmsg(void);

#endif
