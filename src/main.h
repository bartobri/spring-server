#ifndef MAIN_H
#define MAIN_H 1

#define DEFAULT_PORT        51717
#define DEFAULT_HOST        NULL
#define BUFFER_SIZE         1024
#define COMMAND_SIZE        4
#define COMMAND_LIMIT       10
#define PERIODIC_SECONDS    5
#define SERVER              1
#define CLIENT              2

struct commandTable {
	const char *command;
	int (*functionPtr)(int, char *, fd_set *);
};

// Function prototypes defined in server.c and client.c
int startup(char *, char *);
void populate_commands(struct commandTable *);
int periodic(int, fd_set *);
void comp_cleanup(void);
int comp_type(void);

#endif
