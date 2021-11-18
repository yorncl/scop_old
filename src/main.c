#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

void print_usage()
{
	printf("Usage : ./scop OBJFILE\n");
}

int main(int ac, char** av)
{
	if (ac != 2)
	{
		print_usage();
		return 0;
	}
	Obj* obj = parse_file(av[1]);
	if (obj == NULL)
		return EXIT_FAILURE;
	clear_obj(obj);
	return EXIT_SUCCESS;
}
