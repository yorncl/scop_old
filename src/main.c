#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "render.h"

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
	int r = render(obj);
        clear_obj(obj);
	return r == EXIT_FAILURE ? r : EXIT_SUCCESS;
}
