#ifndef BUFFER_H
#define BUFFER_H 1

#define BUFFER_SIZE         1024

void buffer_init(void);
void buffer_set(char *);
char *buffer_get(void);

#endif
