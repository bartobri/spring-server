#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "if/comfunctions.h"

void comfunction_init(void) {
	comfunctions_init();
}

bool comfunction_exists(char *command) {
	int i;
	struct comFunctionsTbl *functions;
	
	functions = comfunctions_get();
	
	for (i = 0; i < COMFUNCTIONS_LIMIT; ++i) {
		
		// If we reached the end of the list, break out of the loop.
		if (functions[i].command == NULL)
			break;

		// If we have a match, return true.
		if (strcmp(functions[i].command, command) == 0)
			return true;
	}
	
	// No match found, return false.
	return false;
}

int comfunction_exec(char *command, char *payload, int socket) {
	int i, r;
	struct comFunctionsTbl *functions;
	
	r = 0;
	functions = comfunctions_get();
	
	for (i = 0; i < COMFUNCTIONS_LIMIT; ++i) {
		
		// If we reached the end of the list, break out of the loop.
		if (functions[i].command == NULL)
			break;

		// If we have a match, return true.
		if (strcmp(functions[i].command, command) == 0) {
			r = functions[i].functionPtr(socket, payload);
			break;
		}
	}
	
	return r;
}
