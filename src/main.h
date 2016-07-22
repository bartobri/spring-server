#ifndef MAIN_H
#define MAIN_H 1

#define BUFFER_SIZE         1024
#define COMMAND_SIZE        4
#define SERVER              1
#define CLIENT              2

// Function prototypes defined in server.c and client.c
int startup(char *, char *);
void periodic(void);
void comp_cleanup(void);
int comp_type(void);

/*
int shutdown(char *);
int component_type(void);
*/

#endif
