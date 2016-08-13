#include <stdio.h>
#include "if/prdfunctions.h"

void prdfunction_init(void) {
	prdfunctions_init();
}

int prdfunction_exec(void) {
	int i, r;
	struct prdFunctionsTbl *functions;
	
	functions = prdfunctions_get();
	
	for (i = 0; i < PRDFUNCTIONS_LIMIT; ++i) {
		
		// If we reached the end of the list, break out of the loop.
		if (functions[i].functionPtr == NULL)
			break;

		// If we have a match, return true.
		r = functions[i].functionPtr();
		
		// Return if any periodic function returns an error (less than zero)
		if (r < 0)
			return -1;
	}
	
	return 0;
}
