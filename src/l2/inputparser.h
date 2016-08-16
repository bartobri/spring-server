#ifndef INPUTPARSER_H
#define INPUTPARSER_H 1

#define COMMAND_SIZE   4
#define PAYLOAD_SIZE   1000

void inputparser_init(void);
void inputparser_set_input(char *);
char *inputparser_get_command(void);
char *inputparser_get_payload(void);

#endif
