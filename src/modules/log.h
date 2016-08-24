#ifndef LOG_H
#define LOG_H 1

void log_init(void);
int log_open(int);
int log_open_server(void);
int log_open_client(void);
void log_write(char *, ...);
void log_close(void);
char *log_get_errmsg(void);

#endif
