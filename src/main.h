#ifndef MAIN_H
#define MAIN_H 1

#define BUFFER_SIZE         1024
#define COMMAND_SIZE        4
#define COMMAND_LIMIT       10
#define SERVER              1
#define CLIENT              2

struct commandTable {
	const char *command;
	int (*functionPtr)(char *);
};

// Function prototypes defined in server.c and client.c
int startup(char *, char *);
void initCommands(struct commandTable *);
void periodic(void);
void comp_cleanup(void);
int comp_type(void);

/*
int shutdown(char *);
int component_type(void);
*/

#endif
