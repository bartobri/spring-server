#ifndef NETWORK_H
#define NETWORK_H 1

void network_init(void);
int network_start_server(char *, char *);
int network_start_client(char *, char *);
int network_accept(int);
int network_read(int);
char *network_get_readdata(void);
int network_write(int, char *);
char *network_get_errmsg(void);

#endif
