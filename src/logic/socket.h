#ifndef SOCKET_H
#define SOCKET_H 1

void socket_init(void);
int socket_read(int);
int socket_write(int, char *);
char *socket_get_buffer(void);
void socket_add(int);
void socket_remove(int);
int socket_get_timestamp(int);
void socket_update_timestamp(int);

#endif
