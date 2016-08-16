#ifndef SOCKET_H
#define SOCKET_H 1

#include <sys/select.h>

void socket_init(void);
int socket_read(int);
int socket_write(int, char *);
char *socket_get_buffer(void);
void socket_add(int);
void socket_add_main(int socket);
void socket_remove(int);
fd_set socket_get_list(void);
int socket_get_main(void);
int socket_get_timestamp(int);
void socket_update_timestamp(int);
void socket_shutdown_all(void);
void socket_shutdown(int);
int socket_next(void);

#endif
