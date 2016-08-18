#ifndef SOCKET_H
#define SOCKET_H 1

int socket_read(int);
int socket_write(int, char *);
char *socket_get_buffer(void);

#endif
