#include <stdio.h>
#include <stdlib.h>
#include "parser.hpp"

int parser(char* filename)
{
	printf("About to parse everything in : %s \n", filename);

	FILE* f = fopen(filename, "r");
	if (!f)
		return EXIT_FAILURE;
	return 0;
}
