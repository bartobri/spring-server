#ifndef BUFFER_H
#define BUFFER_H 1

void buffer_init(void);
char *buffer_get(void);
void buffer_set(char *);
char *buffer_get_command(void);
char *buffer_get_payload(void);

#endif
